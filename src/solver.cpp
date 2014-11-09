#include "precompiled.h"
#include "solver.h"

Solver::Solver(void)
{

}

void Solver::reset()
{
    solution.resize(0);
}

size_t Solver::count(void)
{
    size_t sum = 0;

    for(size_t y = 0 ; y < solution.size() ; y++ )
        sum += solution[y].count();

    return sum;
}

void Solver::flip(size_t x, size_t y)
{
    solution[y].flip(x);
}

bool Solver::isSolved(void) const
{
    return solution.size() > 0;
}

bool Solver::get(const size_t x, const size_t y) const
{
    if(isSolved())
        return solution[y].test(x);
    else
        return false;
}

size_t Solver::solve(Grid2D board)
{
    // initialise les differentes valeurs
    size_t nbSolution = 0;
    solution.clear();

    Grid2D equations;
    boost::dynamic_bitset<unsigned char> result;

    toctic("start");

    size_t side = board.size();
    // initialise la 2eme matrice
    result.resize(side * side);
    for(size_t y = 0 ; y < side ; y++)
        for(size_t x = 0 ; x < side ; x++)
            result.set(y * side + x, ! board[y][x]);

    toctic("init result");

    // initialise la 1ere matrice
    // qui decrit les relations entre les points
    equations.resize(side * side);
#ifdef USE_OPENMP
    #pragma omp parallel for
#endif
    for(long yMatrix = 0 ; yMatrix < side * side ; yMatrix++)
    {
        equations[yMatrix].resize((side * side) | 0xFF);

        // same point
        equations[yMatrix].set(yMatrix, true);

        // right point
        if((yMatrix % side) + 1 != side)
            equations[yMatrix].set(yMatrix + 1, true);

        // left point
        if((yMatrix % side) != 0)
            equations[yMatrix].set(yMatrix - 1, true);

        // upper point
        if(yMatrix >= side)
            equations[yMatrix].set(yMatrix - side, true);

        // lower point
        if(yMatrix < (side*side - side))
            equations[yMatrix].set(yMatrix + side, true);
    }

    toctic("init relations");

    // les matrices representent un systeme d'equations a side * side inconnues
    // resolution par la methode du pivot de gauss

    // 1ere boucle, transformation de la matrice en matrice triangulaire superieure
    for(size_t yMatrix = 0; yMatrix < side * side; yMatrix++)
    {
        // cherche une ligne de valeur courante 1 pour s'en servir comme pivot
        if(equations[yMatrix].test(yMatrix) == false)
        {
            for(size_t pivot = yMatrix + 1 ; pivot < side*side ; pivot++)
            {
                if(equations[pivot].test(yMatrix) == true)
                {
                    equations[yMatrix].swap(equations[pivot]);
                    result[yMatrix] ^= result[pivot];
                    result[pivot]   ^= result[yMatrix];
                    result[yMatrix] ^= result[pivot];
                    break;
                }
            }
        }

        if(equations[yMatrix].test(yMatrix) == false)
            continue;

        //for(size_t y = 0 ; y < side * side ; y++)
        //{
        //	std::string s;
        //	for(size_t x = 0 ; x < side * side ; x++)
        //		s.append(1, equations[y][x] == true ? 'X' : '.');
        //	qDebug("%s", s.c_str());
        //}
        //qDebug("");

        // soustrait le pivot aux lignes qui suivent
        size_t max = yMatrix + side + 1;
        if(side * side - 1 < max)
            max = side * side - 1;

        for(size_t y = yMatrix + 1 ; y <= max ; y++)
        {
            if(equations[y].test(yMatrix) == true) // OPTIM_BOTTLENECK
            {
                result[y] ^= result.test(yMatrix);

#if OPTIMISATION_NONE && !OPTIMISATION_MMX && !OPTIMISATION_SSE
                equations[y] ^= equations[yMatrix];
#endif

#if OPTIMISATION_MMX && ! OPTIMISATION_SSE
                __m64* pivot = const_cast<__m64*>(
                    reinterpret_cast<__m64*>(equations[yMatrix].m_bits.data()));

                __m64* line = const_cast<__m64*>(
                    reinterpret_cast<__m64*>(equations[y].m_bits.data()));

                size_t limit;
                if(side*side % 64 == 0)
                    limit = side*side / 64 - 1;
                else
                    limit = side*side / 64;

                for(size_t x = yMatrix / 64 ; x <= limit; x++)
                {
                    line[x] = _mm_xor_si64(line[x], pivot[x]);
                }
#endif // OPTIMISATION_MMX

#ifdef OPTIMISATION_SSE
                __m128i* pivot = const_cast<__m128i*>(
                    reinterpret_cast<__m128i*>(equations[yMatrix].m_bits.data()));

                __m128i* line = const_cast<__m128i*>(
                    reinterpret_cast<__m128i*>(equations[y].m_bits.data()));

                size_t limit = (side * side) >> 7;

                if(((yMatrix + side) >> 7) + 1 < limit)
                    limit = ((yMatrix + side) >> 7) + 1;

                for(size_t x = yMatrix >> 7 ; x <= limit; x++)
                {
                    line[x] = _mm_xor_si128(line[x], pivot[x]);
                }
#endif // OPTIMISATION_SSE
            }
        }
    }
    toctic("trianguled matrix");

#if OPTIMISATION_MMX && ! OPTIMISATION_SSE
    _m_empty();
#endif

    // transpose matrix
#ifdef USE_OPENMP
        #pragma omp parallel for
#endif
    for(long i = 0 ; i < side * side - 1 ; i++)
    {
        for(long j = equations[i].find_next(i) ; j != boost::dynamic_bitset<unsigned long long>::npos ;
            j = equations[i].find_next(j))
        {
            equations[i].set(j, equations[j].test(i));
            equations[j].set(i);
        }
    }
    toctic("transposed matrix");


    // 2eme boucle, diagonalise la matrice
    for(size_t xMatrix = side * side -1 ;
        (xMatrix >= 0) && (xMatrix < side * side) ;
        xMatrix--)
    {
        if(result.test(xMatrix) == false)
        {
            if(equations[xMatrix].test(xMatrix) == false)
                nbSolution++;
            continue;
        }

        if(equations[xMatrix].test(xMatrix) == false) // && result == true
        {
            solution.resize(0);
            return -1;
        }

        // soustrait chaque ligne avec les lignes d'au dessus
        for(size_t yMatrix = equations[xMatrix].find_first() ; yMatrix < xMatrix ;
            yMatrix = equations[xMatrix].find_next(yMatrix))
        {
            result.flip(yMatrix);
        }
    }
    toctic("solved");

    solution.resize(side);
    for(size_t y = 0 ; y < side ; y++)
    {
        solution[y].resize(side);

        for(size_t x = 0 ; x < side ; x++)
        {
            solution[y].set(x, result[y * side + x]);
        }
    }
    toctic("copied solution");

    toctic("end");
    return nbSolution;
}

#include "precompiled.h"
#include "board.h"

Board::Board(void)
{
    // valeurs par defaut
    // cote 2, tout blanc
    side = 2;
    density = 0;
	blacks = 0;
	clicks = 0;
}

void Board::count(void)
{
	blacks = 0;

	for( int y = 0 ; y < side ; y++ )
		blacks += board[y].count();
}

void Board::click(long x, long y)
{
    // quand la case x, y est cliquee
    // inverse l'etat des cases qui se trouvent autour
    // s'il y a des cases autours
    
	clicks++;

    flip(x, y);

	if(isSolved())
		solution[y].flip(x);
    
    if(x > 0)
        flip(x - 1, y);
        
    if(x < side - 1 )
        flip(x + 1, y);
        
    if(y > 0)
        flip(x, y - 1);
        
    if(y < side - 1)
        flip(x, y + 1);
}

void Board::generate(long nSide, long nDensity)
{
    // reinitialise les differentes grilles
	beginning.resize(0);
	board.resize(0);
    solution.resize(0);
    
	blacks = 0;
	clicks = 0;
    side = nSide;
    density = nDensity;

    // genere les nouvelles grilles
	board.resize(side, false);
    for(long y = 0 ; y < side ; y++)
    {
		board[y].resize(side);
        
        for(long x = 0 ; x < side ; x++)
        {
            bool b;
            long n;
            
            // donne une couleur en fonction de la densite de case noires voulues
            // les proportions des couleurs sont approximatives
            n = rand();
            if(rand() % 100 < density)
                b = true;
            else
                b = false;
                
			board[y].set(x, b);
        }
    }
    
    // fait une copie de la grille pour pouvoir
    // la rappeller si l'utilisateur veut recommencer
	beginning = board;
	count();
}

//std::vector<std::vector<bool>> Board::getBoard(void)
//{
//    return board;
//}

bool Board::isWon(void)
{
	return blacks == side * side;
}

long Board::solve(void)
{
    // initialise les differentes valeurs
    long nbSolution = 0;
	solution.clear();
    boost::numeric::ublas::vector<boost::dynamic_bitset<unsigned long long> > equations;
	boost::dynamic_bitset<unsigned char> result;
    
	toctic("start");

	// initialise la 2eme matrice
    result.resize(side * side);
	for(long y = 0 ; y < side ; y++)
		for(long x = 0 ; x < side ; x++)
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
	for(long yMatrix = 0; yMatrix < side * side; yMatrix++)
    {
		// cherche une ligne de valeur courante 1 pour s'en servir comme pivot
		if(equations[yMatrix].test(yMatrix) == false)
		{
			for(long pivot = yMatrix + 1 ; pivot < side*side ; pivot++)
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

		//for(long y = 0 ; y < side * side ; y++)
		//{
		//	std::string s;
		//	for(long x = 0 ; x < side * side ; x++)
		//		s.append(1, equations[y][x] == true ? 'X' : '.');
		//	qDebug("%s", s.c_str());
		//}
		//qDebug("");

		// soustrait le pivot aux lignes qui suivent
		long max = yMatrix + side + 1;
		if(side * side - 1 < max)
			max = side * side - 1;
		
		for(long y = yMatrix + 1 ; y <= max ; y++)
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
				
				long limit;
				if(side*side % 64 == 0)
					limit = side*side / 64 - 1;
				else
					limit = side*side / 64;

				for(long x = yMatrix / 64 ; x <= limit; x++)
				{
					line[x] = _mm_xor_si64(line[x], pivot[x]);
				}
#endif // OPTIMISATION_MMX

#ifdef OPTIMISATION_SSE
				__m128i* pivot = const_cast<__m128i*>(
					reinterpret_cast<__m128i*>(equations[yMatrix].m_bits.data()));
				
				__m128i* line = const_cast<__m128i*>(
					reinterpret_cast<__m128i*>(equations[y].m_bits.data()));
				
				long limit = (side * side) >> 7;

				if(((yMatrix + side) >> 7) + 1 < limit)
					limit = ((yMatrix + side) >> 7) + 1;

				for(long x = yMatrix >> 7 ; x <= limit; x++)
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
		for(long j = equations[i].find_next(i) ; j != boost::dynamic_bitset<unsigned long>::npos ;
			j = equations[i].find_next(j))
		{
			equations[i].set(j, equations[j].test(i));
			equations[j].set(i);
		}
	}
	toctic("transposed matrix");


    // 2eme boucle, diagonalise la matrice
    for(long xMatrix = side * side -1 ; xMatrix >= 0 ; xMatrix--)
    {
		if(result.test(xMatrix) == false)
		{
			if(equations[xMatrix].test(xMatrix) == false)
				nbSolution++;
			continue;
		}

		if(equations[xMatrix].test(xMatrix) == false) // && result == true
        {
			return -1;
        }
        
		// soustrait chaque ligne avec les lignes d'au dessus
		for(long yMatrix = equations[xMatrix].find_first() ; yMatrix < xMatrix ;
			yMatrix = equations[xMatrix].find_next(yMatrix))
		{
			result.flip(yMatrix);
		}
	}
	toctic("solved");

	solution.resize(side);
	for(long y = 0 ; y < side ; y++)
    {
		solution[y].resize(side);

		for(long x = 0 ; x < side ; x++)
        {
			solution[y].set(x, result[y * side + x]);
        }
    }
	toctic("copied solution");

	toctic("end");
	return nbSolution;
}

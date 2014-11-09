#ifndef SOLVER_H
#define SOLVER_H

#include "precompiled.h"

#include <vector>
#include <omp.h>

#include "board.h"

class Solver
{
public:
    Solver(void);

private:
    // solution si calculee
    Grid2D solution;

    QElapsedTimer timer;
    void tic(void) { timer.restart(); }
    void toc(std::string s = "")
        { qint64 t = timer.elapsed(); qDebug("Elapsed - %s : %ld", s.c_str(), t); }
    void toctic(std::string s = "") { toc(s); tic(); }

public:
    bool get(size_t x, size_t y) const;
    void reset(void);

    // inverse l'etat d'une case
    void flip(size_t x, size_t y);

    // resoud la grille
    size_t solve(Grid2D board);

    size_t count(void);

    bool isSolved(void) const;
};

#endif // SOLVER_H

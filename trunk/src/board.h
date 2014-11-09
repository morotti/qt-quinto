#ifndef BOARD_H
#define BOARD_H

#include "precompiled.h"

#include <vector>

//**************************************************************************//
// class Board
// represente l'aire de jeu de facon abstraite
// dans un std::vector<std::vector<bool>>
// (un tableau de booleen a deux dimension en gros)
// 
//**************************************************************************//

#define USE_OPENMP 1
#define OPTIMISATION_NONE 1
#define OPTIMISATION_MMX 0
#ifdef X64
#define OPTIMISATION_SSE 0
#endif

typedef boost::numeric::ublas::vector<boost::dynamic_bitset<unsigned long long> > Grid2D;

class Board
{
private:
    // cote de la grille, densite de points noirs au depart
    size_t side, density;
    
    size_t clicks, blacks;

    // grille de jeu
    Grid2D board;
    
    // grille de depart
    Grid2D beginning;
    
    // passe une case dans l'etat value, etat noir par defaut
    void set(size_t x, size_t y, bool value = true);
    
    // passe une case dans l'etat blanc
    void reset(size_t x, size_t y);
    
    // inverse l'etat d'une case
    void flip(size_t x, size_t y);

    void recount(void);

public:
    // constructeur 
    Board(void);
    
    // inverse l'etat de la case et de ses cases voisines
    void click(size_t x, size_t y);
    
    // genere une grille de taille et densite donnee
    void generate(size_t nSide, size_t nDensity);
    
    // repasse la grille dans sa position de depart
    void retry(void) { board = beginning; recount(); }
    
    // verifie si on a gagne
    bool isWon(void);
    
    // renvoie la taille du cote de la grille
    size_t getSide(void) const { return side; }
    
    bool get(const size_t x, const size_t y) const;

    size_t getClickCount(void) { return clicks; }

    size_t getWhiteCount(void) { return side * side - blacks; }

    size_t getBlackCount(void) { return blacks; }

    // renvoie une copie de la grille
    Grid2D getBoard(void) { return board; }
};

#endif // BOARD_H

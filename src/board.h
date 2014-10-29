#ifndef BOARD_H
#define BOARD_H

#include "precompiled.h"

#include <vector>
#include <omp.h>

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

class Board
{
private:
    // cote de la grille, densite de points noirs au depart
    long side, density;
    
	long clicks, blacks;

    // grille de jeu
    boost::numeric::ublas::vector<boost::dynamic_bitset<unsigned long long> > board;
    
    // grille de depart
    boost::numeric::ublas::vector<boost::dynamic_bitset<unsigned long long> > beginning;
    
    // solution si calculee
    boost::numeric::ublas::vector<boost::dynamic_bitset<unsigned long long> > solution;

    // passe une case dans l'etat value, etat noir par defaut
    inline void set(long x, long y, bool value = true);
    
    // passe une case dans l'etat blanc
    inline void reset(long x, long y);
    
    // inverse l'etat d'une case
    inline void flip(long x, long y);

	void count(void);

	QElapsedTimer timer;
	inline void tic(void) { timer.restart(); }
	inline void toc(std::string s = "")
		{ qint64 t = timer.elapsed(); qDebug("Elapsed - %s : %ld", s.c_str(), t); }
	inline void toctic(std::string s = "") { toc(s); tic(); }

public:
    // constructeur 
    Board(void);
    
    // inverse l'etat de la case et de ses cases voisines
    inline virtual void click(long x, long y);
    
    // genere une grille de taille et densite donnee
    virtual void generate(long nSide, long nDensity);
    
    // repasse la grille dans sa position de depart
	virtual void retry(void) { board = beginning; solution.resize(0); count(); }
    
    // verifie si on a gagne
    bool isWon(void);
    
    // renvoie la taille du cote de la grille
    inline long getSide(void) const { return side; }
    
	inline bool get(const long x, const long y) const;	

	inline bool isSolved(void) const;

	inline bool getSolution(const long x, const long y) const;

	inline long getClickCount(void) { return clicks; }

	inline long getWhiteCount(void) { return side * side - blacks; }

	inline long getBlackCount(void) { return blacks; }

    // renvoie une copie de la grille
    //std::vector<std::vector<bool>> getBoard(void);

    // resoud la grille
    virtual long solve(void);
};

void Board::set(long x, long y, bool value)
{
    board[y].set(x, value);
}

void Board::reset(long x, long y)
{
	board[y].reset(x);
}

void Board::flip(long x, long y)
{
	board[y].flip(x);
	
	if(board[y].test(x))
		blacks++;
	else
		blacks--;
}

bool Board::get(const long x, const long y) const
{
	return board[y].test(x);
}

bool Board::isSolved(void) const
{
	if(solution.size() > 0)
		return true;
	else
		return false;
}

bool Board::getSolution(const long x, const long y) const
{
	if(isSolved())
		return solution[y].test(x);
	else
		return false;
}

#endif // BOARD_H

#include "precompiled.h"
#include "board.h"

Board::Board(void)
{
    // valeurs par defaut
    // cote 2, tout blanc
    generate(2, 0);
}

void Board::recount(void)
{
	blacks = 0;

    for( size_t y = 0 ; y < side ; y++ )
		blacks += board[y].count();
}

void Board::click(size_t x, size_t y)
{
    // quand la case x, y est cliquee
    // inverse l'etat des cases qui se trouvent autour
    // s'il y a des cases autours
    
	clicks++;

    flip(x, y);
    
    if(x > 0)
        flip(x - 1, y);
        
    if(x < side - 1 )
        flip(x + 1, y);
        
    if(y > 0)
        flip(x, y - 1);
        
    if(y < side - 1)
        flip(x, y + 1);
}

void Board::generate(size_t nSide, size_t nDensity)
{
    // reinitialise les differentes grilles
	beginning.resize(0);
    board.resize(0);
    
	blacks = 0;
	clicks = 0;
    side = nSide;
    density = nDensity;

    // genere les nouvelles grilles
	board.resize(side, false);
    for(size_t y = 0 ; y < side ; y++)
    {
		board[y].resize(side);
        
        for(size_t x = 0 ; x < side ; x++)
        {
            bool b;
            
            // donne une couleur en fonction de la densite de case noires voulues
            // les proportions des couleurs sont approximatives
            if(static_cast<size_t>(rand() % 100) < density)
                b = true;
            else
                b = false;
                
			board[y].set(x, b);
        }
    }
    
    // fait une copie de la grille pour pouvoir
    // la rappeller si l'utilisateur veut recommencer
	beginning = board;
    recount();
}

bool Board::isWon(void)
{
	return blacks == side * side;
}

void Board::set(size_t x, size_t y, bool value)
{
    board[y].set(x, value);
}

void Board::reset(size_t x, size_t y)
{
    board[y].reset(x);
}

void Board::flip(size_t x, size_t y)
{
    board[y].flip(x);

    if(board[y].test(x))
        blacks++;
    else
        blacks--;
}

bool Board::get(const size_t x, const size_t y) const
{
    return board[y].test(x);
}

/*##########################

Search

#############################*/

#include "config.hpp"
#include "Board.hpp"
#include "MoveGenerator.hpp"
#include "Search.hpp"

//best move variable (will be removed when we start storing principal variation)
int best_move;

//search position for best move
void search_position(int depth)
{
    //search for best move in current position
    //50K is infinity at our scale?
    int score = negamax(-50000, 50000, depth);
    //cout<<score<<"\n";
    //best move placeholder (bestmove is UCI command)
    cout<<"bestmove ";
    print_move(best_move);
    cout<<"\n";
}
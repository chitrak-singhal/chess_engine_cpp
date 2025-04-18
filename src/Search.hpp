/*##########################

Search

#############################*/
#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "config.hpp"
#include "Board.hpp"
#include "Evaluate.hpp"
#include "MoveGenerator.hpp"

//best move variable (will be removed when we start storing principal variation)
extern int best_move;

//negamax function
static inline int negamax(int alpha, int beta, int depth)
{
    //base case of recursion (will modify when adding quiniscience search later on to avoid horizon effect)
    //evaluate automaticcaly returns negative score if black's turn so we dont need to pass whose turn it is
    if (depth==0)
        return evaluate();

    //move list
    MoveList move_list;

    //generate moves
    generate_moves(move_list);

    //legal moves count
    int legal_moves=0;

    //king in check flag
    //check if king is attacked by the other side
    int king_square = ((side==white)? get_fsb(bitboards[K]) : get_fsb(bitboards[k]));
    int king_in_check = is_square_attacked(king_square, (1-side));

    //iterate over all possible moves
    for (int i=0;i<move_list.index;i++)
    {
        //preserve game state
        copy_board();

        //increment half move counter
        ply++;

        //make only legal moves
        if (make_move(move_list.moves[i], all_moves))
        {            
            //increment legal moves count
            legal_moves++;

            //swap alpha and beta and negate them
            //negate childscore
            int childscore = -1*negamax(-1*beta, -1*alpha, depth-1);

            //ply and take_back() were done separately for legal and illegal, because we need ply==0 at root node to distinguish it,
            //which will happen only if we immeditely decrease ply after returning from child and also make move automatically takes back illegal moves

            //decrement half move counter
            ply--;
            //take back the move
            take_back();

            //fail hard
            if (childscore>=beta)
            {
                //node fails high (terminology)
                return beta;
            }

            //if this child score is better then current best (which is alpha) 
            if (childscore>alpha) 
            {
                //principle variation node (terminology)
                if (ply==0) //root node
                {
                    // cout<<"hi "; 
                    //associating best move with best score
                    best_move = move_list.moves[i];
                    // print_move(current_best_move);cout<<" "<<depth<<"\n";
                }
            }
            
            //update alpha
            alpha = max(alpha, childscore);
        }
        else
        {            
            //decrement half move counter
            ply--;
        }            
    }
    
    //check if we have 0 legal moves
    if (legal_moves==0)
    {
        //if in check and no legal moves, checkmate
        if (king_in_check)
            //return mating score
            return -49000+ply; //slightly less than -INF to keep it within alpha beta bounds
            //ply is necessary to distinguish between different mates (M1/M4), more detail in notion doc
    
        //else stalemate
        else 
            //return stalemate score;
            return 0; //stalemate is draw
    }

    //return the best score
    //node fails low (terminology)
    return alpha;
}

//search position for best move
void search_position(int depth);

#endif
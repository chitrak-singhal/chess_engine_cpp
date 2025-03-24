/*##########################

Move generator

#############################*/
#ifndef MOVEGENERATOR_HPP
#define MOVEGENERATOR_HPP

#include "config.hpp"
#include "PAT.hpp"
#include "Board.hpp"

//check if a square is attacked by the side to move
static inline int is_square_attacked(int square, int side)
{
    // pass bishop the occupancies of both pieces (the blockers)
    //if white is to move, we see overlaps with white pieces
    if (get_bishop_attack(square, occupancies[both]) & ((side==white)?bitboards[B]:bitboards[b])) return 1;
    //repeating for other pieces
    if (get_rook_attack(square, occupancies[both]) & ((side==white)?bitboards[R]:bitboards[r])) return 1;
    if (get_queen_attack(square, occupancies[both]) & ((side==white)?bitboards[Q]:bitboards[q])) return 1;
    if (knight_attacks[square] & ((side==white)?bitboards[N]:bitboards[n])) return 1;
    if (king_attacks[square] & ((side==white)?bitboards[K]:bitboards[k])) return 1;
    //we keep pieces of opposite color on the square to test so we use !side
    if (pawn_attacks[(!side)][square] & ((side==white)?bitboards[P]:bitboards[p])) return 1;
    return 0;
}

//print function for testing
void print_attacked_squares(int side);

#endif 
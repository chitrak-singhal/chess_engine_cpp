/*##########################

Evaluate

#############################*/
#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include "config.hpp"
#include "Board.hpp"

//material scores of white PKBRQK of white and black respectively
// black scores are negative since we are calculating score from white pov, and we will use negamax, so black's score is simply negative of this score
constexpr int material_score[12] = {
    100, 300, 350, 500, 1000, 10000, -100, -300, -350, -500, -1000,-10000
};

// pawn positional score
constexpr int pawn_score[64] = 
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
constexpr int knight_score[64] = 
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
constexpr int bishop_score[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
constexpr int rook_score[64] =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
constexpr int king_score[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,   5,   0, -15,   0,  10,   0
};

// mirror square for opposite side
constexpr int mirror_square[64] =
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};


//evaluate current position
static inline int evaluate()
{
    //evaluation score
    int score = 0;

    //bitbaord copy since we cannot change original while counting
    U64 bitboard;

    //loop over pieces 
    for (int piece= P;piece<=k;piece++)
    {
        //copy the bitboard
        bitboard = bitboards[piece];

        //count number of pieces
        while(bitboard)
        {
            //add piece material value to score
            score+=material_score[piece];
            int square = get_fsb(bitboard);

            switch(piece)
            {
                //evaluate white pieces
                case P: {score += pawn_score[square]; break;}
                case N: {score += knight_score[square]; break;}
                case B: {score += bishop_score[square]; break;}
                case R: {score += rook_score[square]; break;}
                case K: {score += king_score[square]; break;}

                //evaluate black pieces, notice the minus sign in score
                case p: {score -= pawn_score[mirror_square[square]]; break;}
                case n: {score -= knight_score[mirror_square[square]]; break;}
                case b: {score -= bishop_score[mirror_square[square]]; break;}
                case r: {score -= rook_score[mirror_square[square]]; break;}
                case k: {score -= king_score[mirror_square[square]]; break;}
            }
            
            //reset fsb, so we can count next set bit
            reset_bit(bitboard, square);
        }
    }
    
    //return the score if white, -score if black (negamax is being used)
    if (side==white)
        return score;
    else 
        return -1*score;
}

#endif
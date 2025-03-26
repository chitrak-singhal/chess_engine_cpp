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

/*
          binary move bits                               hexidecimal constants
    
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000  0 means no promotion
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

//encode move
#define encode_move(source,target, piece,promo_piece, capture, double, enpassant, castling) \
((source)|(target<<6)|(piece<<12)|(promo_piece<<16)|(capture<<20)|(double<<21)|(enpassant<<22)|(castling<<23))

//decode move
#define decode_move_source(move) (move & 0x3f)
#define decode_move_target(move) ((move & 0xfc0) >> 6)
#define decode_move_piece(move) ((move & 0xf000) >> 12)
#define decode_move_promo_piece(move) ((move & 0xf0000) >> 16)
#define decode_move_capture(move) (move & 0x100000)
#define decode_move_double(move) (move & 0x200000)
#define decode_move_enpassant(move) (move & 0x400000)
#define decode_move_castling(move) (move & 0x800000)

//convert promoted piece to character
extern char promoted_pieces[128];

//move list
class MoveList 
{
    //store the moves, 256 is sufficient to store moves of one cycle 
    int moves[256];
    //index of the list
    int index;

    public:
    //constructor
    MoveList()
    {
        index = 0;
    }
    
    //add move function
    void add_move(int move)
    {
        //store move
        moves[index] = move;
        //increment index
        index++;
    }

    //print move function according to UCI protocol format
    void print_move(int move)
    {
        //if no prmotion (0 value) promoted_pieces gives null char, so nothing printed
        cout<<square_to_board[decode_move_source(move)]<<square_to_board[decode_move_target(move)]<<promoted_pieces[decode_move_promo_piece(move)]<<"\n";
    }

    //print move list
    void print_move_list()
    {
        cout<<"\n#### MOVE LIST ####\n";
        cout<<"move,  piece, capture, double, enpassant, castling\n";
        //loop over all moves
        for (int i=0;i<index;i++)
        {
            cout<<square_to_board[decode_move_source(moves[i])]<<square_to_board[decode_move_target(moves[i])]<<promoted_pieces[decode_move_promo_piece(moves[i])];
            //print unicode only if windows
            #ifdef _WIN32
                cout<<"    "<<unicode_pieces[decode_move_piece(moves[i])]<<"    ";
            #else 
                cout<<"  "<<ascii_pieces[piece]<<"  ";
            #endif
            cout<<"    "<<bool(decode_move_capture(moves[i]))<<"    ";
            cout<<"    "<<bool(decode_move_double(moves[i]))<<"    ";
            cout<<"    "<<bool(decode_move_enpassant(moves[i]))<<"    ";
            cout<<"    "<<bool(decode_move_castling(moves[i]))<<"    \n";
        }
        cout<<"Total number of moves: "<<index<<"\n\n";
    }
};
//move list object
extern MoveList move_list;

//function to generate all legal moves
static inline void generate_moves()
{
    //source & target square
    int source_square, target_square;

    //copy the current piece's bitboard and it's attacks
    //we will extract source squares from bitboard, and target squares from attacks using FSB
    //we will ensure we dont capture our own piece
    U64 bitboard, attacks;

    //loop over all pieces to genereate moves for them
    for (int piece = P; piece<=k;piece++)
    {
        //copy the piece bitboard
        bitboard = bitboards[piece];

        //generate white pawn and white king castling moves
        //special moves like double push, enpassant etc.
        if (side==white)
        {
            if (piece==P)
            {
                //loop over the bitboard continuously getting the FSB
                while(bitboard)
                {
                    //get the square of the pawn
                    source_square = get_fsb(bitboard);

                    //######generate quiet pawn moves#####
                    //one step ahead target
                    target_square = source_square - 8;
                    
                    // ensure target_square is valid, and it is empty
                    if ((a8<=target_square&&target_square<=h1)&& !get_bit(occupancies[both], target_square))
                    {
                        //promotions, check if pawn is on second last rank
                        if (a7<=source_square&&source_square<=h7)
                        {
                            move_list.add_move(encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                        }
                        else
                        {
                            //move one square ahead
                            move_list.add_move(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                            //move two square ahead if on second rank, and the square 2 steps ahead is empty
                            if ((a2<=source_square&&source_square<=h2)&& !get_bit(occupancies[both], (source_square-16)))
                            {
                                move_list.add_move(encode_move(source_square, (source_square-16), piece, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                    
                    //######generate capture moves#####
                    // can capture only black so take AND with its occupancy
                    attacks = (pawn_attacks[white][source_square] & occupancies[black]);

                    //iterate over the possible targets using FSB
                    while(attacks)
                    {
                        //get the target
                        target_square = get_fsb(attacks);

                        //promotion+capture, check if pawn is on second last rank
                        if (a7<=source_square&&source_square<=h7)
                        {
                            //cout<<"promo capture"<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n"; 
                            move_list.add_move(encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));
                        }
                        else
                        {
                            //normal capture
                            move_list.add_move(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                            //cout<<"capture"<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n"; 
                        }

                        //reset the target so we can move on to the next
                        reset_bit(attacks, target_square);
                    }

                    //######generate enpassant moves#####
                    if (enpassant!=no_square) //enpassant exists
                    {
                        //do AND of enpassant square with pawn attacks 
                        U64 enpassant_attack = (pawn_attacks[white][source_square] & (1ULL<<enpassant));
                        if (enpassant_attack) //if there is indeed enpassant attack
                        {
                            //get the target
                            int target_enpassant = get_fsb(enpassant_attack);
                            //cout<<"enpassant capture"<<square_to_board[source_square]<<" "<<square_to_board[enpassant]<<"\n"; 
                            move_list.add_move(encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    //reset this square so we can move to the next
                    reset_bit(bitboard, source_square);
                }
            }

            //Castling moves
            if (piece==K)
            {
                //king side castling possible 
                if (castle & wk)
                {
                    //nothing should be there between king and rook
                    if (!get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1))
                    {
                        //king and f1 g1 square should not be under attack from black
                        //alternatively g1 square (final square of the move) will can be checked for legality in the make move function and not here
                        if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black)&& !is_square_attacked(g1, black))
                        {
                            //cout<<"White king side possible\n";
                            move_list.add_move(encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
                //queen side castling available
                if (castle & wq)
                {
                    //nothing should be there between king and rook
                    if (!get_bit(occupancies[both], b1) && !get_bit(occupancies[both], c1)&& !get_bit(occupancies[both], d1))
                    {
                        //king and c1 d1 square should not be under attack from black
                        if (!is_square_attacked(e1, black) && !is_square_attacked(c1, black) && !is_square_attacked(d1, black))
                        {
                            //cout<<"White queen side possible\n";
                            move_list.add_move(encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }
        } 
        //black pawn and black king castling moves
        else
        {
            if (piece==p)
            {
                //loop over the bitboard continuously getting the FSB
                while(bitboard)
                {
                    //get the square of the pawn
                    source_square = get_fsb(bitboard);
                    

                    //######generate quiet pawn moves#####
                    //one step below since this a black pawn target
                    target_square = source_square + 8;
                    
                    // ensure target_square is valid, and it is empty
                    if ((a8<=target_square&&target_square<=h1)&& !get_bit(occupancies[both], target_square))
                    {
                        //promotions, check if pawn is on second rank
                        if (a2<=source_square&&source_square<=h2)
                        {
                            move_list.add_move(encode_move(source_square, target_square, piece, q, 0, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, r, 0, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, b, 0, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, n, 0, 0, 0, 0));
                        }
                        else
                        {
                            //move one square below
                            move_list.add_move(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                            //move two square below if on second last rank, and the square 2 steps ahead is empty
                            if ((a7<=source_square&&source_square<=h7)&& !get_bit(occupancies[both], (source_square+16)))
                            {
                                move_list.add_move(encode_move(source_square, (source_square+16), piece, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                    
                    //######generate capture moves#####
                    // can capture only white so take AND with its occupancy
                    attacks = (pawn_attacks[black][source_square] & occupancies[white]);

                    //iterate over the possible targets using FSB
                    while(attacks)
                    {
                        //get the target
                        target_square = get_fsb(attacks);

                        //promotion+capture, check if pawn is on second rank
                        if (a2<=source_square&&source_square<=h2)
                        {
                            //cout<<"promo capture"<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n"; 
                            move_list.add_move(encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                            move_list.add_move(encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));
                        }   
                        else
                        {
                            //normal capture
                            move_list.add_move(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                            //cout<<"capture"<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n"; 
                        }

                        //reset the target so we can move on to the next
                        reset_bit(attacks, target_square);
                    }

                    //######generate enpassant moves#####
                    if (enpassant!=no_square) //enpassant exists
                    {
                        //do AND of enpassant square with pawn attacks 
                        U64 enpassant_attack = (pawn_attacks[black][source_square] & (1ULL<<enpassant));
                        if (enpassant_attack) //if there is indeed enpassant attack
                        {
                            //get the target
                            int target_enpassant = get_fsb(enpassant_attack);
                            //cout<<"enpassant capture"<<square_to_board[source_square]<<" "<<square_to_board[enpassant]<<"\n"; 
                            move_list.add_move(encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    //reset this square so we can move to the next
                    reset_bit(bitboard, source_square);
                }
            }
            //Castling moves
            if (piece==k)
            {
                //king side castling possible 
                if (castle & bk)
                {
                    //nothing should be there between king and rook
                    if (!get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8))
                    {
                        //king and f8 g8 square should not be under attack from white
                        if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white)&& !is_square_attacked(g8,white))
                        {
                            //cout<<"Black king side possible\n";
                            move_list.add_move(encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
                //queen side castling available
                if (castle & bq)
                {
                    //nothing should be there between king and rook
                    if (!get_bit(occupancies[both], b8) && !get_bit(occupancies[both], c8)&& !get_bit(occupancies[both], d8))
                    {
                        //king and c8 d8 square should not be under attack from white
                        if (!is_square_attacked(e8, white) && !is_square_attacked(c8, white) && !is_square_attacked(d8, white))
                        {
                            //cout<<"Black queen side possible\n";
                            move_list.add_move(encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }
        }
        //generate knight moves
        if ((side==white&&piece==N)||(side==black&&piece==n))
        {
            //loop over all knights
            while(bitboard)
            {
                source_square = get_fsb(bitboard);
                //we can move anywhere where there is not a piece of our own color
                attacks = (knight_attacks[source_square] & (~occupancies[side]));
                // loop over all possible attacks
                while(attacks)
                {
                    target_square = get_fsb(attacks);
                    //quiet move if square is unoccupied
                    if (!get_bit(occupancies[both], target_square))
                    //cout<<side<<" Knight quiet "<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //capture move if square is occupied
                    else 
                    //cout<<side<<" Knight capture "<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    reset_bit(attacks, target_square);
                }
                reset_bit(bitboard, source_square);
            }
        }
        //generate bishop moves
        if ((side==white&&piece==B)||(side==black&&piece==b))
        {
            //loop over all bishops
            while(bitboard)
            {
                source_square = get_fsb(bitboard);
                //we can move anywhere where there is not a piece of our own color
                attacks = (get_bishop_attack(source_square, occupancies[both]) & (~occupancies[side]));
                // loop over all possible attacks
                while(attacks)
                {
                    target_square = get_fsb(attacks);
                    //quiet move if square is unoccupied
                    if (!get_bit(occupancies[both], target_square))
                    //cout<<side<<" Bishop quiet "<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //capture move if square is occupied
                    else 
                    //cout<<side<<" Bishop capture "<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    reset_bit(attacks, target_square);
                }
                reset_bit(bitboard, source_square);
            }
        }
        //generate rook moves
        if ((side==white&&piece==R)||(side==black&&piece==r))
        {
            //loop over all rooks
            while(bitboard)
            {
                source_square = get_fsb(bitboard);
                //we can move anywhere where there is not a piece of our own color
                attacks = (get_rook_attack(source_square, occupancies[both]) & (~occupancies[side]));
                // loop over all possible attacks
                while(attacks)
                {
                    target_square = get_fsb(attacks);
                    //quiet move if square is unoccupied
                    if (!get_bit(occupancies[both], target_square))
                    //cout<<side<<" rook quiet "<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //capture move if square is occupied
                    else 
                    //cout<<side<<" rook capture"<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    reset_bit(attacks, target_square);
                }
                reset_bit(bitboard, source_square);
            }
        }
        //generate queen moves
        if ((side==white&&piece==Q)||(side==black&&piece==q))
        {
            //loop over all queens
            while(bitboard)
            {
                source_square = get_fsb(bitboard);
                //we can move anywhere where there is not a piece of our own color
                attacks = (get_queen_attack(source_square, occupancies[both]) & (~occupancies[side]));
                // loop over all possible attacks
                while(attacks)
                {
                    target_square = get_fsb(attacks);
                    //quiet move if square is unoccupied
                    if (!get_bit(occupancies[both], target_square))
                    //cout<<side<<" queen quiet "<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //capture move if square is occupied
                    else 
                    //cout<<side<<"queen capture"<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    reset_bit(attacks, target_square);
                }
                reset_bit(bitboard, source_square);
            }
        }
        //generate king moves
        if ((side==white&&piece==K)||(side==black&&piece==k))
        {
            //loop over king
            while(bitboard)
            {
                source_square = get_fsb(bitboard);
                //we can move anywhere where there is not a piece of our own color
                attacks = (king_attacks[source_square] & (~occupancies[side]));
                // loop over all possible attacks
                while(attacks)
                {
                    target_square = get_fsb(attacks);
                    //quiet move if square is unoccupied
                    if (!get_bit(occupancies[both], target_square))
                    //cout<<side<<" King quiet "<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //capture move if square is occupied
                    else 
                    //cout<<side<<" king capture "<<square_to_board[source_square]<<" "<<square_to_board[target_square]<<"\n";
                        move_list.add_move(encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    reset_bit(attacks, target_square);
                }
                reset_bit(bitboard, source_square);
            }
        }
    }
}

// preserve board state
//sizeof is slower so we use constant value as we already know it
#define copy_board()\
    U64 bitboards_copy[12], occupancies_copy[3];\
    memcpy(bitboards_copy, bitboards, 96);\
    memcpy(occupancies_copy, occupancies, 24);\
    int side_copy = side, enpassant_copy = enpassant, castle_copy = castle, fifty_copy = fifty;

// restore board state
#define take_back()                                                       \
    memcpy(bitboards, bitboards_copy, 96);                                \
    memcpy(occupancies, occupancies_copy, 24);                            \
    side = side_copy;enpassant = enpassant_copy;castle = castle_copy;fifty = fifty_copy;                                                \


#endif 
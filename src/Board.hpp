#ifndef BOARD_HPP
#define BOARD_HPP
/*##########################

Define the board

#############################*/
#include "config.hpp"
//6 pieces and 2 colors so 12
extern U64 bitboards[12]; 

//occupancy (where pieces are) bitboards
//white , black ,and combined
extern U64 occupancies[3];

//whose turn is it
extern int side ;

//en-passant square 
extern int enpassant;//wont alsways have enpassant so no_square initalise

//castling rights
/* castling code
0001 (1) white king, king side
0010 (2) white king, queen side
0100 (4) black king, king side
1000 (8) black king, queen side
now any combination can be represented
ex: 1111 both kings both sides
0011 white king both sides
these are enum'ed in config 
*/
extern int castle;

//fifty move rule counter
extern int fifty;

//we need to convert chars 'P' to constant P , to be used by enum to convert to number index
// 'P' will become index 80 in this int array, and will store 80 value
// [index] allows us to place at a particular index
extern int char_pieces[128];

//print the board
void print_board();

//FEN debug strings
#define empty_board "8/8/8/8/8/8/8/8 b - - 0 0 "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define repetitions "2r3k1/R7/8/1R6/8/8/P4KPP/8 w - - 0 40 "

//parse FEN function
void parse_FEN_string(const string &fen);

#endif
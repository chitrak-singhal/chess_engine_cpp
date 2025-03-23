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

//we need to convert chars 'P' to constant P , to be used by enum to convert to number index
// 'P' will become index 80 in this int array, and will store 80 value
// [index] allows us to place at a particular index
extern int char_pieces[128];

//print the board
void print_board();
#endif
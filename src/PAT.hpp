#ifndef PAT_HPP
#define PAT_HPP

#include "config.hpp"

/*##########################

Precalculated Attack Tables (PAT)

#############################*/

//PAT format: table[side][square]
//PAT for various pieces are as follows:
extern U64 pawn_attacks[2][64];  
extern U64 knight_attacks[64];
extern U64 king_attacks[64];
extern U64 bishop_attacks[64][512];
extern U64 rook_attacks[64][4096];
extern U64 bishop_romasks[64]; //relevant occupancy masks
extern U64 rook_romasks[64];

//function to generate pawn attacks
U64 mask_pawn_attacks(int side, int square);

//function to generate knight attacks
U64 mask_knight_attacks(int square);

//function to generate king attacks
U64 mask_king_attacks(int square);

//function to generate bishop relevant occupancy bits
U64 generate_relevant_occupancy_mask_bishop(int square);

//function to generate bishop attack sets, given blocker
//we will go till edge this time as we can attack the blocker on edge as well, and here we are generating attacks, not relevant occupancy bits
U64 generate_bishop_attack_sets(int square, U64 blocker);

//function to generate rook relevant occupany bits
U64 generate_relevant_occupancy_mask_rook(int square);

//function to generate rook attack sets, given blocker
//we will go till edge this time as we can attack the blocker on edge as well, and here we are generating attacks, not relevant occupancy bits
U64 generate_rook_attack_sets(int square, U64 blocker);

//function to generate blocker bitboard given blocker bitboard index and relevant occupancy mask
U64 generate_blocker_bitboard(int blocker_bitboard_index,int num_of_relevant_occu, U64 relevant_occupancy_mask);

//functions to generate attacks for all position for all pieces (PATS)
//for leaper pieces: (leaper attacks are not affected by blocking pieces, hence leaper and sliding pieces tables have to be generated separately)
void init_leapers_attacks();

void init_sliding_attacks();

//lookup bishop attack
static inline U64 get_bishop_attack(int square, U64 bitboard)
{
    //filter relevant occupanices from the board
    bitboard &= bishop_romasks[square];
    //reusing bitboard variable for magic index variable to aoid declaring new variable
    bitboard = ((bitboard*bishop_magic_numbers[square])>>(64-relevant_bit_count_bishop[square]));
    //look up the table
    return bishop_attacks[square][bitboard];
}

//lookup rook attack
static inline U64 get_rook_attack(int square, U64 bitboard)
{
    //filter relevant occupanices from the board
    bitboard&=rook_romasks[square];
    //reusing bitboard variable for magic index variable to aoid declaring new variable
    bitboard = ((bitboard*rook_magic_numbers[square])>>(64-relevant_bit_count_rook[square]));
    return rook_attacks[square][bitboard];
}

//lookup queen attack
static inline U64 get_queen_attack(int square, U64 bitboard)
{
    //create a copy of the bitboard for rook and bishop
    U64 bishop_bitboard = bitboard;
    U64 rook_bitboard = bitboard;
    //generate their attacks
    bishop_bitboard &= bishop_romasks[square];
    //reusing bishop_bitboard for magic index variable to aoid declaring new variable
    bishop_bitboard = ((bishop_bitboard*bishop_magic_numbers[square])>>(64-relevant_bit_count_bishop[square]));
    
    rook_bitboard&=rook_romasks[square];
    rook_bitboard = ((rook_bitboard*rook_magic_numbers[square])>>(64 - relevant_bit_count_rook[square]));

    //either rook or bishop attacks are queen attacks
    return (bishop_attacks[square][bishop_bitboard]|rook_attacks[square][rook_bitboard]);
}

#endif
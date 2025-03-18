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

//function to generate pawn attacks
U64 mask_pawn_attacks(int side, int square);

//function to generate knight attacks
U64 mask_knight_attacks(int square);

//function to generate king attacks
U64 mask_king_attacks(int square);

//function to generate bishop relevant occupancy bits
U64 mask_bishop_attacks(int square);

//function to generate bishop attack sets, given blocker
//we will go till edge this time as we can attack the blocker on edge as well, and here we are generating attacks, not relevant occupancy bits
U64 generate_bishop_attack_sets(int square, U64 blocker);

//function to generate rook relevant occupany bits
U64 mask_rook_attacks(int square);

//function to generate rook attack sets, given blocker
//we will go till edge this time as we can attack the blocker on edge as well, and here we are generating attacks, not relevant occupancy bits
U64 generate_rook_attack_sets(int square, U64 blocker);

//functions to generate attacks for all position for all pieces (PATS)
//for leaper pieces: (leaper attacks are not affected by blocking pieces, hence leaper and sliding pieces tables have to be generated separately)
void init_leapers_attacks();

#endif
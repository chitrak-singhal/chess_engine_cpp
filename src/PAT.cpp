#include "config.hpp"

/*##########################

Precalculated Attack Tables (PAT)

#############################*/

//PAT format: table[side][square]
//PAT for various pieces are as follows:
U64 pawn_attacks[2][64];
U64 knight_attacks[64]; //knights have side independent movement
U64 king_attacks[64];

//function to generate pawn attacks
U64 mask_pawn_attacks(int side, int square)
{
    //create piece bitboard
    U64 bitboard = 0ULL;
    // bitboard to store results
    U64 attacks = 0ULL;
    //place piece on the board
    set_bit(bitboard, square);
    if (side==0) //white to move
    {
        //we need to be careful about out of bounds, 
        //we will use ~FILE_A (not file A/H), so that we can exclude pawns on these files while considering side ways capture
        attacks|= ((bitboard>>7)&(~A_FILE)); //right side capture cannot come on a file
        attacks|= ((bitboard>>9)&(~H_FILE));
    }
    else
    {
        attacks|= ((bitboard<<7)&(~H_FILE)); 
        attacks|= ((bitboard<<9)&(~A_FILE));
    }
    return attacks;
}

//function to generate knight attacks
U64 mask_knight_attacks(int square)
{
    U64 bitboard = 0ULL;
    U64 attacks = 0ULL;
    set_bit(bitboard, square);
    //knights are side independent
    attacks = ((bitboard>>6)&~AB_FILE); //right side 2 steps cannot come on AB, can come on C to H
    attacks |= ((bitboard>>10)&~GH_FILE);
    attacks |= ((bitboard>>15)&~A_FILE);
    attacks |= ((bitboard>>17)&~H_FILE);
    attacks |= ((bitboard<<6)&(~GH_FILE));
    attacks |= ((bitboard<<10)&~AB_FILE);
    attacks |= ((bitboard<<15)&~H_FILE);
    attacks |= ((bitboard<<17)&~A_FILE);
    return attacks;
}

//function to generate king attacks
U64 mask_king_attacks(int square)
{
    U64 bitboard = 0ULL;
    U64 attacks = 0ULL;
    set_bit(bitboard, square);
    attacks = ((bitboard>>1)&~H_FILE); 
    attacks |= ((bitboard>>7)&~A_FILE);
    attacks |= ((bitboard>>8));
    attacks |= ((bitboard>>9)&~H_FILE);
    attacks |= ((bitboard<<1)&(~A_FILE));
    attacks |= ((bitboard<<7)&~H_FILE);
    attacks |= ((bitboard<<8));
    attacks |= ((bitboard<<9)&~A_FILE);
    return attacks;
}

//function to generate bishop relevant occupancy mask
U64 generate_relevant_occupancy_mask_bishop(int square)
{
    U64 attacks = 0ULL; //more accurately the relevant occupancy squares 
    // rank and file variables
    int rank,file; //board rank = 8 - rank
    //target rank and file variables
    int tar_rank = square/8; int tar_file = square %8;
    //mask relevant bishop occupancy bits
    //edges are not taken while masking relevant occupany bits, so we dont need to go to rank 7 file 7 or rank 0 file 0
    for (rank = tar_rank+1, file = tar_file+1; rank<=6&&file<=6;file++,rank++) 
        attacks|= (1ULL<<(rank*8+file)); //board rank = 8 - rank variable, so this for loop creates the bottom right diagonal
    for (rank = tar_rank+1, file = tar_file-1; rank<=6&&file>=1;file--,rank++) 
        attacks|= (1ULL<<(rank*8+file)); //bottom left diagonal
    for (rank = tar_rank-1, file = tar_file+1; rank>=1&&file<=6;file++,rank--) 
        attacks|= (1ULL<<(rank*8+file)); //top right diagonal
    for (rank = tar_rank-1, file = tar_file-1; rank>=1&&file>=1;file--,rank--) 
        attacks|= (1ULL<<(rank*8+file)); //top left diagonal
    return attacks;
}

//function to generate bishop attack sets, given blocker
//we will go till edge this time as we can attack the blocker on edge as well, and here we are generating attacks, not relevant occupancy bits
U64 generate_bishop_attack_sets(int square, U64 blocker)
{
    U64 attacks = 0ULL; //distinct attack sets
    // rank and file variables
    int rank,file; //board rank = 8 - rank
    //target rank and file variables
    int tar_rank = square/8; int tar_file = square %8;
    for (rank = tar_rank+1, file = tar_file+1; rank<=7&&file<=7;file++,rank++) 
    {
        attacks|= (1ULL<<(rank*8+file));
        if (get_bit(blocker, rank*8+file)) break; // if there was a blocker at this square, the piece cannot attack further
    } //board rank = 8 - rank variable, so this for loop creates the bottom right diagonal
    for (rank = tar_rank+1, file = tar_file-1; rank<=7&&file>=0;file--,rank++) 
    {
        attacks|= (1ULL<<(rank*8+file));
        if (get_bit(blocker, rank*8+file)) break;
    } //bottom left diagonal
    for (rank = tar_rank-1, file = tar_file+1; rank>=0&&file<=7;file++,rank--) 
    {
        attacks|= (1ULL<<(rank*8+file));
        if (get_bit(blocker, rank*8+file)) break;
    } //top right diagonal
    for (rank = tar_rank-1, file = tar_file-1; rank>=0&&file>=0;file--,rank--) 
    {
        attacks|= (1ULL<<(rank*8+file));
        if (get_bit(blocker, rank*8+file)) break;
    } //top left diagonal
    return attacks;
}

//function to generate rook relevant occupany bits
U64 generate_relevant_occupancy_mask_rook(int square)
{
    U64 attacks = 0ULL; //more accurately the relevant occupancy squares 
    // rank and file variables
    int rank,file; //board rank = 8 - rank
    //target rank and file variables
    int tar_rank = square/8; int tar_file = square %8;
    //mask relevant rook occupancy bits
    //edges are not taken while masking relevant occupany bits, so we dont need to go to rank 7 file 7 or rank 0 file 0
    for (rank = tar_rank+1, file = tar_file; rank<=6;rank++) 
        attacks|= (1ULL<<(rank*8+file)); //board rank = 8 - rank variable, so this for loop creates the down part
    for (rank = tar_rank-1, file = tar_file; rank>=1;rank--) 
        attacks|= (1ULL<<(rank*8+file)); //top part
    for (rank = tar_rank, file = tar_file+1; file<=6;file++) 
        attacks|= (1ULL<<(rank*8+file)); //right part
    for (rank = tar_rank, file = tar_file-1; file>=1;file--) 
        attacks|= (1ULL<<(rank*8+file)); //left part
    return attacks;
}

//function to generate rook attack sets, given blocker bitboard
//we will go till edge this time as we can attack the blocker on edge as well, and here we are generating attacks, not relevant occupancy bits
U64 generate_rook_attack_sets(int square, U64 blocker)
{
    U64 attacks = 0ULL; //distinct attack sets
    // rank and file variables
    int rank,file; //board rank = 8 - rank
    //target rank and file variables
    int tar_rank = square/8; int tar_file = square %8;
    for (rank = tar_rank+1, file = tar_file; rank<=7;rank++) 
    {
        attacks|= (1ULL<<(rank*8+file));
        if (get_bit(blocker, rank*8+file)) break;
    } //board rank = 8 - rank variable, so this for loop creates the down part
    for (rank = tar_rank-1, file = tar_file; rank>=0;rank--) 
    {
        attacks|= (1ULL<<(rank*8+file));
        if (get_bit(blocker, rank*8+file)) break;
    } //top part
    for (rank = tar_rank, file = tar_file+1; file<=7;file++) 
    {
        attacks|= (1ULL<<(rank*8+file));
        if (get_bit(blocker, rank*8+file)) break;
    } //right part
    for (rank = tar_rank, file = tar_file-1; file>=0;file--) 
    {
        attacks|= (1ULL<<(rank*8+file));
        if (get_bit(blocker, rank*8+file)) break;
    }
     //left part
    return attacks;
}

//function to generate blocker bitboard given blocker bitboard index and relevant occupancy mask
U64 generate_blocker_bitboard(int blocker_bitboard_index, U64 relevant_occupancy_mask)
{
    U64 blocker = 0ULL; //blocker bitboard
    int num_of_relevant_occu = bit_count(relevant_occupancy_mask); //number of relevant occupancies in the mask
    for (int relevant_occu=0; relevant_occu<num_of_relevant_occu; relevant_occu++) //iterate over the relevant occupancies
    {
        int square = get_fsb(relevant_occupancy_mask); //get the relevant occupancy square
        reset_bit(relevant_occupancy_mask, square); //reset it, so that we can get the next relevant occupancy square
        if ((blocker_bitboard_index>>relevant_occu)&1) //check if there is a blocker at that relevant occupancy square
            set_bit(blocker, square);
    }
    return blocker;
}

//functions to generate attacks for all position for all pieces (PATS)
//for leaper pieces: (leaper attacks are not affected by blocking pieces, hence leaper and sliding pieces tables have to be generated separately)
void init_leapers_attacks()
{
    for (int square = 0; square<64; square++)
    {
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square]=mask_king_attacks(square);
    }
}

//random number generator
unsigned int state = 1804289383;
unsigned int random_generator_U32()
{
    unsigned int num = state;
    //xorshift32 algorithm
    num^= (num<<13);
    num^= (num>>17); 
    num^= (num<<5);

    state = num; //update state

    return num;
}
U64 random_generator_U64() //this random number will be used to generate the magic number candidates
{
    //define 4 random numbers low non-zero bits numbers
    U64 num1 = ((U64)random_generator_U32() & 0xFFFF); //& with 0xFFFF keeps only first 16 bits
    U64 num2 = ((U64)random_generator_U32() & 0xFFFF);
    U64 num3 = ((U64)random_generator_U32() & 0xFFFF);
    U64 num4 = ((U64)random_generator_U32() & 0xFFFF);

    return ((num1)| (num2<<16)|(num3<<32)|(num4<<48));
}

//generate magic number candidate
U64 generate_magic_number_candidate()
{
    return (random_generator_U64()&random_generator_U64()&random_generator_U64());
}
/*##########################

Including system files

#############################*/

#include<iostream>
using namespace std;

//define bitboard datatype
#define U64 unsigned long long

//enum board square
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3, 
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
};

//enum sides to move
enum {white, black}; //0 means white

//preserved for future use

// "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
// "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
// "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
// "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
// "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
// "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
// "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
// "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",

/*##########################

Bit manipulation functions and macros

#############################*/

#define get_bit(bitboard, square) ((bitboard>>square)&1ULL)
#define set_bit(bitboard, square) (bitboard|=(1ULL<<square))
//if we reset using xor, calling reset multiple times can set the bit accidently, so we need an if condition along with it
#define reset_bit(bitboard, square) {if (get_bit(bitboard, square)) {bitboard^= (1ULL<<square);}}

//print bitboard function
void print_bitboard(U64 bitboard)
{
    for (int rank = 0; rank<8; rank++)
    {
        cout<<" "<<(8 - rank)<<"  ";
        for (int file = 0; file<8; file++)
        {
            // file 8 is top most, chess board like normal 
            // counting from top left
            int square = 8*rank + file;
            cout<<get_bit(bitboard, square)<<" ";
        }
        cout<<"\n";
    }
    cout<<"    ";
    for (char file = 'a'; file<='h'; file++)
        cout<<file<<" ";
    cout<<"\n\n";
    //print bitboard as unsigned decimal number
    cout<<"Bitboard: "<<bitboard<<"\n";
}
/*##########################

Precalculated Attack Tables (PAT)

#############################*/

//file constants
const U64 A_FILE = 0x0101010101010101;
const U64 H_FILE = 0x8080808080808080;
const U64 B_FILE = 0x0202020202020202;
const U64 G_FILE = 0x4040404040404040;
const U64 GH_FILE = H_FILE|G_FILE;
const U64 AB_FILE = A_FILE|B_FILE;

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

/*##########################

Main driver

#############################*/

int main(int argc, char const *argv[])
{
    cout<<"Welcome to Domino, never lose again!\n";
    init_leapers_attacks();
    for (int i=0;i<64;i++)
    {
        print_bitboard(king_attacks[i]);
    }
    return 0;
}

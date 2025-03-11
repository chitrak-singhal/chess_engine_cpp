/*##########################

Including system files

#############################*/

#include<bits/stdc++.h>
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

Main driver

#############################*/

int main(int argc, char const *argv[])
{
    cout<<"Welcome to Domino, never lose again!\n";
    U64 bitboard = 0ULL;
    //setting a square to test enum
    set_bit(bitboard, e4);
    set_bit(bitboard, f2);
    print_bitboard(bitboard);
    reset_bit(bitboard, e4);
    reset_bit(bitboard, e4);
    print_bitboard(bitboard);
    return 0;
}

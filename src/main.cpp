#include "config.hpp" //include all the basic stuff

//print bitboard function
void print_bitboard(U64 bitboard)
{
    for (int rank = 0; rank<8; rank++)
    {
        cout<<" "<<(8 - rank)<<"  ";
        for (int file = 0; file<8; file++)
        {
            // rank 8 is top most, chess board like normal 
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
    //cout<<"Bitboard: "<<bitboard<<"\n";
}

/*##########################

Precalculated Attack Tables (PAT)

#############################*/
#include "PAT.hpp"

/*##########################

Define the board

#############################*/

//6 pieces and 2 colors so 12
U64 bitboards[12]; 

//occupancy (where pieces are) bitboards
//white , black ,and combined
U64 occupancies[3];

//whose turn is it
//initialised to -1 here so that we cannnot start without initializing it properly
int side = -1;

//en-passant square 
int enpassant = no_square;//safe initialization

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
int castle;

//we need to convert chars 'P' to constant P , to be used by enum to convert to number index
// 'P' will become index 80 in this int array, and will store 80 value
// [index] allows us to place at a particular index
int char_pieces[128] {0};

/*##########################

Magic number generator

#############################*/
#include "MagicGenerator.hpp" 

/*##########################

Initialize everything

#############################*/

void init_everything()
{
    init_leapers_attacks();
    init_sliding_attacks();
    char_pieces['P'] = P;
    char_pieces['N'] = N;
    char_pieces['B'] = B;
    char_pieces['R'] = R;
    char_pieces['Q'] = Q;
    char_pieces['K'] = K;
    char_pieces['p'] = p;
    char_pieces['n'] = n;
    char_pieces['b'] = b;
    char_pieces['r'] = r;
    char_pieces['q'] = q;
    char_pieces['k'] = k;
}

/*##########################

Main driver

#############################*/

int main()
{
    ios::sync_with_stdio(false); //turn off sync with C's stdout
    cin.tie(nullptr); //stops flushing cout buffer before every cin operation
    // Set Windows console to UTF-8 mode only if on Windows
    #ifdef _WIN32 //_WIN32 has both 32 and 64, _WIN64 has only 64
        SetConsoleOutputCP(CP_UTF8);
    #endif
    cout<<"####################################\n";
    cout<<"Welcome to Domino, never lose again!\n";
    cout<<"####################################\n";
    init_everything();
    set_bit(bitboards[P], e2); //setting white pawn on e2
    print_bitboard(bitboards[P]);
    cout<<("piece: ")<<char_pieces['K']<<"\n";
    return 0;
}

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

#include "Board.hpp"

/*##########################

Magic number generator

#############################*/

#include "MagicGenerator.hpp" 

/*##########################

Move generator

#############################*/

#include "MoveGenerator.hpp"

/*##########################

Initialize everything

#############################*/

void init_everything()
{
    init_leapers_attacks();
    init_sliding_attacks();
    char_pieces['P'] = P; //0
    char_pieces['N'] = N; //1 and so on
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
    //initialise everything
    init_everything();
    parse_FEN_string("8/8/4r3/3B4/8/8/8/8 w - - ");
    print_board();
    print_bitboard(occupancies[both]);
    print_attacked_squares(white);
    return 0;
}

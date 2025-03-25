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
    parse_FEN_string("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ");
    print_board();
    int x = encode_move(5,6,K,Q,1,0,0,1);
    for (int i=23;i>=0;i--)
    {
        cout<<((x>>i)&1);
        if ((i%4)==0) cout<<" ";
    }
    cout<<"\n";
    cout<<decode_move_source(x)<<" ";
    cout<<decode_move_target(x)<<" ";
    cout<<decode_move_piece(x)<<" ";
    cout<<decode_move_promo_piece(x)<<" ";
    cout<<decode_move_capture(x)<<" ";
    cout<<decode_move_double(x)<<" ";
    cout<<decode_move_enpassant(x)<<" ";
    cout<<decode_move_castling(x)<<" ";
    return 0;
}

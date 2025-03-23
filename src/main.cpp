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
int side ;

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

//print the board
void print_board()
{
    cout<<"\n";
    //board rank = 8 - rank
    for (int rank = 0;rank<8;rank++)
    {
        cout<<" "<<(8 - rank)<<"  ";
        for (int file = 0;file<8;file++)
        {
            int square = 8*rank + file;
            int piece = -1;
            //check all piece bitboards;
            //remember how we label pieces PNBRQKpnbrqk
            for (int i=P;i<=k;i++)
            {
                if (get_bit(bitboards[i], square)) 
                {
                    piece = i; break;
                }
            }
            if (piece==-1) cout<<". ";
            else 
            {
                //print unicode only if windows
                #ifdef _WIN32
                    cout<<unicode_pieces[piece]<<" ";
                #else 
                    cout<<ascii_pieces[piece]<<" ";
                #endif
            }
        }
        cout<<"\n";
    }
    cout<<"    ";
    for (char file = 'a'; file<='h'; file++)
        cout<<file<<" ";
    cout<<"\n\n";
    //print other state variables
    cout<<"Side to move: "<<(!side?"White\n":"Black\n");
    cout<<"En-passant square: "<<(enpassant==64?"No square":square_to_board[enpassant])<<"\n";
    cout<<"Possible castles:\n";
    if ((castle>>0)&1) cout<<"White king side\n";
    if ((castle>>1)&1) cout<<"White queen side\n";
    if ((castle>>2)&1) cout<<"Black king side\n";
    if ((castle>>3)&1) cout<<"White queen side\n";
}

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
    set_bit(bitboards[P], a2); 
    set_bit(bitboards[P], b2);
    set_bit(bitboards[P], c2); 
    set_bit(bitboards[P], d2);
    set_bit(bitboards[P], e2); 
    set_bit(bitboards[P], f2);
    set_bit(bitboards[P], g2); 
    set_bit(bitboards[P], h2);
    set_bit(bitboards[p], a7); 
    set_bit(bitboards[p], b7);
    set_bit(bitboards[p], c7); 
    set_bit(bitboards[p], d7);
    set_bit(bitboards[p], e7); 
    set_bit(bitboards[p], f7);
    set_bit(bitboards[p], g7); 
    set_bit(bitboards[p], h7);
    set_bit(bitboards[R], a1); 
    set_bit(bitboards[N], b1);
    set_bit(bitboards[B], c1); 
    set_bit(bitboards[Q], d1);
    set_bit(bitboards[K], e1); 
    set_bit(bitboards[B], f1);
    set_bit(bitboards[N], g1); 
    set_bit(bitboards[R], h1);
    set_bit(bitboards[r], a8); 
    set_bit(bitboards[n], b8);
    set_bit(bitboards[b], c8); 
    set_bit(bitboards[q], d8);
    set_bit(bitboards[k], e8); 
    set_bit(bitboards[b], f8);
    set_bit(bitboards[n], g8); 
    set_bit(bitboards[r], h8);
    side=  black;
    castle |=wk;
    castle|=bk;
    enpassant = e3;
    print_board();
    for (int i=P;i<=k;i++)
    {
        print_bitboard(bitboards[i]);
    }
    return 0;
}

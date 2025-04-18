/*##########################

Define the board

#############################*/
#include "config.hpp"
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

//fifty move rule counter
int fifty;

//half move counter for search, starts from 0 for each search
int ply;

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
    if ((castle>>3)&1) cout<<"Black queen side\n";
    cout<<"Fifty move counter: "<<fifty<<"\n";
}

//parse FEN function
void parse_FEN_string(const string &fen)
{
    //reset board, occupancies and state variables
    memset(bitboards, 0ULL, sizeof(bitboards));
    memset(occupancies, 0ULL, sizeof(occupancies));
    side = 0;
    castle = 0;
    enpassant = no_square;
    int ind =0; //index in fen string
    for (int rank = 0; rank<8; rank++)
    {
        for (int file = 0; file<8;file++)
        {
            int square  = 8*rank+file;
            if ((fen[ind]>='a'&&fen[ind]<='z')||(fen[ind]>='A'&&fen[ind]<='Z'))
            {
                int piece = char_pieces[(int)fen[ind]]; //piece character to piece number
                set_bit(bitboards[piece],square); //place the piece
                ind++;
            }
            else if (fen[ind]>='0'&&fen[ind]<='9')
            {
                // empty squares, skip this many files
                file+= (fen[ind]-'0') - 1; //-1 because ++ from for loop
                ind++;
            }
        }
        ind++;
    }
    side = !(fen[ind]=='w'); //if white moves then side 0
    ind+=2;
    while(fen[ind]!=' ')
    {
        if (fen[ind]=='K') {castle|= wk;} //enums are already set
        else if (fen[ind]=='Q') {castle|= wq;}
        else if (fen[ind]=='k') {castle|= bk;}
        else if (fen[ind]=='q') {castle|= bq;}
        ind++;
    }
    ind++;
    if (fen[ind]!='-')
    {
        int file = (fen[ind]-'a'); //think e6 as example
        ind++;
        int rank = 8-(fen[ind]-'0'); //board rank  = 8 -rank, rank = 8 - board rank;
        ind++;
        enpassant = 8*rank + file;
        ind++;
    }
    else ind+=2; 
    
    //half move counter
    //temporary variable to store number of half moves
    int half_moves = 0;
    while(fen[ind]!=' ')
    {
        half_moves = half_moves*10 + (fen[ind]-'0');
        ind++;
    }
    fifty = half_moves;
    ind++;
    //full moves count to be added

    //fill occupancies
    for (int piece = P; piece <= K; piece++)
        occupancies[white] |= bitboards[piece];
    for (int piece = p; piece <= k; piece++)
        occupancies[black] |= bitboards[piece];
    occupancies[both] |= occupancies[white];
    occupancies[both] |= occupancies[black];
}
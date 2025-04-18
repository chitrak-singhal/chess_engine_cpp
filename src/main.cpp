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
}

//Precalculated Attack Tables (PAT)
#include "PAT.hpp"

//Define the board
#include "Board.hpp"

//Magic number generator
//#include "MagicGenerator.hpp" 

//Move generator
#include "MoveGenerator.hpp"

//Perft test
#include "Perft.hpp"

//UCI functions
#include "UCI.hpp"

//Evaluate
#include "Evaluate.hpp"

//Search
#include "Search.hpp"
// int best_move_debug;
// int negamax(int alpha, int beta, int depth)
// {
//     if (depth==0)
//         {return evaluate();}
//     MoveList move_list;
//     generate_moves(move_list);
//     int current_best_move;
//     int old_alpha = alpha; 
//     for (int i=0;i<move_list.index;i++)
//     {
//         copy_board();
//         ply++;
//         if (make_move(move_list.moves[i], all_moves)==0)
//         {
//             ply--; continue;
//         }
//         int score= -negamax(-beta, -alpha, depth-1);
//         ply--;
//         take_back();
//         if (score>=beta) return beta;;
//         if (score>alpha) {alpha = score; if (ply==0) current_best_move = move_list.moves[i];}       
//     }

//     //if alpha improves
//     if (old_alpha!=alpha)
//     //update best_move
//     {
//         best_move_debug = current_best_move;
//         //if (depth==4) cout<<"current best move: "<<current_best_move<<" depth: "<<depth<<"\n";
//     }
    
//     //return the best score
//     //node fails low (terminology)
//     return alpha;
// }

// void print_move_debug(int move)
// {
//     //if no prmotion (0 value) promoted_pieces gives null char, so nothing printed
//     //but this might not work everytime, so to be safe, let's add if else
//     if (promoted_pieces[decode_move_promo_piece(move)])
//         //removed new lines for uci reasons
//         cout<<square_to_board[decode_move_source(move)]<<square_to_board[decode_move_target(move)]<<promoted_pieces[decode_move_promo_piece(move)];
//     else
//         cout<<square_to_board[decode_move_source(move)]<<square_to_board[decode_move_target(move)];
// }
// void search_position_debug(int depth)
// {
//     //search for best move in current position
//     //50K is infinity at our scale
//     int score = negamax(-50000, 50000, depth);

//     cout<<score<<"\n";
//     //best move placeholder (bestmove is UCI command)
//     cout<<"bestmove ";
//     print_move_debug(best_move_debug);
//     cout<<"\n";
// }

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
    //according to uci protocol all promoted pieces are in lower case
    promoted_pieces[Q] = 'q';
    promoted_pieces[R] = 'r';
    promoted_pieces[B] = 'b';
    promoted_pieces[N] = 'n';
    promoted_pieces[q] = 'q';
    promoted_pieces[r] = 'r';
    promoted_pieces[b] = 'b';
    promoted_pieces[n] = 'n';
}

/*##########################

Main driver

#############################*/

int main()
{
    ios::sync_with_stdio(false); //turn off sync with C's stdout
    //cin.tie(nullptr); //stops flushing cout buffer before every cin operation
    // Set Windows console to UTF-8 mode only if on Windows
    #ifdef _WIN32 //_WIN32 has both 32 and 64, _WIN64 has only 64
        SetConsoleOutputCP(CP_UTF8);
    #endif

    cout<<"####################################\n";
    cout<<"Welcome to Domino, never lose again!\n";
    cout<<"####################################\n";

    //initialise everything
    init_everything();

    bool debug  = false;
    if (debug) 
    {
        

        //testing evaluate
        parse_FEN_string("R7/6P1/5Q2/2k5/8/8/7Q/4Q1K1 w KQkq - 0 1" );
        print_board();
        search_position(4);
        MoveList move_list;
        generate_moves(move_list);
        move_list.print_move_list();
    }
    else
        //connect to the GUI
        uci_loop();

    return 0;
}

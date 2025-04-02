/*##########################

UCI functions

#############################*/
#include "config.hpp"
#include "MoveGenerator.hpp"

//get time in milisecs
int get_time_ms()
{
    #ifdef _WIN32
        return GetTickCount();
    #else
        struct timeval time_value;
        gettimeofday(&time_value, NULL);
        return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
    #endif
}

//parse user or GUI move string for UCI purpose (a2a3, a7a8q)
int parse_move_string(const string& move_string)
{
    //we need to check if move provided by the user/GUI is legal
    //so create a move list and check if that move is present in it

    MoveList move_list;
    generate_moves(move_list);

    //parse the move rank = 8 - board rank
    //square = 8*rank + file = 8*(8-board_rank) + file
    int source_square = 8*(8 - (move_string[1] - '0')) + (move_string[0] - 'a');
    int target_square = 8*(8 - (move_string[3] - '0')) + (move_string[2] - 'a');
    //check if provided move has promotion
    int ind = 0;
    while(move_string[ind]!='\0') {ind++;}
    bool has_promotion  = ((ind>=5)?true:false);
    //iterate over the list to check if this move is present
    for (int i=0;i<move_list.index;i++)
    {
        int move = move_list.moves[i];
        //if soruce and target match
        if (decode_move_source(move)==source_square && decode_move_target(move)==target_square)
        {            
            //if promotion is involved
            if (has_promotion)
            {
                //prmotion piece matches
                if (promoted_pieces[decode_move_promo_piece(move)] == move_string[4])
                    return move;
            }
            //move matched
            else
            {
                //if the move from move list has promotion skip
                if (decode_move_promo_piece(move)) continue;
                
                //legal move
                return move;
            }
        }
    }

    //illegal move
    return 0;
}
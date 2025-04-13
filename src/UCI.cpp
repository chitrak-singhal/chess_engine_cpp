/*##########################

UCI functions

#############################*/
#include "config.hpp"
#include "MoveGenerator.hpp"
#include "Board.hpp"

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

//parse "position" command for UCI (examples of this command are in notion doc)
void parse_position(const string &command)
{
    //cuurent index set after "position"
    int ind = 9;
    //stores index of moves keyword, using size_t because it is the right thing to do and also becuase we will be comparing with string::npos which is size_t type (unsigned int basically) and 2^64 -1, int is only 2^63 - 1
    size_t pos = string::npos;
    //avoid researching for moves keyword
    bool searched_for_moves = false;
    //if startpos is present
    if (command.compare(ind, 8, "startpos")==0)
    {
        //initialize starting position
        parse_FEN_string(start_position);
    }
    //fen is present
    else
    {
        //if fen not present by mistake
        if (command[ind]!='f') {parse_FEN_string(start_position);}
        //fen present
        else
        {
            //first character of fen
            ind+= 4; 
            //check if moves keyword present
            pos = command.find('m');
            searched_for_moves = true;
            //if moves keyword found fen string is from ind till pos - 2
            if (pos!=string::npos)
                parse_FEN_string(command.substr(ind, pos-2-ind+1));
            //if moves keyword not found, take till last
            else
                parse_FEN_string(command.substr(ind));
        }
    }
    //if we havent searched for moves keyword in above code yet
    if (!searched_for_moves)
        pos = command.find('m');
    //if moves keyword found
    if (pos!=string::npos)
    {
        ind = pos;
        //shift index to first move
        ind+= 6;
        int n = command.size();
        while(ind<n)
        {
            int move = 0;
            //move does not have promotion
            if (command[ind+4]==' '|| ind+4==n)
                {move = parse_move_string(command.substr(ind, 4)); ind+=5;} //parse and shift index to next move
            //move has promotion
            else
                {move = parse_move_string(command.substr(ind, 5)); ind+=6;} //parse and shift index to next move
            
            //illegal move
            if (move==0)
                break;
            
            //make the move on the chess board
            make_move(move, all_moves);
        }
    }
}
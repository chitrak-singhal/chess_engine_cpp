/*##########################

Perft test

#############################*/
#include "config.hpp"
#include "MoveGenerator.hpp"
#include "UCI.hpp"

//the perft test
//takes depth as parameter returns number of nodes
U64 perft(int depth)
{
    //base case of recursion, return one node at last level
    if (depth==0) return 1ULL;
    //total nodes
    U64 nodes = 0;
    //this will store move list from this position
    MoveList move_list;
    //generate moves from this position
    generate_moves(move_list);
    //iterate over all possible moves 
    for (int i=0;i<move_list.index;i++)
    {
        //preserve board state for next move
        copy_board();
        //make the move, if it was legal, call perft recursively
        if (make_move(move_list.moves[i], all_moves))
        {
            nodes+= perft(depth-1);
        }
        //take back the move so we can explore the next move branch
        take_back();
    }
    //return the nodes number
    return nodes;
}

//to run the test
//this will start the test ans measure time
void perft_test(int depth)
{
    //time at start of test
    U64 start_time = get_time_ms();
    //number of nodes 
    U64 nodes = perft(depth);
    //time at end of test
    U64 end_time = get_time_ms();
    cout<<"Time taken: "<<(end_time-start_time)<<"ms\n";
    cout<<"No. of nodes "<<nodes<<"\n";

    //matching with predetermined values to see if we passed or failed
    if (depth==6)
    {
        if (nodes==119060324) cout<<"Test Passed\n";
        else cout<<"Test Failed\n";
    }
    else if (depth==7)
    {
        if (nodes==3195901860) cout<<"Test Passed\n";
        else cout<<"Test Failed\n";
    }
}
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

Main driver

#############################*/

int main()
{
    ios::sync_with_stdio(false); //turn off sync with C's stdout
    cin.tie(nullptr); //stops flushing cout buffer before every cin operation 
    cout<<"####################################\n";
    cout<<"Welcome to Domino, never lose again!\n";
    cout<<"####################################\n";
    init_leapers_attacks();
    cout<<random_generator()<<"\n";
    cout<<random_generator()<<"\n";
    cout<<random_generator()<<"\n";
    cout<<random_generator()<<"\n";
    return 0;
}

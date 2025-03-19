#include "config.hpp"
#include "PAT.hpp"
/*##########################

Magic number generator

#############################*/
//random number generator
unsigned int state = 1804289383;
unsigned int random_generator_U32()
{
    unsigned int num = state;
    //xorshift32 algorithm
    num^= (num<<13);
    num^= (num>>17); 
    num^= (num<<5);

    state = num; //update state

    return num;
}

//this random number will be used to generate the magic number candidates
U64 random_generator_U64() 
{
    //define 4 random numbers low non-zero bits numbers
    U64 num1 = ((U64)(random_generator_U32()) & 0xFFFF); //& with 0xFFFF keeps only first 16 bits
    U64 num2 = ((U64)(random_generator_U32()) & 0xFFFF);
    U64 num3 = ((U64)(random_generator_U32()) & 0xFFFF);
    U64 num4 = ((U64)(random_generator_U32()) & 0xFFFF);

    return ((num1)| (num2<<16)|(num3<<32)|(num4<<48));
}

//generate magic number candidate
U64 generate_magic_number_candidate()
{
    return (random_generator_U64()&random_generator_U64()&random_generator_U64());
}

//checks if generated magic number candidates are valid for a square
U64 search_magic_number(int square,  int is_bishop)
{
    //array of blocker bitboards
    // size is 4096 because maximum number of blocker bitboards for a relevant occupancy mask is 2^12 (since there are at most 12 relevant occupancies for any square)
    U64 blocker_bitboards[4096];

    //array of attack sets
    //size is same as blocker_bitboards_because for each blocker bitboard, we will store corresponding attack
    U64 attacks[4096]; 
    
    //blocker_to_attack_map, it maps blocker bitboard to attack (our ultimate goal), used to check collisons later on
    U64 blocker_to_attack_map[4096];

    //get the relevant occupancy mask for either bishop/rook
    U64 relevant_occupancy_mask = is_bishop?generate_relevant_occupancy_mask_bishop(square):generate_relevant_occupancy_mask_rook(square);

    //number of relevant occupancies
    int num_of_relevant_bits = is_bishop?relevant_bit_count_bishop[square]:relevant_bit_count_rook[square];
    
    //num of blocker bitboards is 2^(number of set bits in relevant occupancy mask)
    int num_of_blocker_bitboards = (1<<num_of_relevant_bits);
    //cout<<num_of_blocker_bitboards<<"b\n";

    for (int blocker_bitboard_index = 0; blocker_bitboard_index<num_of_blocker_bitboards; blocker_bitboard_index++)
    {
        int index  =blocker_bitboard_index; //just for clarity
        //filling blocker bitboards array
        blocker_bitboards[index] = generate_blocker_bitboard(index, num_of_relevant_bits, relevant_occupancy_mask);

        //filling attacks array
        attacks[index] = is_bishop?generate_bishop_attack_sets(square, blocker_bitboards[index])
                        :generate_rook_attack_sets(square, blocker_bitboards[index]);
    }

    //test magic numbers
    //we will generate magic number candidates 1e8 times and take the one that fit
    for (int random_cnt = 0; random_cnt<1e8;random_cnt++)
    {
        //magic number candidate
        U64 magic_number = generate_magic_number_candidate();
        //preliminary test to outright reject bad ones
        if (bit_count((relevant_occupancy_mask*magic_number)&0xFF00000000000000)<6) continue;
        //cout<<hex<<magic_number<<" "<<dec<<random_cnt<<"x\n";

        //reset blocker_to_attack_map
        memset(blocker_to_attack_map, 0ULL, sizeof(blocker_to_attack_map));

        int index, fail; //index is bitboard_blocker_index

        //test magic INDEX
        /* 
        1) iterate over all blocker bitboards
        2)  magic_index = blocker_bitboard * magic_number, shifted by 64-n, where n is the number of relevant occupancies
        3) check if this magic index works (no collisons with previous blocker bitboards)
        4) Return the magic number if it passes, else check another one.
        */
        for (index=0,fail=0; !fail && index<num_of_blocker_bitboards; index++)
        {
            //index is blocker_bitboard_index
            int n = num_of_relevant_bits;
            int magic_index = (int)((blocker_bitboards[index]*magic_number)>>(64-n));
            
            //if there is no attack stored at this magic index
            if (blocker_to_attack_map[magic_index] == 0ULL)
                blocker_to_attack_map[magic_index]  = attacks[index];

            //if there is a collison (the map already has some value, different than what we want to store)
            //Note it is possible that some other blocker pattern had the same magic_index and same attack, therefore we use the condition map[magic_index]!=attack[index] instead of simply map[magic_index]!=0
            else if (blocker_to_attack_map[magic_index]!=attacks[index])
    
                fail = 1; //invalid magic number;
        }
        if (!fail) return magic_number;
    }

    //we reach this point if no magic number was found despite trying 1e8 times
    cout<<"Error: Magic Number search failed!\n";
    return 0ULL;
}

//initialise list of magic numbers for each square
void init_magic_numbers()
{
    for (int square =0; square<64; square++)
    {
        //for rook
        U64 magic_num = search_magic_number(square,rook);
        cout<<hex<<"0x"<<magic_num<<"ULL,\n";
    }
    cout<<"############\n\n";
    for (int square =0; square<64; square++)
    {
        //for bishop
        U64 magic_num = search_magic_number(square,bishop);
        cout<<hex<<"0x"<<magic_num<<"ULL,\n";
    }
}

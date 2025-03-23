#ifndef BITMANIPULATION_HPP
#define BITMANIPULATION_HPP //definition safeguards
#define U64 unsigned long long
/*##########################

Bit manipulation functions and macros

#############################*/

#define get_bit(bitboard, square) (((bitboard)>>(square))&1ULL)
#define set_bit(bitboard, square) ((bitboard)|=(1ULL<<(square)))
//if we reset using xor, calling reset multiple times can set the bit accidently, so we need an if condition along with it
#define reset_bit(bitboard, square) {if (get_bit(bitboard, square)) {(bitboard)^= (1ULL<<(square));}}

//count bits function
//declared static inline since we will use it often
//inline suggests(not manadates) the compiler to replace function call with actual code, reduces overhead
//static limits scope of function to this file only
//static makes sure each .cpp file that includes this header gets its own independent copy of the function, preventing linker conflicts
static inline int bit_count(U64 bitboard)
{
    int bitcnt = 0; 
    while(bitboard) {bitcnt++; bitboard &=(bitboard-1);} //reset FSB (first set bit)
    return bitcnt;
}

//get FSB function
static inline int get_fsb(U64 bitboard)
{
    if (!bitboard) return -1;
    return bit_count((bitboard&-bitboard)-1);
}

#endif
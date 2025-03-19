#ifndef MAGICGENERATOR_HPP
#define MAGICGENERATOR_HPP
/*##########################

Pre-saved magic numbers and magic number generator

#############################*/
#include "config.hpp"
#include "PAT.hpp"

//random number generator
extern unsigned int state;
unsigned int random_generator_U32();
U64 random_generator_U64(); //this random number will be used to generate the magic number candidates
//generate magic number candidate
U64 generate_magic_number_candidate();

//checks if generated magic number candidates are valid for a square
U64 search_magic_number(int square, int is_bishop);

//initialise list of magic numbers for each square
void init_magic_numbers();

#endif
/*##########################

Perft test

#############################*/
#ifndef PERFT_HPP
#define PERFT_HPP

#include "config.hpp"
#include "MoveGenerator.hpp"
#include "UCI.hpp"

//the perft test
//takes depth as parameter returns number of nodes
U64 perft(int depth);

//to run the test
//this will start the test ans measure time
void perft_test(int depth);

#endif
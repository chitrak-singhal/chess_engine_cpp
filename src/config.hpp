#ifndef CONFIG_HPP
#define CONFIG_HPP

/*##########################

Including system files

#############################*/

#include<iostream>
#include <array>
using namespace std;

//including bit manipulation functions
#include "BitManipulation.hpp"

//bitboard datatype already defined in BitManipulation.hpp
//#define U64 unsigned long long

//file constants //using const instead of constexpr avoids multiple copies in different cpp files, constexpr variables must be evaluated at compile time
constexpr U64 A_FILE = 0x0101010101010101;
constexpr U64 H_FILE = 0x8080808080808080;
constexpr U64 B_FILE = 0x0202020202020202;
constexpr U64 G_FILE = 0x4040404040404040;
constexpr U64 GH_FILE = H_FILE|G_FILE;
constexpr U64 AB_FILE = A_FILE|B_FILE;

//enum board square
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3, 
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
};

//covert square number to board square
//using const would lead to mulitple copies
//but cannot use constexpr with string since string is not a literal type
//literal types can be ompletely evaluated at compile time
// array of literal types is literal type
// so we use constexpr array<const char*, 64> which is a compile-time constant
constexpr std::array<const char*, 64> square_to_board = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"};

//enum sides to move
enum {white, black}; //0 means white

#endif
/*##########################

UCI functions

#############################*/
#ifndef UCI_HPP
#define UCI_HPP

#include "config.hpp"

//get time in milisecs
int get_time_ms();

//parse move string for UCI purpose (a2a3, a7a8q)
int parse_move_string(const string &move_string);

//parse "position" command for UCI (examples of this command are in notion doc)
void parse_position(const string &command);

#endif
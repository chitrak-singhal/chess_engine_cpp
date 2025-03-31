/*##########################

UCI functions

#############################*/
#include "config.hpp"

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
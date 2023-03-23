#ifndef __PATH_H__
#define __PATH_H__

#include "../config.h"

// each unit in the path. eg. C:/Users, C, Users are induvidual units
struct path_unit{
    char *unit_str; // each unit stored in string format.
    struct path_unit* next_unit;
};  

// path of a file in the file system
struct path{

    char drive_id; // drives are identified by uppercase characters. eg: A, B, C, ...
    struct path_unit* units; // first unit of the path

};



#endif
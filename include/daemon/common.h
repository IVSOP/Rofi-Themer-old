#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <iomanip>
#include <iostream>

// type of entry
// ignore numbers, ended up not using them
enum TYPE {
	APPLY = 0,        // apply this setting
	SUB = 1,          // goes into subtable
	LIST = 2,         // show list of options, can only pick 1
	LIST_PICTURE = 4, // same but strings are pictures
	APPLY_LIST = 8
};

#endif

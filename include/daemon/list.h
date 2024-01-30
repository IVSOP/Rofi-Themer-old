#ifndef LIST_H
#define LIST_H

#include <vector>
#include <string>
#include "common.h"

// this is similar to a table, but has some incompatible differences:
// I need to store the current used option (!= theme) and a list of strings for each theme
// In the future I might big brain it and just use tables for everything

class List {
public:
	int selected_option = -1;
	std::vector<std::vector<std::string>> data; // data for every theme is a vector, so I have to make this mess
	bool show_pictures = false;

	List() = default;
	List(std::string &line, bool show_pictures);
	~List() = default;

	void print(int depth_level);

private:
	void parseList(std::string list); // NOT a reference!!! afraid the erase() would mess everything up
};


#endif

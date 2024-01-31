#ifndef DATA_H
#define DATA_H

// actual data built from the data files, also contains infos that are global to them, like color icons

#include "table.h"
#include "entry.h"

struct Data {
	// int total_themes = 0; just check color_icons length
	std::vector<std::string> color_icons;
	Table main_table;

	Data(const std::string &dir);

	void parseColors(const std::string &path);
	void print();
	std::string read(std::string &input) const;
};

#endif

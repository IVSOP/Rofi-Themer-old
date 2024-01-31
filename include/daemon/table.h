#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <map>
#include <string>
#include "entry.h"

// data structure representing a table retrieved from data file
struct Table {
	std::map<std::string, Entry> data; // the actual data in the table, looked up by string
	int most_used = -1; // most used theme, this way it is not recalculated constantly

	Table() = default;
	Table(const std::string &path);
	~Table() = default;

	void print(int depth_level);

	// sort of a bandaid fix, not elegant for this to receive color_icons, gets passed down recursively all the time
	std::string read(std::string &input) const;
};

#endif

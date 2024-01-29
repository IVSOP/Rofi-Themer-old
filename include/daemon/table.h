#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <unordered_map>
#include <string>
struct Entry;

// data structure representing a table retrieved from data file
struct Table {
	std::unordered_map<std::string, Entry> data; // the actual data in the table, looked up by string
	std::vector<int> active; // active[i] == j means option #i has theme #j
	int most_used = -1; // most used theme, this way it is not recalculated constantly

	Table() = default;
	Table(std::unordered_map<std::string, Entry> data) { this->data = data; }
	~Table() = default;
};

#endif

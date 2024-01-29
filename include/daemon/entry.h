#ifndef ENTRY_H
#define ENTRY_H

#include "common.h"
struct Table;
#include <variant>


// an entry in the table
struct Entry {
	TYPE type;
	// vectors and tables have constructors and destructors, so simple union cannot be used, had to use variant
	std::variant<
			std::vector<std::string>, // case of apply
			Table // case of both subtable and list are both represented as a new table
	> data;

	Entry() = default;
	Entry(std::vector<std::string> strings);
	Entry(TYPE type, Table table);
	~Entry() = default;
};

#endif

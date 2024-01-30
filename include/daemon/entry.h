#ifndef ENTRY_H
#define ENTRY_H

#include "common.h"
struct Table;
#include <variant>
#include <memory>
#include "list.h"


// an entry in the table
struct Entry {
	TYPE type;
	// vectors and tables have constructors and destructors, so simple union cannot be used, had to use variant
		// HOW DO THESE POINTERS GET FREED WTF
	std::variant<
			std::vector<std::string>, // case of apply
			std::unique_ptr<Table>, // case of both subtable and list are both represented as a new table. pointer due to circular dependency
			std::unique_ptr<List> // case of being a list, cannot be a table unfortunately. could be object and not pointer, but this makes it consistent
	> data;
	int active_theme; // theme selected as active for this entry

	Entry() = default;
	Entry(const std::string &name, std::string &line); // receives line, parses it
	void parseApply(std::string &line);
	void parseSub(const std::string &name, std::string &line);
	void parseList(std::string &line, bool show_pictures);
	// Entry(std::vector<std::string> strings);
	// Entry(TYPE type, Table table);
	~Entry() = default;

	void print(int depth_level) const;
};

#endif

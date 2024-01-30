#include "table.h"
#include <fstream>
#include "errors.h"

Table::Table(const std::string &path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		print_error("Error opening file");
		exit(1);
    }

	std::string line;
	size_t pos;
    while (std::getline(file, line)) {
		pos = line.find(';');
		std::string name = line.substr(0, pos);
		std::string data = line.substr(pos + 1);

		this->data.emplace(name, data); // careful, calls Entry(line)
    }

	file.close();
}

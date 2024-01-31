#include "table.h"
#include <fstream>
#include "errors.h"
#include <algorithm>

Table::Table(const std::string &path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		print_error("Error opening file");
		fprintf(stderr, "'%s'\n", path.c_str());
		exit(1);
    }

	printf("parsing %s\n", path.c_str());

	std::string line;
	size_t pos;
    while (std::getline(file, line)) {
		pos = line.find(';');
		std::string name = line.substr(0, pos);
		std::string data = line.substr(pos + 1);

		// WHAT THE FUCK?????? most cursed thing I have ever seen. why can I not just enter 3 arguments and live in peace wtf
		this->data.emplace(
			::std::piecewise_construct, // special to enable forwarding
			::std::forward_as_tuple(name), // arguments for key constructor
			::std::forward_as_tuple(name, data) // arguments for value constructor
		);
		// this->data.emplace(name, Entry(name, data)); // error wtf???
    }

	// calculate most used themes
	std::vector<int> themes(data.size());
	for (unsigned int i = 0; i < themes.size(); i++) {
		themes[i] = 0;
	}
	for (const auto &entrypair : this->data) {
		themes[entrypair.second.active_theme] += 1; // if not set, will be -1, segfault prob
	}

	// this is to find the max, got lazy
	this->most_used = std::distance(themes.begin(),std::max_element(themes.begin(), themes.end()));

	file.close();
}

void Table::print(int depth_level) {
	std::cout << std::setw((depth_level - 1) * 4) << "" << "[" << std::endl;
	for (const auto &entrypair : this->data) {
		std::cout << std::setw(depth_level * 4) << "" << entrypair.first << " (" << entrypair.second.active_theme << "): ";
		entrypair.second.print(depth_level);
	}
	std::cout << std::setw((depth_level - 1) * 4) << "" << "] most used: " << this->most_used << std::endl;
}

std::string Table::read(std::string &input) const {
	size_t pos = input.find('/');
	if (pos == std::string::npos) {
		// error..........................
	}
	std::string name = input.substr(0, pos);
	if (name == "*") { // get all the data
		std::string wildcard = "*"; // got lazy
		std::string res = "";
		for (const auto &entrypair : this->data) { // got lazy iterating values only
			res += entrypair.second.read(wildcard);
		}
		return res;
	} else {
		const Entry &entry = this->data.at(name);
		
		// catch the exception.................
		
		input.erase(0, pos + 1); // 1 is len of delimiter
		return entry.read(input);
	}
}

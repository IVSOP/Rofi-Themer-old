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

	// printf("parsing %s\n", path.c_str());

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

	calcMostUsed();

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
	// if (pos == std::string::npos) {
	// 	print_error("read is invalid");
	// }
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

// calculate most used themes
void Table::calcMostUsed() {
	std::vector<int> themes(data.size());
	for (unsigned int i = 0; i < themes.size(); i++) {
		themes[i] = 0;
	}
	for (const auto &entrypair : this->data) {
		themes[entrypair.second.active_theme] += 1; // if not set, will be -1, segfault prob
	}

	// this is to find the max, got lazy
	this->most_used = std::distance(themes.begin(),std::max_element(themes.begin(), themes.end()));
}

// info needs to be passed, so that all the tables before it can add things to it, otherwise it would be lost
// input is the same string but things that were parsed were erased
// for example:
// 		input:   rofi/blabla
//		options: m/1/rofi/blabla
// and will go into table 'rofi'

// input is string that gets parsed
// info is accumulator of the path through the data structure to get here

std::string Table::menu(int theme, std::string &input, std::string &info, const std::vector<std::string> &color_icons) {
	// if there is no /, pos will be very big and the token will be the entire input
	size_t pos = input.find('/');
	std::string token = input.substr(0, pos);
	input.erase(0, pos + 1); // 1 is len of delimiter

	if (token.length() == 0) { // show menu of this table
		std::string res = "";
		for (const auto &entrypair : this->data) { // got lazy iterating values only
			// printf("Adding %s\n", entrypair.first.c_str());
			res += entrypair.second.menu(entrypair.first, info, color_icons); // this menu func only displays their name
		}
		// write(STDOUT_FILENO, res.c_str(), res.size());
		return res;
	} else if (token == "*") { // apply all options on this table
		return "* not implemented";
	} else {
		Entry &entry = this->data.at(token); // TODO error handling if it does not exist

		// this seems very confusing, but tables never alter info, only the entries
		// this way, for the back option, instead of removing last option, it already does not exist
		// ex 1/rofi, ou are in the menu for rofi. The back option would have info = 1/. 1/ is already what was passed as info, just copy it
		std::string rofi_str = entry.menu(token, theme, input, info, color_icons);

		// since we do not know if things were changed or not, since going into a new menu may or may not result in changes, allways recalculate most used themes
		calcMostUsed();

		// rofi_active();

		return rofi_str;
	}
}

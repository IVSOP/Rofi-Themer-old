#include "data.h"
#include <fstream>
#include "errors.h"

Data::Data(const std::string &dir)
: color_icons(), main_table(dir + "main.tb")
{
	parseColors(dir + "color-icons");
}

void Data::parseColors(const std::string &path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		print_error("Error opening file");
		exit(1);
    }

	std::string line;
	const std::string delim = "#";
    while (std::getline(file, line)) {
		// ignore comments, only valid at the start of the line for now
		// if (line.compare(0, delim.length(), delim) != 0) {
        	this->color_icons.emplace_back(line);
		// } did not implement this, when files get overwritten it all goes away
    }

	file.close();
}

void Data::print() {
	puts("Color icons are:");
	for (const std::string &str : color_icons) {
		puts(str.c_str());
	}

	puts("Data:");
	main_table.print(1);
}
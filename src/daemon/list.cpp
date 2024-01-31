#include "list.h"

void List::parseList(std::string list) {
	// printf("got token %s\n", list.c_str());
	size_t pos;
	std::string token;
	std::vector<std::string> vec;

	do {
		pos = list.find(';');
		token = list.substr(0, pos);
		if (token.length() > 0) vec.push_back(token);
		list.erase(0, pos + 1); // 1 is len of delimiter
		// puts(token.c_str());

	} while (pos != std::string::npos);

	this->data.push_back(vec); // WARNING THIS ASSUMES CORRECT THEME ORDER
}

List::List(std::string &line, bool show_pictures)
: show_pictures(show_pictures), data()
{
	// get selected option, should be first number
	size_t start = line.find(';');
	this->selected_option = std::stoi(line);
	line.erase(0, start + 1);

	// separate all [lists], parse them individually
	start = line.find('[');
	size_t end = line.find(']');
	std::string token;
	while (start != std::string::npos) {
		token = line.substr(start + 1, end - 2); // why did -1 not work wtf I give up
		parseList(token);

		line.erase(0, end + 1);
		start = line.find('['),
		end = line.find(']');
	}
}

void List::print(int depth_level) {
	std::cout << std::setw((depth_level - 1) * 4) << "" << "{" << std::endl;
	for (const auto &vec : this->data) {
		std::cout << std::setw(depth_level * 4) << "";
		for (const auto &str : vec) {
			std::cout << "" << str << ";";
		}
		std::cout << std::endl;
	}
	std::cout << std::setw((depth_level - 1) * 4) << "" << "} selected: " << this->selected_option << std::endl;
}

std::string List::read(std::string &input, int active_theme) const {
	const std::vector<std::string> &vec = this->data[active_theme];
	return vec[this->selected_option] + "\n";
}

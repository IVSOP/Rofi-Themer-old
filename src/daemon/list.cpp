#include "list.h"

void List::parseList(std::string list) {
	// printf("got token %s\n", list.c_str());
	size_t pos;
	std::string token;
	std::vector<std::string> vec;
	while ((pos = list.find(';')) != std::string::npos) {
		token = list.substr(0, pos);
		if (token.length() > 0) vec.push_back(token);
		list.erase(0, pos + 1); // 1 is len of delimiter
		// puts(token.c_str());
	}
	this->data.push_back(vec); // WARNING THIS ASSUMES CORRECT THEME ORDER
}

List::List(std::string &line, bool show_pictures)
: show_pictures(show_pictures), data()
{
	// separate all [lists], parse them individually
	size_t start = line.find('['),
	end = line.find(']');
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
	std::cout << std::setw((depth_level - 1) * 4) << "" << "}" << std::endl;
}

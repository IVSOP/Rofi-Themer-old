#include "entry.h"
#include "table.h"
#include "errors.h"

Entry::Entry(const std::string &name, std::string &line) {
	// first is the type, this determines what parsing will be done
	size_t pos = line.find(';');
	std::string typestr = line.substr(0, pos);
	std::string data = line.substr(pos + 1);

	// second is what theme was selected

	// very inefficient but it will be fast anyway, idc
	if (typestr == "apply") {
		this->type = APPLY;
		parseApply(data);
	} else if (typestr == "apply_list") {
		this->type = APPLY_LIST;
		parseApplyList(data);
	} else if (typestr == "sub") {
		this->type = SUB;
		parseSub(name, data);
	} else if (typestr == "list") {
		this->type = LIST;
		parseList(data, false);
	} else if (typestr == "list_picture") {
		this->type = LIST_PICTURE;
		parseList(data, true);
	} else {
		print_error("Invalid type");
		fprintf(stderr, "'%s'\n", typestr.c_str());
		exit(1);
	}
}

void Entry::parseApply(std::string &line) {
	// just add every entry to vector of strings
	std::vector<std::string> vec = std::vector<std::string>();

	size_t pos = 0;
	std::string token;

	// first token is a number
	pos = line.find(';');
	// if pos == std::string::npos.....
	this->active_theme = std::stoi(line);
	line.erase(0, pos + 1);

	// loops like these are do while since the last element is ";..." and not ";...;"

	do {
		pos = line.find(';');
		token = line.substr(0, pos);
		if (token.length() > 0) vec.push_back(token);
		line.erase(0, pos + 1); // 1 is len of delimiter
		// puts(token.c_str());
	} while (pos != std::string::npos);

	this->data = vec;
}

void Entry::parseSub(const std::string &name, std::string &line) {
	// got lazy moving this to main function
	size_t pos = line.find(';');
	// if pos == std::string::npos.....
	this->active_theme = std::stoi(line);
	line.erase(0, pos + 1);

	this->data.emplace<std::unique_ptr<Table>>(std::make_unique<Table>("data/" + name + ".tb"));
}

void Entry::parseList(std::string &line, bool show_pictures) {
	// got lazy moving this to main function
	size_t pos = line.find(';');
	// if pos == std::string::npos.....
	this->active_theme = std::stoi(line);
	line.erase(0, pos + 1);

	this->data.emplace<std::unique_ptr<List>>(std::make_unique<List>(line, show_pictures));
}

// bad at naming things, parses [....] into vec of strings
void parseApplyListAux(std::string &list, std::vector<std::string> &vec) {
	size_t pos;
	std::string token;
	do {
		pos = list.find(';');
		token = list.substr(0, pos);
		if (token.length() > 0) vec.push_back(token);
		list.erase(0, pos + 1); // 1 is len of delimiter
		// puts(token.c_str());
	} while (pos != std::string::npos);
}

void Entry::parseApplyList(std::string &line) {
	// got lazy moving this to main function
	size_t pos = line.find(';');
	// // if pos == std::string::npos.....
	this->active_theme = std::stoi(line);
	line.erase(0, pos + 1);

	std::vector<std::vector<std::string>> main_vec;
	

	// separate all [lists], parse them individually
	size_t start = line.find('['),
	end = line.find(']');
	std::string token;
	while (start != std::string::npos) {
		// std::cout << "parsing line " << line << std::endl;
		token = line.substr(start + 1, end - 1 - start);
		// std::cout << "parsing list " << token << std::endl;
		std::vector<std::string> vec;
		parseApplyListAux(token, vec);
		main_vec.push_back(vec);

		line.erase(0, end + 1);
		start = line.find('['),
		end = line.find(']');
	}

	this->data = main_vec;
}

void Entry::print(int depth_level) const {
	switch (this->type) {
		case APPLY:
			for (const std::string &str : std::get<APPLY_DATA>(this->data)) {
				std::cout << str << ";";
			}
			std::cout << std::endl;
			break;

		case APPLY_LIST: // why is the depth_level different here from everywhere else wtf
			std::cout << std::endl;
			std::cout << std::setw(depth_level * 4) << "" << "{" << std::endl;
			for (const auto &vec : std::get<APPLY_LIST_DATA>(this->data)) {
				for (const auto &str : vec) {
					std::cout << std::setw((depth_level + 1)  * 4) << "" << str << ";";
				}
				std::cout << std::endl;
			}
			std::cout << std::setw(depth_level * 4) << "" << "} (apply)" << std::endl;
			break;

		case SUB:
			std::cout << std::endl;
			std::get<SUB_DATA>(this->data).get()->print(depth_level + 1);
			break;

		case LIST:
			std::cout << std::endl;
			std::get<LIST_DATA>(this->data).get()->print(depth_level + 1);
			break;

		case LIST_PICTURE:
			std::cout << std::endl;
			std::get<LIST_DATA>(this->data).get()->print(depth_level + 1);
			break;
	}
}

std::string Entry::read(std::string &input) const {
	switch (this->type) { // not making a jump table due to poor readability, I will assume compiler does it for me
		case APPLY:
			{ // wtf
				// check if 0 size????
				const APPLY_DATA &vec = std::get<APPLY_DATA>(this->data);
				return vec[this->active_theme] + "\n";
				break;
			}

		case APPLY_LIST:
			{
				const APPLY_LIST_DATA &vec = std::get<APPLY_LIST_DATA>(this->data);
				std::string res = "";
				for (const std::string &str : vec[this->active_theme]) {
					res += str + "\n";
				}
				return res;
			}

		case SUB:
			return std::get<SUB_DATA>(this->data).get()->read(input);
			break;

		case LIST:
			return std::get<LIST_DATA>(this->data).get()->read(this->active_theme);
			break;

		case LIST_PICTURE:
			return std::get<LIST_DATA>(this->data).get()->read(this->active_theme);
			break;

		default: // I will assume the type can never be anything else ever, but this way compiler shuts up
			return "error";
	}
}

std::string Entry::print_option(const std::string &name, const std::string &info, const std::vector<std::string> &color_icons) const {
	return rofi_message(name, color_icons[this->active_theme], info);
}

// print only func, called only by the table. otherwise, apply and apply list would have teir options changed
std::string Entry::menu(const std::string &name, const std::string &info, const std::vector<std::string> &color_icons) const {
	return print_option(name, info + "/" + name, color_icons);
}

// info + "/" + name repeated seems bad, but makes easier to implement 'Back', read Table::menu()
std::string Entry::menu(const std::string &name, int theme, std::string &input, std::string &info, const std::vector<std::string> &color_icons) {
	switch (this->type) {
		case APPLY: // no further parsing required, option needs to be set. maybe see if string is "" for safety?
			this->active_theme = theme;
			return print_option(name, info + "/" + name, color_icons);
			break;
		case APPLY_LIST:
			this->active_theme = theme;
			return print_option(name, info + "/" + name, color_icons);
			break;
		case SUB: // need to go into subtable
			{
				std::string why_is_this_needed = info + "/" + name;
				std::string back = info + "/"; // 😭😭 I'm in way too deep to change this now, in the future might redo the whole info thing
				return std::get<SUB_DATA>(this->data).get()->menu(theme, input, why_is_this_needed, back, color_icons);
				// this->active_theme = ...
				break;
			}
		case LIST:
			return "list";
			break;
		case LIST_PICTURE:
			return "pics";
			break;

		default: // I will assume the type can never be anything else ever, but this way compiler shuts up
			return "error";
	}
}

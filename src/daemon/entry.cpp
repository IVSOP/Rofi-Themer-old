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

	while ((pos = line.find(';')) != std::string::npos) {
		token = line.substr(0, pos);
		line.erase(0, pos + 1); // 1 is len of delimiter
		// puts(token.c_str());
	}

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

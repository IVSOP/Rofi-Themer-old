#include "common.h"

#define INIT_ESCAPE '\0'
#define ESCAPE_SEP '\x1f'

std::string rofi_message(const std::string &text, const std::string &info) {
	return 
		text + INIT_ESCAPE +
		"info" + ESCAPE_SEP + info + "\n";
	// missing active[]!!!!!!!!!!!!!!!!!!
}

std::string rofi_message(const std::string &text, const std::string &icon, const std::string &info) {
	return 
		text + INIT_ESCAPE +
		"info" + ESCAPE_SEP + info + ESCAPE_SEP +
		"icon" + ESCAPE_SEP + icon + "\n";
	// missing active[]!!!!!!!!!!!!!!!!!!
}

std::string print_back(const std::string &info) {
	const std::string str = "Back";
	return rofi_message(str, info);
}

std::string print_all(const std::string &info) {
	const std::string str = "All";
	return rofi_message(str, info + "/*");
}

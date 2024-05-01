#include "files.h"
#include "errors.h"

FileHandler::FileHandler(const std::string &path)
: file(path)
{
	if (!file.is_open()) {
		print_error("Error opening file");
		exit(1);
    }
}

FileHandler::~FileHandler() {
	file.close();
}

bool FileHandler::getline(std::string &buffer) {
	buffer.clear();

	char byte;
    while (file.get(byte)) {
        if (byte == '\\') { // if \ found, ignore next char
			file.ignore(1);
		} else if (byte == '\n') { // if \n found, return success
			return true;
		} else { // else if something found, add it to the string
			buffer += byte;
		}
    }

	// last line may not end in \n and instead EOF
	// but error is only when it is completely empty
	if (buffer.size() > 0) {
		return true;
	} else {
		return false;
	}

}

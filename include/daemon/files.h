/*
FUTURE OPTIMIZATIONS


mmap()
memchr() to find the \n

use
#define likely __glibc_likely
#define unlikely __glibc_unlikely

*/
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <fstream>

struct FileHandler {
	FileHandler() = delete;
	FileHandler(const std::string &path);
	~FileHandler();

	// reads line, stores it into buffer. buffer is reset at the start. '\' makes it so that next character is ignored
	// no error checking is performed at all, other than returning false on EOF
	bool getline(std::string &buffer); // make it so that buffer is internal to this class????


	std::ifstream file;
};

#endif

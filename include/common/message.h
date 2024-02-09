#ifndef MESSAGE_H
#define MESSAGE_H

#include <string.h>

enum MESSAGE_TYPE {
	READ = 'r',
	MENU = 'm',
	SAVE = 'w',  // write
	EXIT = 'k',  // kill
	UNDEF = 'u'  // reserved for code below
};

struct Message {
	MESSAGE_TYPE type;
	char str[256];

	Message()
	: type(UNDEF) {
		memset(str, '\0', 256);
	}
	~Message() = default;
};

#endif

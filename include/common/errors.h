#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>

#define print_error(msg) printf("%s, %s, line %d:\n", __FILE__, __func__, __LINE__); perror(msg);

#endif

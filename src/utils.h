#ifndef INCLUDE_UTILS
#define INCLUDE_UTILS

#include <string>
#include <GL/glew.h>

namespace ta3 {

bool readText(std::string& text, const std::string& fileName);

void trim(char *s, int *start, size_t *len);

}

#endif

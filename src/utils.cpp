#include <fstream>
#include <sstream>
#include <cctype>
#include <cstring>
#include "log.h"
#include "utils.h"

namespace ta3 {

bool readText(std::string& text, const std::string& fileName)
{
    std::ifstream in;

    in.open(fileName);
    if(in.fail()) {
        LOG_ERROR("Failed to open file %s", fileName.c_str());
        return false;
    }

    std::stringstream ss;
    ss << in.rdbuf();
    text = ss.str();

    if(in.bad()) {
        LOG_ERROR("Error happened while reading file %s", fileName.c_str());
        return false;
    }

    return true;
}

void trim(char *s, int *start, size_t *len)
{
    int i;
    for(i = 0; s[i] != '\0'; ++i) {
        if(!isspace(s[i])) {
            break;
        }
    }

    *start = i;
    if(s[i] == '\0') {
        *len = 0;
        return;
    }

    int j;
    for(j = strlen(s) - 1; j > i; --j) {
        if(!isspace(s[j])) {
            break;
        }
    }

    *len = j - i + 1;
}

}

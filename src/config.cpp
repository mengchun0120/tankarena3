#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "log.h"
#include "utils.h"
#include "config.h"

namespace ta3 {

bool parseLine(char *line, char **name, char **value)
{
    char *eqPos = strchr(line, '=');
    if(eqPos == NULL) {
        LOG_ERROR("Invalid line. Must be of the form name = value");
        return false;
    }

    *eqPos = '\0';

    int start;
    size_t len;

    trim(line, &start, &len);
    if(len == 0) {
        LOG_ERROR("Name cannot be empty");
        return false;
    }

    *name = line + start;
    line[start + len] = '\0';

    eqPos++;
    trim(eqPos, &start, &len);
    *value = eqPos + start;
    eqPos[start + len] = '\0';

    return true;
}

int findConfigItem(const std::vector<Config::ConfigItem> items, const char* name)
{
    unsigned int itemCount = items.size();
    for(unsigned i = 0; i < itemCount; ++i) {
        if(strcmp(items[i].m_name, name) == 0) {
            return i;
        }
    }

    return -1;
}

void setValue(Config::ConfigItem& item, const char *value)
{
    switch(item.m_type) {
    case Config::TYPE_INT:
        *((int *)(item.m_mem)) = atoi(value);
        item.m_set = 1;
        break;
    case Config::TYPE_DOUBLE:
        *((double *)(item.m_mem)) = atof(value);
        item.m_set = 1;
        break;
    case Config::TYPE_STRING:
        *((std::string *)item.m_mem) = value;
        item.m_set = 1;
        break;
    }
}

bool validateConfigItems(const std::vector<Config::ConfigItem> items)
{
    unsigned itemCount = items.size();
    for(unsigned int i = 0; i < itemCount; ++i) {
        if(items[i].m_required && !items[i].m_set) {
            LOG_ERROR("Parameter %s is required, but not set", items[i].m_name);
            return false;
        }
    }

    return true;
}

Config::Config()
: m_width(0)
, m_height(0)
{

}

Config::~Config()
{

}

bool Config::load(const char* fileName)
{
    std::vector<ConfigItem> items = {
        { "width", TYPE_INT, &m_width, true, false },
        { "height", TYPE_INT, &m_height, true, false },
        { "simpleVertexShaderFile", TYPE_STRING, &m_simpleVertexShaderFile, true, false },
        { "simpleFragShaderFile", TYPE_STRING, &m_simpleFragShaderFile, true, false },
        { "title", TYPE_STRING, &m_title, true, false }
    };

    FILE *fp = fopen(fileName, "r");
    if(fp == NULL) {
        LOG_ERROR("Failed to open file %s", fileName);
        return false;
    }

    const size_t BUFFER_LEN = 1000;
    char *buffer = new char[BUFFER_LEN];
    bool retVal = true;
    size_t len = BUFFER_LEN;

    while(getline(&buffer, &len, fp) != -1) {
        char *name, *value;
        if(parseLine(buffer, &name, &value) == -1) {
            retVal = true;
            break;
        }

        LOG_INFO("Reading %s = %s", name, value);

        int i = findConfigItem(items, name);
        if(i == -1) {
            LOG_WARN("Unknown parameter name %s", name);
            continue;
        }

        setValue(items[i], value);
    }

    if(ferror(fp)) {
        LOG_ERROR("Failed to read file %s", fileName);
        retVal = false;
    } else if(!validateConfigItems(items)) {
        retVal = false;
    }

    delete[] buffer;
    fclose(fp);

    return retVal;
}

} // end of namespace ta3

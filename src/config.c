#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "config.h"
#include "log.h"

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

int parse_line(char *line, char **name, char **value)
{
    char *eq_pos = strchr(line, '=');
    if(eq_pos == NULL) {
        LOG_ERROR("Invalid line. Must be of the form name = value");
        return -1;
    }

    *eq_pos = '\0';

    int start;
    size_t len;

    trim(line, &start, &len);
    if(len == 0) {
        LOG_ERROR("Name cannot be empty");
        return -1;
    }

    *name = line + start;

    eq_pos++;
    trim(eq_pos, &start, &len);
    *value = eq_pos + start;

    return 0;
}

void init_config(Config *cfg)
{
    cfg->first = NULL;
}

int read_config(Config *cfg, const char *file)
{
    FILE *fp = fopen(file, "r");
    if(fp == NULL) {
        LOG_ERROR("Failed to open file %s", file);
        return -1;
    }

    size_t len = 1000;
    char *buffer = (char *)malloc(len);
    int retval = 0;

    while(getline(&buffer, &len, fp) != -1) {
        char *name, *value;
        if(parse_line(buffer, &name, &value) == -1) {
            retval = -1;
            break;
        }

        add_config(cfg, name, value);
    }

    if(ferror(fp)) {
        LOG_ERROR("Failed to read file %s", file);
        retval = -1;
    }

    free(buffer);
    fclose(fp);

    return retval;
}

void add_config(Config *cfg, const char *name, const char *value)
{
    ConfigItem *cfg_item = (ConfigItem *)malloc(sizeof(ConfigItem));
    cfg_item->name = (char *)malloc(strlen(name) + 1);
    strcpy(cfg_item->name, name);
    cfg_item->value = (char *)malloc(strlen(value) + 1);
    strcpy(cfg_item->value, value);
    cfg_item->next = cfg->first;
    cfg->first = cfg_item;
}

const char *find_config(Config *cfg, const char *name)
{
    ConfigItem *cfg_item;
    for(cfg_item = cfg->first; cfg_item != NULL; cfg_item = cfg_item->next) {
        if(strcmp(cfg_item->name, name) == 0) {
            break;
        }
    }
    return cfg_item != NULL ? cfg_item->value : NULL;
}

void destroy_config(Config *cfg)
{
    ConfigItem *cfg_item, *next_item;
    for(cfg_item = cfg->first; cfg_item != NULL; cfg_item = next_item) {
        next_item = cfg_item->next;
        free(cfg_item->name);
        free(cfg_item->value);
        free(cfg_item);
    }
    cfg->first = NULL;
}

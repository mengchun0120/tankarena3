#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
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
    line[start + len] = '\0';

    eq_pos++;
    trim(eq_pos, &start, &len);
    *value = eq_pos + start;
    eq_pos[start + len] = '\0';

    return 0;
}

int find_config_item(ConfigItem *items, int item_count, const char *name)
{
    int i = 0;
    for(i = 0; i < item_count; ++i) {
        if(strcmp(items[i].name, name) == 0) {
            break;
        }
    }

    return i < item_count ? i : -1;
}

void set_value(ConfigItem *item, const char *value)
{
    switch(item->type) {
    case INT:
        *((int *)(item->mem)) = atoi(value);
        item->set = 1;
        break;
    case DOUBLE:
        *((double *)(item->mem)) = atof(value);
        item->set = 1;
        break;
    case STRING: {
            char **str = (char **)(item->mem);
            *str = (char *)malloc(strlen(value) + 1);
            strcpy(*str, value);
            item->set = 1;
            break;
        }
    }
}

int validate_config_items(ConfigItem *items, int item_count)
{
    int i;
    for(i = 0; i < item_count; ++i) {
        if(items[i].required && !items[i].set) {
            LOG_ERROR("Parameter %s is required, but not set", items[i].name);
            return -1;
        }
    }

    return 0;
}

void init_config(Config *cfg)
{
    cfg->width = 0;
    cfg->height = 0;
    cfg->simple_vertex_shader_file = NULL;
    cfg->simple_frag_shader_file = NULL;
    cfg->title = NULL;
}

int read_config(Config *cfg, const char *file)
{
    ConfigItem items[] = {
        { "width", INT, &(cfg->width), 1, 0 },
        { "height", INT, &(cfg->height), 1, 0 },
        { "simple_vertex_shader_file", STRING, &(cfg->simple_vertex_shader_file), 1, 0 },
        { "simple_frag_shader_file", STRING, &(cfg->simple_frag_shader_file), 1, 0 },
        { "title", STRING, &(cfg->title), 1, 0 }
    };

    FILE *fp = fopen(file, "r");
    if(fp == NULL) {
        LOG_ERROR("Failed to open file %s", file);
        return -1;
    }

    size_t len = 1000;
    char *buffer = (char *)malloc(len);
    int retval = 0;
    int item_count = sizeof(items) / sizeof(ConfigItem);

    while(getline(&buffer, &len, fp) != -1) {
        char *name, *value;
        if(parse_line(buffer, &name, &value) == -1) {
            retval = -1;
            break;
        }

        LOG_INFO("Reading %s = %s", name, value);

        int i = find_config_item(items, item_count, name);
        if(i == -1) {
            LOG_WARN("Unknown parameter name %s", name);
            continue;
        }

        set_value(&items[i], value);
    }

    if(ferror(fp)) {
        LOG_ERROR("Failed to read file %s", file);
        retval = -1;
    } else if(validate_config_items(items, item_count) == -1) {
        retval = -1;
    }

    free(buffer);
    fclose(fp);

    return retval;
}

void destroy_config(Config *cfg)
{
    free(cfg->simple_vertex_shader_file);
    free(cfg->simple_frag_shader_file);
    free(cfg->title);
}

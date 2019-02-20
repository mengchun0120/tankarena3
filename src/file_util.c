#include <stdio.h>
#include <malloc.h>
#include "file_util.h"
#include "log.h"

char *read_text(const char *file_name)
{
    FILE *fp = fopen(file_name, "rb");
    if(fp == NULL) {
        LOG_ERROR("Failed to open file %s", file_name);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    char *buffer = (char *)malloc(size + 1);

    rewind(fp);
    fread(buffer, 1, size, fp);

    if(ferror(fp)) {
        LOG_ERROR("Failed to read from file %s", file_name);
        return NULL;
    }

    buffer[size] = '\0';

    return buffer;
}

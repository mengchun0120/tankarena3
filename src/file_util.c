#include <stdio.h>
#include <malloc.h>
#include "file_util.h"

char *read_text(const char *file_name)
{
    FILE *fp = fopen(file_name, "rb");
    if(fp == NULL) {
        return NULL;
    }

    long size = fseek(fp, 0, SEEK_END);
    char *buffer = (char *)malloc(size + 1);

    rewind(fp);
    fread(buffer, 1, size, fp);

    if(ferror(fp)) {
        return NULL;
    }

    buffer[size] = '\0';

    return buffer;
}

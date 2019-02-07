#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "test_map.h"

TestRecord *g_first_test = NULL;

void add_test(const char *test_name, TestFunc test_func)
{
    TestRecord *test = (TestRecord *)malloc(sizeof(TestRecord));

    test->test_name = (char *)malloc(strlen(test_name) + 1);
    strcpy(test->test_name, test_name);
    test->test_func = test_func;

    test->next = g_first_test;
    g_first_test = test;
}

TestFunc find_test(const char *test_name)
{
    TestRecord *test = g_first_test;
    for(; test != NULL; test = test->next) {
        if(strcmp(test->test_name, test_name) == 0) {
            break;
        }
    }

    return test->test_func;
}

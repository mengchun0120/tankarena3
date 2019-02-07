#ifndef INCLUDE_TEST_MAP
#define INCLUDE_TEST_MAP

typedef int (*TestFunc)();

typedef struct TestRecordType {
    char *test_name;
    TestFunc test_func;
    struct TestRecordType *next;
} TestRecord;

extern TestRecord *g_first_test;

void add_test(const char *test_name, TestFunc test_func);

TestFunc find_test(const char *test_name);

#endif

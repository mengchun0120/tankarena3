#ifndef INCLUDE_TEST_CONFIG
#define INCLUDE_TEST_CONFIG

typedef struct TestConfigType {
    char *vertex_shader_file;
    char *frag_shader_file;
    char *test_filter;
} TestConfig;

extern TestConfig g_test_cfg;

void testcfg_init(TestConfig *cfg);

#endif

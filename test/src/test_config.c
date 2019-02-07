#include <stdio.h>
#include "test_config.h"

TestConfig g_test_cfg;

void testcfg_init(TestConfig *cfg)
{
    cfg->vertex_shader_file = NULL;
    cfg->frag_shader_file = NULL;
    cfg->test_filter = NULL;
}

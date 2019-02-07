#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "test_config.h"
#include "test_map.h"
#include "test_opengl_util.h"

int parse_argument(int argc, char *argv[])
{
    int i = 1;
    while(i < argc) {
        if(strcmp(argv[i], "-v") == 0 && (i+1) < argc) {
            g_test_cfg.vertex_shader_file = argv[i+1];
            i += 2;

        } else if(strcmp(argv[i], "-f") == 0 && (i+1) < argc) {
            g_test_cfg.frag_shader_file = argv[i+1];
            i += 2;

        } else if(strcmp(argv[i], "-t") == 0 && (i+1) < argc) {
            g_test_cfg.test_filter = argv[i+1];
            i += 2;

        } else {
            fprintf(stderr, "Usage: %s -v vertex_shader_file -f frag_shader_file\n", argv[0]);
            return -1;
        }
    }

    return 0;
}

void init_test()
{
    add_test("test_build_program", test_build_program);
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return;
    }


    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return;
    }

}

int run_test()
{
    if(g_test_cfg.test_filter != NULL) {
        TestFunc test_func = find_test(g_test_cfg.test_filter);
        if(test_func != NULL) {
            return test_func();
        } else {
            fprintf(stderr, "Test %s not found\n", g_test_cfg.test_filter);
            return -1;
        }
    } else {
        int fail_count = 0;
        for(TestRecord *test = g_first_test; test != NULL; test = test->next) {
            int rc = test->test_func();
            if(rc != 0) {
                fprintf(stderr, "%s failed with rc=%d\n", test->test_name, rc);
                ++fail_count;
            } else {
                fprintf(stdout, "%s succeeded\n", test->test_name);
            }
        }
        return fail_count > 0 ? -1 : 0;
    }
}

int main(int argc, char *argv[])
{
    if(parse_argument(argc, argv) != 0) {
        exit(1);
    }

    init_test();
    exit(run_test());
}

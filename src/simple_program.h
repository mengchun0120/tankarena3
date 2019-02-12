#ifndef INCLUDE_SIMPLE_PROGRAM
#define INCLUDE_SIMPLE_PROGRAM

#include "program.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SimpleProgram {
    Program pg;
    GLint pos_loc;
    GLint color_loc;
} SimpleProgram;

int build_simple_program(SimpleProgram *simple_pg, const char *vertex_shader_file,
                        const char *frag_shader_file);

void destroy_simple_program(SimpleProgram *simple_pg);

#ifdef __cplusplus
}
#endif

#endif

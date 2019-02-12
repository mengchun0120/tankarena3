#ifndef INCLUDED_PROGRAM
#define INCLUDED_PROGRAM

#include <GL/glew.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ProgramType {
    GLuint program;
    GLuint vertex_shader;
    GLuint frag_shader;
} Program;

void reset_program(Program *pg);

int build_program(Program *pg, const char *vertex_shader_file, const char *frag_shader_file);

void destroy_program(Program *pg);

#ifdef __cplusplus
}
#endif


#endif

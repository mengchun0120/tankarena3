#ifndef INCLUDED_OPENGL_UTIL
#define INCLUDED_OPENGL_UTIL

#include <GL/glew.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ProgramType {
    GLuint program;
    GLuint vertex_shader;
    GLuint frag_shader;
} Program;

void pg_init(Program *pg);

void pg_build(Program *pg, const char *vertex_shader_file, const char *frag_shader_file);

void pg_destroy(Program *pg);

#ifdef __cplusplus
}
#endif


#endif

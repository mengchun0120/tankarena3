#include "log.h"
#include "simple_program.h"

int build_simple_program(SimpleProgram *simple_pg, const char *vertex_shader_file,
                        const char *frag_shader_file)
{
    if(build_program(&simple_pg->pg, vertex_shader_file, frag_shader_file) == -1) {
        LOG_ERROR("Failed to build simple program from source %s and %s",
                  vertex_shader_file, frag_shader_file);
        return -1;
    }

    GLuint program = simple_pg->pg.program;

    simple_pg->pos_loc = glGetAttribLocation(program, "pos");
    if(simple_pg->pos_loc == -1) {
        destroy_program(&simple_pg->pg);
        LOG_ERROR("Couldn't find attribute pos");
        return -1;
    }

    simple_pg->color_loc = glGetUniformLocation(program, "color");
    if(simple_pg->color_loc == -1) {
        destroy_program(&simple_pg->pg);
        LOG_ERROR("Couldn't find uniform color");
        return -1;
    }

    return 0;
}

void destroy_simple_program(SimpleProgram *simple_pg)
{
    destroy_program(&simple_pg->pg);
}

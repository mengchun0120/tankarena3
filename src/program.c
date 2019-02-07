#include <malloc.h>
#include <string.h>
#include <GL/glew.h>
#include "file_util.h"
#include "program.h"

GLuint compile_shader(GLenum type, const char *file)
{
    char *source = read_text(file);
    if(source == NULL) {
        return 0;
    }

    GLuint shader = glCreateShader(type);
    if(shader == 0) {
        free(source);
        return 0;
    }

    GLint source_len = strlen(source);
    glShaderSource(shader, 1, (const GLchar * const*)&source, &source_len);
    glCompileShader(shader);

    GLint compile_status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if(compile_status == 0) {
        glDeleteShader(shader);
        shader = 0;
    }

    free(source);
    return shader;
}

GLuint link_program(GLuint vertex_shader, GLuint frag_shader)
{
    GLuint program = glCreateProgram();
    if(program == 0) {
        return 0;
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    GLint link_status;

    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if(link_status == 0) {
        return 0;
    }

    return program;
}

void pg_init(Program *pg)
{
    pg->program = 0;
    pg->vertex_shader = 0;
    pg->frag_shader = 0;
}

void pg_build(Program *pg, const char *vertex_shader_file, const char *frag_shader_file)
{
    pg_init(pg);

    pg->vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_file);
    if(pg->vertex_shader == 0) {
        pg_destroy(pg);
        return;
    }

    pg->frag_shader = compile_shader(GL_FRAGMENT_SHADER, frag_shader_file);
    if(pg->frag_shader == 0) {
        pg_destroy(pg);
        return;
    }

    pg->program = link_program(pg->vertex_shader, pg->frag_shader);
    if(pg->program == 0) {
        pg_destroy(pg);
    }
}

void pg_destroy(Program *pg)
{
    if(pg->program != 0) {
        if(pg->vertex_shader != 0) {
            glDetachShader(pg->program, pg->vertex_shader);
        }
        if(pg->frag_shader != 0) {
            glDetachShader(pg->program, pg->frag_shader);
        }
    }

    if(pg->vertex_shader != 0) {
        glDeleteShader(pg->vertex_shader);
    }

    if(pg->frag_shader != 0) {
        glDeleteShader(pg->frag_shader);
    }

    if(pg->program != 0) {
        glDeleteProgram(pg->program);
    }

    pg_init(pg);
}

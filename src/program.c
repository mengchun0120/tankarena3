#include <malloc.h>
#include <string.h>
#include "file_util.h"
#include "program.h"
#include "log.h"

const char *shader_name(GLenum type) {
    switch(type) {
    case GL_VERTEX_SHADER:
        return "vertex shader";
    case GL_FRAGMENT_SHADER:
        return "fragment shader";
    }

    return "invalid shader";
}

GLuint compile_shader(GLenum type, const char *file)
{
    char *source = read_text(file);
    if(source == NULL) {
        LOG_ERROR("Failed to read text from %s", file);
        return 0;
    }

    LOG_INFO("Comiple %s using source:\n%s", shader_name(type), source);

    GLuint shader = glCreateShader(type);
    if(shader == 0) {
        LOG_ERROR("Failed to create shader %d", type);
        free(source);
        return 0;
    }

    GLint source_len = strlen(source);
    glShaderSource(shader, 1, (const GLchar * const*)&source, &source_len);
    glCompileShader(shader);

    GLint compile_status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if(compile_status == 0) {
        GLint info_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
        char *info = (char *)malloc(info_len + 1);

        glGetShaderInfoLog(shader, info_len, NULL, info);
        LOG_INFO("Results of compilation:\n%s", info);

        free(info);

        LOG_ERROR("Compilation failed");
        glDeleteShader(shader);
        shader = 0;
    } else {
        LOG_INFO("Compilation is successful");
    }

    free(source);
    return shader;
}

GLuint link_program(GLuint vertex_shader, GLuint frag_shader)
{
    GLuint program = glCreateProgram();
    if(program == 0) {
        LOG_ERROR("Failed to create program");
        return 0;
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    GLint link_status;

    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if(link_status == 0) {
        GLint info_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
        char *info = (char *)malloc(info_len + 1);

        glGetProgramInfoLog(program, info_len, NULL, info);
        LOG_INFO("Results of linking:\n%s", info);

        free(info);

        LOG_ERROR("Linking failed");
        return 0;
    } else {
        LOG_INFO("Linking is successful");
    }

    return program;
}

void reset_program(Program *pg)
{
    pg->program = 0;
    pg->vertex_shader = 0;
    pg->frag_shader = 0;
}

int build_program(Program *pg, const char *vertex_shader_file, const char *frag_shader_file)
{
    reset_program(pg);

    pg->vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_file);
    if(pg->vertex_shader == 0) {
        destroy_program(pg);
        return -1;
    }

    pg->frag_shader = compile_shader(GL_FRAGMENT_SHADER, frag_shader_file);
    if(pg->frag_shader == 0) {
        destroy_program(pg);
        return -1;
    }

    pg->program = link_program(pg->vertex_shader, pg->frag_shader);
    if(pg->program == 0) {
        destroy_program(pg);
        return -1;
    }

    return 0;
}

int load_shader_param(Program *pg, const ShaderParamConfig *param_cfg, unsigned int param_count)
{
    for(unsigned int i = 0; i < param_count; ++i) {
        GLint loc = -1;
        switch(param_cfg[i].type) {
        case UNIFORM_PARAM:
            loc = glGetUniformLocation(pg->program, param_cfg[i].name);
            break;
        case ATTRIB_PARAM:
            loc = glGetAttribLocation(pg->program, param_cfg[i].name);
            break;
        default:
            LOG_ERROR("Wrong param type %d", param_cfg[i].type);
            return -1;
        }

        if(loc == -1) {
            LOG_ERROR("Failed to find %s", param_cfg[i].name);
            return -1;
        }

        *(param_cfg[i].location) = loc;
    }

    return 0;
}

void destroy_program(Program *pg)
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

    reset_program(pg);
}

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

typedef enum ShaderParamTypeEnum {
    UNIFORM_PARAM,
    ATTRIB_PARAM
} ShaderParamType;

typedef struct ShaderParamConfigType {
    GLint *location;
    const char *name;
    ShaderParamType type;
} ShaderParamConfig;

void reset_program(Program *pg);

int build_program(Program *pg, const char *vertex_shader_file, const char *frag_shader_file);

int load_shader_param(Program *pg, const ShaderParamConfig *param_cfg, unsigned int param_count);

void destroy_program(Program *pg);

#ifdef __cplusplus
}
#endif


#endif

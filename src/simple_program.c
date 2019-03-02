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

    ShaderParamConfig param_cfg[] = {
        { &simple_pg->pos_loc, "pos", ATTRIB_PARAM },
        { &simple_pg->color_loc, "color", UNIFORM_PARAM },
        { &simple_pg->use_tex_loc, "useTex", UNIFORM_PARAM },
        { &simple_pg->tex_coord_loc, "texCoord", ATTRIB_PARAM },
        { &simple_pg->sampler_loc, "sampler", UNIFORM_PARAM }
    };

    if(-1 == load_shader_param(&simple_pg->pg, param_cfg, sizeof(param_cfg)/sizeof(ShaderParamConfig))) {
        LOG_ERROR("Failed to load parameters from shader");
        destroy_program(&simple_pg->pg);
        return -1;
    }

    return 0;
}

void destroy_simple_program(SimpleProgram *simple_pg)
{
    destroy_program(&simple_pg->pg);
}

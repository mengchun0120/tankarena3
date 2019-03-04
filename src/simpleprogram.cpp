#include "log.h"
#include "simpleprogram.h"

namespace ta3 {

SimpleProgram::SimpleProgram()
: ShaderProgram()
, m_posLocation(-1)
, m_colorLocation(-1)
, m_texCoordLocation(-1)
, m_useTexLocation(-1)
, m_samplerLocation(-1)
{
}

SimpleProgram::~SimpleProgram()
{
}

bool SimpleProgram::initParam()
{
    std::vector<ParamConfig> configs = {
        ParamConfig(&m_posLocation, "pos", PARAM_ATTRIB),
        ParamConfig(&m_colorLocation, "color", PARAM_UNIFORM),
        ParamConfig(&m_useTexLocation, "useTex", PARAM_UNIFORM),
        ParamConfig(&m_texCoordLocation, "texCoord", PARAM_ATTRIB),
        ParamConfig(&m_samplerLocation, "sampler", PARAM_UNIFORM)
    };

    if(!loadParam(configs)) {
        LOG_ERROR("Failed to load parameter from shader program");
        return false;
    }

    return true;
}

} // end of namespace ta3

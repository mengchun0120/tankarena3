#include "utils.h"
#include "log.h"
#include "shaderprogram.h"

namespace ta3 {

const char* ShaderProgram::shaderName(GLenum type)
{
    switch(type) {
    case GL_VERTEX_SHADER:
        return "vertex shader";
    case GL_FRAGMENT_SHADER:
        return "fragment shader";
    }

    return "invalid shader";
}

GLuint ShaderProgram::compileShader(GLenum type, const std::string& fileName)
{
    std::string source;

    if(!readText(source, fileName)) {
        return 0;
    }

    const GLchar* cSource = source.c_str();

    LOG_INFO("Comiple %s using source:\n%s", shaderName(type), cSource);

    GLuint shader = glCreateShader(type);
    if(shader == 0) {
        LOG_ERROR("Failed to create shader %d", type);
        return 0;
    }

    GLint len = source.length();

    glShaderSource(shader, 1, (const GLchar * const*)&cSource, &len);
    glCompileShader(shader);

    GLint compileStatus;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus == 0) {
        GLint infoLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        char* info = new char[infoLen + 1];

        glGetShaderInfoLog(shader, infoLen, NULL, info);
        LOG_INFO("Results of compilation:\n%s", info);

        delete[] info;

        LOG_ERROR("Compilation failed");
        glDeleteShader(shader);
        shader = 0;
    } else {
        LOG_INFO("Compilation is successful");
    }

    return shader;
}

GLuint ShaderProgram::linkProgram(GLuint vertex_shader, GLuint frag_shader)
{
    GLuint program = glCreateProgram();
    if(program == 0) {
        LOG_ERROR("Failed to create program");
        return 0;
    }

    glAttachShader(program, vertex_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);

    GLint linkStatus;

    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == 0) {
        GLint infoLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        char* info = new char[infoLen + 1];

        glGetProgramInfoLog(program, infoLen, NULL, info);
        LOG_INFO("Results of linking:\n%s", info);

        delete[] info;

        LOG_ERROR("Linking failed");
        return 0;
    } else {
        LOG_INFO("Linking is successful");
    }

    return program;
}

ShaderProgram::ShaderProgram()
: m_vertexShader(0)
, m_fragShader(0)
, m_program(0)
{
}

ShaderProgram::ShaderProgram(const std::string& vertexShaderFile,
                             const std::string& fragShaderFile)
: m_valid(false)
, m_vertexShader(0)
, m_fragShader(0)
, m_program(0)
{
    if(!build(vertexShaderFile, fragShaderFile)) {
        LOG_ERROR("Failed to build shader program");
    }
}

ShaderProgram::~ShaderProgram()
{
    destroy();
}

bool ShaderProgram::build(const std::string& vertexShaderFile,
                          const std::string& fragShaderFile)
{
    destroy();

    m_vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderFile);
    if(m_vertexShader == 0) {
        return false;
    }

    m_fragShader = compileShader(GL_FRAGMENT_SHADER, fragShaderFile);
    if(m_fragShader == 0) {
        return false;
    }

    m_program = linkProgram(m_vertexShader, m_fragShader);

    m_valid = (m_program != 0);

    return m_valid;
}

void ShaderProgram::destroy()
{
    if(m_program != 0) {
        glDetachShader(m_program, m_vertexShader);
        glDetachShader(m_program, m_fragShader);
    }

    if(m_vertexShader != 0) {
        glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
    }

    if(m_fragShader != 0) {
        glDeleteShader(m_fragShader);
        m_fragShader = 0;
    }

    if(m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
    }

    LOG_INFO("ShaderProgram destroyed");

    m_valid = false;
}

bool ShaderProgram::use()
{
    if(!m_valid) {
        LOG_ERROR("Couldn't use an invalid ShaderProgram");
        return false;
    }

    glUseProgram(m_program);

    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) {
        LOG_ERROR("glUseProgram failed: err=%d", err);
        return false;
    }

    return true;
}

bool ShaderProgram::loadParam(const std::vector<ParamConfig>& configs)
{
    if(!m_valid) {
        LOG_ERROR("Couldn't load param into an invalid ShaderProgram");
        return false;
    }

    unsigned int paramCount = configs.size();
    for(unsigned int i = 0; i < paramCount; ++i) {
        GLint loc = -1;
        const ParamConfig& cfg = configs[i];
        switch(cfg.m_type) {
        case PARAM_UNIFORM:
            loc = glGetUniformLocation(m_program, cfg.m_name);
            break;
        case PARAM_ATTRIB:
            loc = glGetAttribLocation(m_program, cfg.m_name);
            break;
        default:
            LOG_ERROR("Invalid param type %d", cfg.m_type);
            return false;
        }

        if(loc == -1) {
            LOG_ERROR("Failed to find %s", cfg.m_name);
            return false;
        }

        *(cfg.m_location) = loc;
    }

    return true;
}

} // end of namespace ta3

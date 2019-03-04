#ifndef INCLUDE_SHADERPROGRAM
#define INCLUDE_SHADERPROGRAM

#include <GL/glew.h>
#include <vector>
#include <string>

namespace ta3 {

class ShaderProgram {
public:
    enum ParamType {
        PARAM_UNIFORM,
        PARAM_ATTRIB,
        PARAM_INVALID
    };

    struct ParamConfig {
        GLint* m_location;
        const char* m_name;
        ParamType m_type;

        ParamConfig(GLint* loc, const char* name, ParamType type)
        : m_location(loc)
        , m_name(name)
        , m_type(type)
        {}
    };

    static const char* shaderName(GLenum type);

    static GLuint compileShader(GLenum type, const std::string& fileName);

    static GLuint linkProgram(GLuint vertex_shader, GLuint frag_shader);

    ShaderProgram();

    ShaderProgram(const std::string& vertexShaderFile,
                  const std::string& fragShaderFile);

    virtual ~ShaderProgram();

    bool build(const std::string& vertexShaderFile,
               const std::string& fragShaderFile);

    void destroy();

    GLuint program() const
    {
        return m_program;
    }

    GLuint vertexShader() const
    {
        return m_vertexShader;
    }

    GLuint fragShader() const
    {
        return m_fragShader;
    }

    bool valid() const
    {
        return m_valid;
    }

    bool use();

    bool loadParam(const std::vector<ParamConfig>& configs);

protected:
    GLuint m_valid;
    GLuint m_vertexShader;
    GLuint m_fragShader;
    GLuint m_program;
};

} // end of namespace ta3

#endif

#ifndef INCLUDE_SIMPLEPROGRAM
#define INCLUDE_SIMPLEPROGRAM

#include "vertexarray.h"
#include "texture.h"
#include "shaderprogram.h"

namespace ta3 {

class SimpleProgram: public ShaderProgram {
public:
    SimpleProgram();

    virtual ~SimpleProgram();

    bool initParam();

    bool draw(GLenum mode, const VertexArray& vertexArray, const Texture& texture);

    bool draw(GLenum mode, const VertexArray& vertexArray, const float* color);

protected:
    GLint m_posLocation;
    GLint m_colorLocation;
    GLint m_texCoordLocation;
    GLint m_useTexLocation;
    GLint m_samplerLocation;
};

} // end of namespace ta3

#endif

#ifndef INCLUDE_SIMPLEPROGRAM
#define INCLUDE_SIMPLEPROGRAM

#include "shaderprogram.h"

namespace ta3 {

class SimpleProgram: public ShaderProgram {
public:
    SimpleProgram();

    virtual ~SimpleProgram();

    bool initParam();

protected:
    GLint m_posLocation;
    GLint m_colorLocation;
    GLint m_texCoordLocation;
    GLint m_useTexLocation;
    GLint m_samplerLocation;
};

} // end of namespace ta3

#endif

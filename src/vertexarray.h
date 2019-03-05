#ifndef INCLUDE_VERTEXARRAY
#define INCLUDE_VERTEXARRAY

#include <GL/glew.h>

namespace ta3 {

class VertexArray {
public:
    VertexArray();

    VertexArray(const float*        vertexArray,
                unsigned int        vertexArraySize,
                bool                hasTexture=true,
                const unsigned int* indexArray=nullptr,
                unsigned int        indexArraySize=0);

    virtual ~VertexArray();

    bool load(const float*        vertexArray,
              unsigned int        vertexArraySize,
              bool                hasTexture=true,
              const unsigned int* indexArray=nullptr,
              unsigned int        indexArraySize=0);

    void destroy();

    bool valid() const
    {
        return m_vao != 0;
    }

    GLuint vao() const
    {
        return m_vao;
    }

    GLuint vbo() const
    {
        return m_vbo;
    }

    GLuint ebo() const
    {
        return m_ebo;
    }

    bool hasTexture() const
    {
        return m_hasTexture;
    }

protected:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    bool m_hasTexture;
};

} // end of namespace ta3

#endif

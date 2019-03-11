#ifndef INCLUDE_VERTEXARRAY
#define INCLUDE_VERTEXARRAY

#include <GL/glew.h>
#include "constants.h"

namespace ta3 {

class VertexArray {
public:
    static const unsigned int VERTEX_OFFSET = 0;
    static const unsigned int TEX_COORD_OFFSET = Constants::VERTEX_SIZE;

    VertexArray();

    VertexArray(const float*        vertexArray,
                unsigned int        numVertices,
                bool                containTexCoord=false,
                const unsigned int* indexArray=nullptr,
                unsigned int        numIndices=0);

    virtual ~VertexArray();

    bool load(const float*        vertexArray,
              unsigned int        numVertices,
              bool                containTexCoord=false,
              const unsigned int* indexArray=nullptr,
              unsigned int        numIndices=0);

    void destroy();

    bool valid() const
    {
        return m_vertexArray != 0;
    }

    GLuint vertexArray() const
    {
        return m_vertexArray;
    }

    unsigned int numVertices() const
    {
        return m_numVertices;
    }

    GLuint vertexBuffer() const
    {
        return m_vertexBuffer;
    }

    bool containTexCoord() const
    {
        return m_containTexCoord;
    }

    unsigned int numIndices() const
    {
        return m_numIndices;
    }

    GLuint indexBuffer() const
    {
        return m_indexBuffer;
    }

    unsigned int stride() const
    {
        return m_containTexCoord ? Constants::VERTEX_TEX_COORD_SIZE : Constants::VERTEX_SIZE;
    }

protected:
    void reset();

protected:
    GLuint m_vertexArray;
    unsigned int m_numVertices;
    GLuint m_vertexBuffer;
    bool m_containTexCoord;
    unsigned int m_numIndices;
    GLuint m_indexBuffer;
};

} // end of namespace ta3

#endif

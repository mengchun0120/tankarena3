#include "log.h"
#include "vertexarray.h"

namespace ta3 {

VertexArray::VertexArray()
{
    reset();
}

VertexArray::VertexArray(const float*        vertexArray,
                         unsigned int        numVertices,
                         bool                containTexCoord,
                         const unsigned int* indexArray,
                         unsigned int        numIndices)
{
    reset();
    load(vertexArray, numVertices, containTexCoord, indexArray, numIndices);
}

VertexArray::~VertexArray()
{
    if(valid()) {
        destroy();
    }
}

void VertexArray::reset()
{
    m_vertexArray = 0;
    m_vertexBuffer = 0;
    m_numVertices = 0;
    m_containTexCoord = false;
    m_indexBuffer = 0;
    m_numIndices = 0;
}

bool VertexArray::load(const float*        vertexArray,
                       unsigned int        numVertices,
                       bool                containTexCoord,
                       const unsigned int* indexArray,
                       unsigned int        numIndices)
{
    if(valid()) {
        LOG_WARN("Trying to reset valid VertexArray");
        destroy();
    }

    if(vertexArray == nullptr) {
        LOG_ERROR("vertexArray cannot be null");
        return false;
    }

    if(numVertices == 0) {
        LOG_ERROR("numVertices cannot be zero");
        return false;
    }

    glGenVertexArrays(1, &m_vertexArray);
    if(m_vertexArray == 0) {
        LOG_ERROR("Failed to generate vertex array: %d", glGetError());
        return false;
    }

    glGenBuffers(1, &m_vertexBuffer);
    if(m_vertexBuffer == 0) {
        LOG_ERROR("Failed to generate vertex buffer: %d", glGetError());
        destroy();
        return false;
    }

    if(indexArray != nullptr) {
        if(numIndices == 0) {
            LOG_ERROR("numIndices cannot be zero");
            destroy();
            return false;
        }

        glGenBuffers(1, &m_indexBuffer);
        if(m_indexBuffer == 0) {
            LOG_ERROR("Failed to generate EBO: %d", glGetError());
            destroy();
            return false;
        }
    }

    m_containTexCoord = containTexCoord;
    m_numVertices = numVertices;

    glBindVertexArray(m_vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * stride(), vertexArray, GL_STATIC_DRAW);

    if(indexArray != nullptr) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int),
                     indexArray, GL_STATIC_DRAW);
    }

    m_containTexCoord = containTexCoord;

    return true;
}

void VertexArray::destroy()
{
    if(m_indexBuffer != 0) {
        glDeleteBuffers(1, &m_indexBuffer);
    }

    if(m_vertexBuffer != 0) {
        glDeleteBuffers(1, &m_vertexBuffer);
    }

    if(m_vertexArray != 0) {
        glDeleteVertexArrays(1, &m_vertexArray);
    }

    reset();
}

} // end of namespace ta3

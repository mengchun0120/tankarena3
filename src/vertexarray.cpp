#include "log.h"
#include "vertexarray.h"

namespace ta3 {

VertexArray::VertexArray()
: m_vao(0)
, m_vbo(0)
, m_ebo(0)
, m_hasTexture(false)
{
}

VertexArray::VertexArray(const float*        vertexArray,
                         unsigned int        vertexArraySize,
                         bool                hasTexture,
                         const unsigned int* indexArray,
                         unsigned int        indexArraySize)
: m_vao(0)
, m_vbo(0)
, m_ebo(0)
, m_hasTexture(hasTexture)
{
    load(vertexArray, vertexArraySize, hasTexture, indexArray, indexArraySize);
}

VertexArray::~VertexArray()
{
    if(valid()) {
        destroy();
    }
}

bool VertexArray::load(const float*        vertexArray,
                       unsigned int        vertexArraySize,
                       bool                hasTexture,
                       const unsigned int* indexArray,
                       unsigned int        indexArraySize)
{
    if(valid()) {
        LOG_WARN("Trying to reset valid VertexArray");
        destroy();
    }

    if(vertexArray == nullptr) {
        LOG_ERROR("vertexArray cannot be null");
        return false;
    }

    if(vertexArraySize == 0) {
        LOG_ERROR("vertexArraySize cannot be zero");
        return false;
    }

    glGenVertexArrays(1, &m_vao);
    if(m_vao == 0) {
        LOG_ERROR("Failed to generate VAO: %d", glGetError());
        return false;
    }

    glGenBuffers(1, &m_vbo);
    if(m_vbo == 0) {
        LOG_ERROR("Failed to generate VBO: %d", glGetError());
        destroy();
        return false;
    }

    if(indexArray != nullptr) {
        if(indexArraySize == 0) {
            LOG_ERROR("indexArraySize cannot be zero");
            destroy();
            return false;
        }

        glGenBuffers(1, &m_ebo);
        if(m_ebo == 0) {
            LOG_ERROR("Failed to generate EBO: %d", glGetError());
            destroy();
            return false;
        }
    }

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexArraySize, vertexArray, GL_STATIC_DRAW);

    if(indexArray != nullptr) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize, indexArray, GL_STATIC_DRAW);
    }

    m_hasTexture = hasTexture;

    return true;
}

void VertexArray::destroy()
{
    if(m_ebo != 0) {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

    if(m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

    if(m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}

} // end of namespace ta3

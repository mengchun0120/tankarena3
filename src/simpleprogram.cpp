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

bool SimpleProgram::draw(GLenum mode, const VertexArray& vertexArray, const Texture& texture)
{
    if(!vertexArray.valid()) {
        LOG_ERROR("vertexArray is invalid");
        return false;
    }

    if(!vertexArray.containTexCoord()) {
        LOG_ERROR("vertexArray doesn't contain texture coordinates");
        return false;
    }

    glUniform1i(m_useTexLocation, 1);
    glUniform1i(m_samplerLocation, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.textureId());

    unsigned int stride = vertexArray.stride();

    glBindVertexArray(vertexArray.vertexArray());
    glVertexAttribPointer(m_posLocation, Constants::VERTEX_SIZE, GL_FLOAT,
                          GL_FALSE, stride, (void*)VertexArray::VERTEX_OFFSET);
    glEnableVertexAttribArray(m_posLocation);

    glVertexAttribPointer(m_texCoordLocation, Constants::VERTEX_SIZE, GL_FLOAT,
                          GL_FALSE, stride, (void*)VertexArray::TEX_COORD_OFFSET);
    glEnableVertexAttribArray(m_texCoordLocation);

    if(vertexArray.indexBuffer() != 0) {
        glDrawElements(mode, vertexArray.numIndices(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(mode, 0, vertexArray.numVertices());
    }

    return true;
}

bool SimpleProgram::draw(GLenum mode, const VertexArray& vertexArray, const float* color)
{
    if(!vertexArray.valid()) {
        LOG_ERROR("vertexArray is invalid");
        return false;
    }

    glUniform1i(m_useTexLocation, 0);
    glUniform1fv(m_colorLocation, 1, color);

    glBindVertexArray(vertexArray.vertexArray());
    glVertexAttribPointer(m_posLocation, Constants::VERTEX_SIZE, GL_FLOAT,
                          GL_FALSE, vertexArray.stride(), (void*)VertexArray::VERTEX_OFFSET);
    glEnableVertexAttribArray(m_posLocation);

    if(vertexArray.indexBuffer() != 0) {
        glDrawElements(mode, vertexArray.numIndices(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(mode, 0, vertexArray.numVertices());
    }

    return true;
}

} // end of namespace ta3

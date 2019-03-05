#include "log.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace ta3 {

Texture::Texture()
: m_texture(0)
, m_width(0)
, m_height(0)
{

}

Texture::Texture(const std::string& fileName)
: m_texture(0)
, m_width(0)
, m_height(0)
{
    load(fileName);
}

Texture::~Texture()
{
    if(m_texture != 0) {
        destroy();
    }
}

bool Texture::load(const std::string& fileName)
{
    if(m_texture != 0) {
        LOG_WARN("Trying to reset valid texture");
        destroy();
    }

    glGenTextures(1, &m_texture);
    if(m_texture == 0) {
        LOG_ERROR("Failed to generate texture: %d", glGetError());
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    stbi_set_flip_vertically_on_load(1);

    int nrChannels;
    const char* cFileName = fileName.c_str();

    unsigned char *data = stbi_load(cFileName, &m_width, &m_height, &nrChannels, 0);
    if (data == nullptr) {
        LOG_ERROR("Failed to load texture from %s", cFileName);
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    LOG_DEBUG("Texture %d loaded successfully from %s", m_texture, cFileName);

    return true;
}

void Texture::destroy()
{
    if(m_texture == 0) {
        LOG_WARN("Trying to destroy invalid texture");
        return;
    }

    glDeleteTextures(1, &m_texture);
    m_texture = 0;

    LOG_DEBUG("Texture %d destroyed", m_texture);
}

} // end of namespace ta3

#ifndef INCLUDE_TEXTURE
#define INCLUDE_TEXTURE

#include <GL/glew.h>
#include <string>

namespace ta3 {

class Texture {
public:
    Texture();

    Texture(const std::string& fileName);

    virtual ~Texture();

    void destroy();

    bool load(const std::string& fileName);

    bool valid() const
    {
        return m_texture != 0;
    }

    GLuint texture() const
    {
        return m_texture;
    }

    int width() const
    {
        return m_width;
    }

    int height() const
    {
        return m_height;
    }

private:
    GLuint m_texture;
    int m_width;
    int m_height;
};

} // end of namespace ta3

#endif

#include "vertex_buffer.h"
#include "log.h"

GLuint create_vertex_buffer(const GLfloat *vertex_data, size_t data_size)
{
    GLuint buffer_id;

    glGenBuffers(1, &buffer_id);
    if(buffer_id == 0) {
        LOG_ERROR("Failed to generate vertex buffer");
        return 0;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, data_size, vertex_data, GL_STATIC_DRAW);

    GLint actual_size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &actual_size);
    if(data_size != actual_size) {
        LOG_ERROR("Failed to create vertex buffer: actual_size=%d data_size=%d", actual_size, data_size);
        glDeleteBuffers(1, &buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return 0;
    }

    return buffer_id;
}

void set_vertex_buffer_pointer(GLuint buffer_id, GLuint index, GLint size, GLsizei stride)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, 0);
    LOG_INFO("A %d", glGetError())
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

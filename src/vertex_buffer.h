#ifndef INCLUDE_VERTEX_BUFFER
#define INCLUDE_VERTEX_BUFFER

#include <GL/glew.h>

GLuint create_vertex_buffer(const GLfloat *vertex_data, size_t num_floats);

void set_vertex_buffer_pointer(GLuint buffer_id, GLuint index, GLint size, GLsizei stride);

#endif

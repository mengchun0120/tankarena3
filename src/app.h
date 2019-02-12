#ifndef INCLUDE_APP
#define INCLUDE_APP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "simple_program.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AppType {
    GLFWwindow* window;
    SimpleProgram simple_pg;
} App;

#ifdef __cplusplus
}
#endif

#endif

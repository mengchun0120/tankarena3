#ifndef INCLUDE_APP
#define INCLUDE_APP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "simple_program.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AppType {
    GLFWwindow* window;
    SimpleProgram simple_pg;
} App;

int init_app(App *app, Config *cfg);

int run_app(App *app);

void destroy_app(App *app);

#ifdef __cplusplus
}
#endif

#endif

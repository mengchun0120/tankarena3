#include "app.h"
#include "config.h"
#include "log.h"

int init_window(App *app, Config *cfg)
{
    LOG_INFO("Initializing window");

    if(!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    app->window = glfwCreateWindow(cfg->width, cfg->height, cfg->title, NULL, NULL);
    if(app->window == NULL){
        LOG_ERROR("Failed to open GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(app->window);

    if(glewInit() != GLEW_OK) {
        LOG_ERROR("Failed to initialize GLEW");
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(app->window, GLFW_STICKY_KEYS, GL_TRUE);

    return 0;
}

int init_app(App *app, Config *cfg)
{
    LOG_INFO("Initializing app");

    if(init_window(app, cfg) == -1) {
        LOG_ERROR("Failed to initilaize window context");
        return -1;
    }

    if(build_simple_program(&(app->simple_pg), cfg->simple_vertex_shader_file, cfg->simple_frag_shader_file) == -1) {
        LOG_ERROR("Failed to build SimpleProgram");
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    return 0;
}

int run_app(App *app)
{
    do{
        glClear( GL_COLOR_BUFFER_BIT );


        // Swap buffers
        glfwSwapBuffers(app->window);
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(app->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(app->window) == 0 );

    return 0;
}

void destroy_app(App *app)
{
    destroy_simple_program(&app->simple_pg);
    glfwTerminate();
}

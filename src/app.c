#include "app.h"
#include "config.h"
#include "log.h"
#include "vertex_buffer.h"

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

    glUseProgram(app->simple_pg.pg.program);
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

int run_app(App *app)
{
    GLfloat vertex[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);

    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(vao);

    GLuint buffer_id = create_vertex_buffer(vertex, sizeof(vertex));
    set_vertex_buffer_pointer(buffer_id, app->simple_pg.pos_loc, 2, 0);

    glUniform4f(app->simple_pg.color_loc, 1.0f, 0.0f, 0.0f, 1.0f);
    LOG_INFO("dd %d", glGetError());

    do{
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        LOG_INFO("ff %d", glGetError());

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

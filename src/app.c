#include "app.h"
#include "config.h"
#include "log.h"
#include "texture_util.h"

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

    glewExperimental = GL_TRUE;
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
    float vertices[] = {
        // positions  // texture coords
         0.5f,  0.5f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(app->simple_pg.pos_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(app->simple_pg.pos_loc);
    // texture coord attribute
    glVertexAttribPointer(app->simple_pg.tex_coord_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(app->simple_pg.tex_coord_loc);

    glUniform1i(app->simple_pg.sampler_loc, 0);
    glUniform1i(app->simple_pg.use_tex_loc, 1);

    int width, height;
    GLuint texture = load_texture("base0.png", &width, &height);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    do{
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

#ifndef INCLUDE_APP
#define INCLUDE_APP

#include "simpleprogram.h"
#include "config.h"
#include <GLFW/glfw3.h>

namespace ta3 {

class App {
public:
    static App& getSingleton();

    static bool initSingleton(const Config& cfg);

    ~App();

    bool init(const Config& cfg);

    bool run();

private:
    App();

    bool initWindow(const Config& cfg);

    bool initOpenGL();

private:
    GLFWwindow* m_window;
    SimpleProgram m_program;
};

} // end of namespace ta3

#endif

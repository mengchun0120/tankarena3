#include <cstdlib>
#include <iostream>
#include "log.h"
#include "app.h"
#include "config.h"

int main(int argc, char *argv[])
{
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " configFile logFile" << std::endl;
        return 1;
    }

    #ifdef ENABLE_LOG
    if(!ta3::Logger::initSingleton(argv[2], ta3::Logger::LEVEL_DEBUG)) {
        std::cerr << "Failed to setup log" << std::endl;
        return 1;
    }
    #endif

    ta3::Config cfg;
    if(!cfg.load(argv[1])) {
        LOG_ERROR("Failed to read config from %s", argv[1]);
        return 1;
    }

    if(!ta3::App::initSingleton(cfg)) {
        LOG_ERROR("Failed to initialize App");
        return 1;
    }

    ta3::App& app = ta3::App::getSingleton();
    if(!app.run()) {
        LOG_ERROR("Something wrong happened while running app");
        return 1;
    }
}

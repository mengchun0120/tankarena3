#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "app.h"
#include "config.h"

int main(int argc, char *argv[])
{
    if(argc != 3) {
        fprintf(stderr, "Usage: %s config_file log_file", argv[0]);
        exit(1);
    }

    #ifdef ENABLE_LOG
    init_log(&g_logger);
    if(setup_log(&g_logger, argv[2], LEVEL_DEBUG) == -1) {
        fprintf(stderr, "Failed to setup log");
        exit(1);
    }
    #endif

    Config cfg;
    App app;
    int app_ok = 0;
    int rc = 0;

    init_config(&cfg);
    if(read_config(&cfg, argv[1]) == -1) {
        fprintf(stderr, "Failed to read config from %s", argv[1]);
        exit(1);
    }

    if(init_app(&app, &cfg) == -1) {
        LOG_ERROR("Failed to initialize App");
        rc = 1;
        goto exit_main;
    }

    app_ok = 1;

    if(run_app(&app) == -1) {
        LOG_ERROR("Something wrong happened while running app");
        rc = 1;
    }

exit_main:
    if(app_ok) {
        destroy_app(&app);
    }

    #ifdef ENABLE_LOG
    close_log(&g_logger);
    #endif

    return rc;
}

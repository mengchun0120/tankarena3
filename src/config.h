#ifndef INCLUDE_CONFIG
#define INCLUDE_CONFIG

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemType {
    char *name;
    char *value;
    struct ConfigItemType *next;
} ConfigItem;

typedef struct ConfigType {
    ConfigItem *first;
} Config;

void init_config(Config *cfg);

int read_config(Config *cfg, const char *file);

void add_config(Config *cfg, const char *name, const char *value);

const char *find_config(Config *cfg, const char *name);

void destroy_config(Config *cfg);

#ifdef __cplusplus
}
#endif


#endif

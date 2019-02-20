#ifndef INCLUDE_CONFIG
#define INCLUDE_CONFIG

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ConfigValueTypeEnum {
    INT,
    STRING,
    DOUBLE
} ConfigValueType;

typedef struct ConfigItemStruct {
    const char *name;
    ConfigValueType type;
    void *mem;
    int required;
    int set;
} ConfigItem;

typedef struct ConfigType {
    int width;
    int height;
    char *simple_vertex_shader_file;
    char *simple_frag_shader_file;
    char *title;
} Config;

void init_config(Config *cfg);

int read_config(Config *cfg, const char *file);

void destroy_config(Config *cfg);

#ifdef __cplusplus
}
#endif


#endif

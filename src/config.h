#ifndef INCLUDE_CONFIG
#define INCLUDE_CONFIG

#include <string>

namespace ta3 {

class Config {
public:
    enum ConfigValueType {
        TYPE_INT,
        TYPE_STRING,
        TYPE_DOUBLE
    };

    struct ConfigItem {
        const char* m_name;
        ConfigValueType m_type;
        void* m_mem;
        bool m_required;
        bool m_set;
    };

public:
    Config();

    ~Config();

    bool load(const char* fileName);

public:
    int m_width;
    int m_height;
    std::string m_simpleVertexShaderFile;
    std::string m_simpleFragShaderFile;
    std::string m_title;
};

} // end of namespace ta3

#endif

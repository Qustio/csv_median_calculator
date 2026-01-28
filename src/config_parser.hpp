#ifndef CONFIG_PARSER
#define CONFIG_PARSER

#include <string>
#include "app_config.hpp"

class ConfigParser {
public:
    static AppConfig load(const std::string& path);
};

#endif // APP_CONFIG

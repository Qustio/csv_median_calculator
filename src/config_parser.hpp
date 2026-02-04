#ifndef CONFIG_PARSER
#define CONFIG_PARSER

#include <string>
#include "app_config.hpp"

class config_parser {
public:
	static app_config load(const std::string &path_);
};

#endif // CONFIG_PARSER

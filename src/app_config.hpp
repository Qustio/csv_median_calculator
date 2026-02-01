#ifndef APP_CONFIG
#define APP_CONFIG

#include <string>
#include <vector>

struct AppConfig {
	std::string input;
	std::string output;
    std::vector<std::string> filename_mask;
};

#endif // APP_CONFIG

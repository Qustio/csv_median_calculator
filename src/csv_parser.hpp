#ifndef CSV_PARSER
#define CSV_PARSER

#include <set>
#include <filesystem>
#include <vector>
#include "app_config.hpp"
#include "price_timestemp_data.hpp"

namespace fs = std::filesystem;

class CsvParser {
public:
	CsvParser(AppConfig app_config_);
	void find_files();
	void read_data();
	[[nodiscard]]
	std::span<const PriceTimestempData> get_timestamps() const {
        return _timestamps;
    }
private:
	AppConfig _config;
	std::set<fs::path> _input_files;
	std::vector<PriceTimestempData> _timestamps;
};

#endif // CSV_PARSER

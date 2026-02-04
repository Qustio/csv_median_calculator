#ifndef CSV_PARSER
#define CSV_PARSER

#include <cstddef>
#include <queue>
#include <filesystem>
#include <vector>
#include <generator>
#include "app_config.hpp"
#include "price_timestemp_data.hpp"
#include "rapidcsv.h"

namespace fs = std::filesystem;

class CsvParser {
public:
	CsvParser(AppConfig app_config_);
	std::generator<PriceTimestempData> read_data();
private:
	class CsvFileReader {
	public:
		CsvFileReader(const fs::path& path_);
		void read_next();
		[[nodiscard]] bool has_more() const;
        [[nodiscard]] PriceTimestempData last_value() const;
        bool operator>(const CsvFileReader &other_) const;
    private:
        rapidcsv::Document _file;
        fs::path _path;
        size_t _current_row = 0;
        size_t _total_rows = 0;
        PriceTimestempData _last_value = {};
	};
	AppConfig _config;
	size_t count = 0;
	std::priority_queue<CsvFileReader, std::vector<CsvFileReader>, std::greater<>> _input_files;
};



#endif // CSV_PARSER

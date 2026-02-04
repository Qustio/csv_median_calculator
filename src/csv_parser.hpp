#ifndef CSV_PARSER
#define CSV_PARSER

#include <cstddef>
#include <filesystem>
#include <generator>
#include <queue>
#include <vector>
#include "app_config.hpp"
#include "price_timestemp_data.hpp"
#include "rapidcsv.h"

namespace fs = std::filesystem;

class csv_parser {
public:
	csv_parser(app_config app_config_);
	std::generator<price_timestemp_data> read_data();

private:
	class csv_file_reader {
	public:
		csv_file_reader(const fs::path &path_);
		void read_next();
		[[nodiscard]] bool has_more() const;
		[[nodiscard]] price_timestemp_data last_value() const;
		bool operator>(const csv_file_reader &other_) const;

	private:
		rapidcsv::Document _file;
		fs::path _path;
		size_t _current_row = 0;
		size_t _total_rows = 0;
		price_timestemp_data _last_value = {};
	};

	app_config _config;
	size_t _count = 0;
	std::priority_queue<
		csv_file_reader,
		std::vector<csv_file_reader>,
		std::greater<>>
		_input_files;
};

#endif // CSV_PARSER

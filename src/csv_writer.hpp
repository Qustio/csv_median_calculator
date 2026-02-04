#ifndef CSV_WRITER
#define CSV_WRITER

#include <filesystem>
#include "rapidcsv.h"

namespace fs = std::filesystem;

class csv_writer {
public:
	csv_writer(const fs::path &output_path_);
	csv_writer(const csv_writer &) = delete;
	csv_writer &operator=(const csv_writer &) = delete;
	csv_writer(csv_writer &&) noexcept = default;
	csv_writer &operator=(csv_writer &&) noexcept = default;
	~csv_writer();

	void write_median_change(long long int timestamp_, double median_);

private:
	void write_header();

	rapidcsv::Document _file;
	fs::path _path;
	size_t _current_row = 0;
};

#endif // CSV_WRITER

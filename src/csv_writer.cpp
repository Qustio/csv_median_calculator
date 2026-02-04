#include "csv_writer.hpp"
#include <spdlog/spdlog.h>

csv_writer::csv_writer(const fs::path &output_path_) :
	_path(output_path_),
	_file(
		output_path_,
		rapidcsv::LabelParams(0, -1),
		rapidcsv::SeparatorParams(';')
	) {
	if (!fs::exists(output_path_)) {
		throw std::runtime_error(
			"Failed to open output file: " + output_path_.string()
		);
	}
	write_header();
	spdlog::info("Создан файл вывода: {}", output_path_.string());
}

csv_writer::~csv_writer() { _file.Save(); }

void csv_writer::write_header() {
	_file.Clear();
	_file.SetColumnName(0, "receive_ts");
	_file.SetColumnName(1, "median");
}

void csv_writer::write_median_change(long long int timestamp_, double median_) {
	_file.SetCell(0, _current_row, timestamp_);
	_file.SetCell(1, _current_row, median_);
	_current_row++;
}

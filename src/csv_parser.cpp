#include "csv_parser.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>
#include "price_timestemp_data.hpp"
#include "spdlog/fmt/bundled/format.h"

csv_parser::csv_parser(app_config app_config_) :
	_config(std::move(app_config_)) {
	spdlog::info(fmt::format("Входная директория: {}", _config.input));
	spdlog::info(fmt::format("Выходная директория: {}", _config.output));
	if (!std::filesystem::exists(_config.input)) {
		spdlog::critical(
			fmt::format("Директория \"{}\" не найдена", _config.input)
		);
		return;
	}
	for (const auto &entry :
		 std::filesystem::directory_iterator(_config.input)) {
		if (!std::filesystem::is_regular_file(entry.path())) {
			continue;
		}
		const auto stem = entry.path().stem().string();
		for (const auto &mask : _config.filename_mask) {
			if (stem.find(mask) != std::string::npos) {
				spdlog::info(fmt::format(" - {}", entry.path().string()));
				_input_files.emplace(entry.path());
			}
		}
	}
	spdlog::info("Найдено файлов: {}", _input_files.size());
};

std::generator<price_timestemp_data> csv_parser::read_data() {
	while (!_input_files.empty()) {
		auto reader = _input_files.top();
		_input_files.pop();
		co_yield reader.last_value();
		try {
			if (reader.has_more()) {
				reader.read_next();
				_input_files.push(reader);
			}
		} catch (const std::exception &e) {
			spdlog::critical(fmt::format("{}", e.what()));
		}
	}
	spdlog::info(fmt::format("Прочитано записей: {}", _count));
}

csv_parser::csv_file_reader::csv_file_reader(const fs::path &path_) :
	_file(path_, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(';')),
	_path(path_),
	_total_rows(_file.GetRowCount()) {
	if (_total_rows > 0) {
		read_next();
	}
}

void csv_parser::csv_file_reader::read_next() {
	SPDLOG_DEBUG("Чтение файла {}", _path.string());
	_last_value = price_timestemp_data{
		.timestemp = _file.GetCell<long long int>("receive_ts", _current_row),
		.price = _file.GetCell<double>("price", _current_row),
	};
	_current_row++;
}

[[nodiscard]]
bool csv_parser::csv_file_reader::has_more() const {
	return _current_row < _total_rows;
}

[[nodiscard]]
price_timestemp_data csv_parser::csv_file_reader::last_value() const {
	return _last_value;
}

bool
csv_parser::csv_file_reader::operator>(const csv_file_reader &other_) const {
	return _last_value.timestemp > other_._last_value.timestemp;
}

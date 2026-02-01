#include <filesystem>
#include <spdlog/spdlog.h>
#include "csv_parser.hpp"
#include "rapidcsv.h"

CsvParser::CsvParser(AppConfig app_config_)
    : _config(std::move(app_config_)) {
		spdlog::info(fmt::format("Входная директория: {}", _config.input));
		spdlog::info(fmt::format("Выходная директория: {}", _config.output));
	};

void CsvParser::find_files() {
	if (!std::filesystem::exists(_config.input)) {
		spdlog::critical(fmt::format("Директория \"{}\" не найдена", _config.input));
		return;
	}
	for (const auto& entry : std::filesystem::directory_iterator(_config.input)) {
		if (!std::filesystem::is_regular_file(entry.path())) {
			continue;
		}
		const auto stem = entry.path().stem().string();
		for (const auto& mask : _config.filename_mask) {
			if (stem.find(mask) != std::string::npos) {
				_input_files.insert(entry.path());
			}
		}
	}
	spdlog::info("Найдено файлов: {}", _input_files.size());
	for (const auto& input_file: _input_files) {
		spdlog::info(fmt::format(" - {}", input_file.string()));
	}
}
void CsvParser::read_data() {
	for (const auto& input_file: _input_files) {
		SPDLOG_DEBUG("Чтение файла {}", input_file.string());
		rapidcsv::Document doc(
			input_file,
			rapidcsv::LabelParams(0, -1),
			rapidcsv::SeparatorParams(';')
		);
		const auto row_count = doc.GetRowCount();
		_timestamps.reserve(_timestamps.size() + row_count);
		for (size_t index = 0; index < row_count; ++index) {
			PriceTimestempData timestemp{
				.timestemp = doc.GetCell<long long int>("receive_ts", index),
				.price = doc.GetCell<double>("price", index),
			};
			_timestamps.push_back(timestemp);
		}
		SPDLOG_DEBUG("Прочитано {} записей из {}", row_count, input_file.string());
	}
	spdlog::info(fmt::format("Прочитано записей: {}", _timestamps.size()));
}
#include <filesystem>
#include <spdlog/spdlog.h>
#include "csv_parser.hpp"
#include "price_timestemp_data.hpp"
#include "spdlog/fmt/bundled/format.h"

CsvParser::CsvParser(AppConfig app_config_)
: _config(std::move(app_config_)) {
	spdlog::info(fmt::format("Входная директория: {}", this->_config.input));
	spdlog::info(fmt::format("Выходная директория: {}", this->_config.output));
	if (!std::filesystem::exists(this->_config.input)) {
		spdlog::critical(fmt::format("Директория \"{}\" не найдена", this->_config.input));
		return;
	}
	for (const auto& entry : std::filesystem::directory_iterator(this->_config.input)) {
		if (!std::filesystem::is_regular_file(entry.path())) {
			continue;
		}
		const auto stem = entry.path().stem().string();
		for (const auto& mask : this->_config.filename_mask) {
			if (stem.find(mask) != std::string::npos) {
				spdlog::info(fmt::format(" - {}", entry.path().string()));
				this->_input_files.emplace(entry.path());
			}
		}
	}
	spdlog::info("Найдено файлов: {}", this->_input_files.size());
};

std::generator<PriceTimestempData> CsvParser::read_data() {
	while (!this->_input_files.empty()) {
		auto reader = _input_files.top();
		_input_files.pop();
		co_yield reader.last_value();
		try {
			if (reader.has_more()) {
				reader.read_next();
				this->_input_files.push(reader);
			}
        } catch (const std::exception& e) {
			spdlog::critical(fmt::format("{}", e.what()));
        }
	}
	spdlog::info(fmt::format("Прочитано записей: {}", this->count));
}

CsvParser::CsvFileReader::CsvFileReader(const fs::path& path_)
: _file(path_,rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(';')),
  _path(path_),
  _total_rows(_file.GetRowCount()) {
	if (this->_total_rows > 0){
		read_next();
	}
}
void CsvParser::CsvFileReader::read_next() {
	SPDLOG_DEBUG("Чтение файла {}", this->_path.string());
	this->_last_value = PriceTimestempData{
		.timestemp =
			this->_file.GetCell<long long int>("receive_ts", this->_current_row),
		.price = this->_file.GetCell<double>("price", this->_current_row),
	};
	this->_current_row++;
}
[[nodiscard]]
bool CsvParser::CsvFileReader::has_more() const {
	return _current_row < _total_rows;
}
[[nodiscard]]
PriceTimestempData CsvParser::CsvFileReader::last_value() const {
	return this->_last_value;
}
bool CsvParser::CsvFileReader::operator>(const CsvFileReader &other_) const {
	return this->_last_value.timestemp > other_._last_value.timestemp;
}

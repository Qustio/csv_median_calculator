#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <fstream>
#include <istream>
#include <spdlog/spdlog.h>
#include "app_config.hpp"
#include "config_parser.hpp"
#include "csv_parser.hpp"
#include "median_calculator.hpp"
#include "csv_writer.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::debug);
    po::options_description desc("Options");
    desc.add_options()
        ("config, cfg", po::value<std::string>()->default_value("config.toml"), "path to config")
    ;
    po::variables_map variables;
    try {
        po::store(po::parse_command_line(argc, argv, desc), variables);
        po::notify(variables); 
    } catch (const po::unknown_option& e) {
        spdlog::error(e.what());
        for (const auto& option : desc.options()) {
            spdlog::info(
                "{} - {}",
                option->canonical_display_name(),
                option->description()
            );
        }
        spdlog::shutdown();
        return -1;
    }
    auto config_path = variables["config"].as<std::string>();
    if (!std::filesystem::exists(config_path)) {
        spdlog::error("Конфигурация {} не найдена", config_path);
        spdlog::shutdown();
        return -1;
    }
	spdlog::info("Чтение конфигурации {}", config_path);
	AppConfig config;
	try {
		config = ConfigParser::load(config_path);
		SPDLOG_DEBUG("Конфиг output: {}", config.output);
	} catch (const std::exception& e) {
		spdlog::critical(e.what());
		spdlog::shutdown();
        return -1;
    }
	CsvParser parser(config);
	MedianCalculator calculator;
    auto output =  fs::path(config.output) / "result.csv";
    if (!fs::exists(output)) {
        std::ofstream file(output);
    }
	CsvWriter writer(output);

	try {
		for (const auto& timestemp : parser.read_data()) {
			auto median = calculator.calculate_next(timestemp);
			if (median) {
				spdlog::info(fmt::format("Медиана изменена - {}", median.value()));
				writer.write_median_change(timestemp.timestemp, median.value());
			}
		}
	} catch (const std::exception& e) {
		spdlog::critical(e.what());
		spdlog::shutdown();
        return -1;
	}
	return 0;
}
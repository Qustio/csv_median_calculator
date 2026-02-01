#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <spdlog/spdlog.h>
#include "app_config.hpp"
#include "config_parser.hpp"
#include "csv_parser.hpp"

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
	try {
		parser.find_files();
		parser.read_data();
	} catch (const std::exception& e) {
		spdlog::critical(e.what());
		spdlog::shutdown();
        return -1;
	}
	return 0;
}
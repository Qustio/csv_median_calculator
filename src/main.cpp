#include <filesystem>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <spdlog/spdlog.h>
#include <toml++/toml.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::debug);
    po::options_description desc("Options");
    desc.add_options()
        ("config, cfg", po::value<std::string>()->default_value("config.toml"), "path to config")
    ;
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm); 
    } catch (const po::unknown_option& e) {
        spdlog::error(e.what());
        for (const auto& option : desc.options()) {
            spdlog::info(
                "{} - {}",
                option.get()->canonical_display_name(),
                option.get()->description()
            );
        }
        spdlog::shutdown();
        return -1;
    }
    SPDLOG_DEBUG("{}", vm["config"]);
    auto config_path = vm["config"].as<std::string>();
    if (std::filesystem::exists(config_path)) {
        SPDLOG_INFO("Чтение конфигурации {}", config_path);
    } else {
        SPDLOG_INFO("Конфигурация {} не найдена", config_path);
        spdlog::shutdown();
        return -1;
    }
}
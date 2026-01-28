#include "config_parser.hpp"
#include <string>
#include <toml++/toml.hpp>
#include <vector>

std::vector<std::string> parse_string_array(const toml::array* arr) {
    if (arr == nullptr) {
		return {};
	}

    std::vector<std::string> result;
    result.reserve(arr->size());

    for (auto&& node : *arr) {
        result.emplace_back(node.value_or(""));
    }
    return result;
}


AppConfig ConfigParser::load(const std::string &path) {
	toml::table data = toml::parse_file(path);

	auto input = data["main"]["input"].value<std::string>();
	auto output = data["main"]["output"].value<std::string>();
	auto filename_mask = parse_string_array(data["main"]["filename_mask"].as_array());
	if (!input) {
        throw std::runtime_error("параметр input не задан");
    }
	return AppConfig{
		.input = input.value(),
		.output = output,
		.filename_mask = filename_mask
	};
}
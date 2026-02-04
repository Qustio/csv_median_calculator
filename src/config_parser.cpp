#include "src/config_parser.hpp"
#include <string>
#include <vector>
#include <toml++/toml.hpp>

std::vector<std::string> parse_string_array(const toml::array *arr_) {
	if (arr_ == nullptr) {
		return {};
	}

	std::vector<std::string> result;
	result.reserve(arr_->size());

	for (auto &&node : *arr_) {
		result.emplace_back(node.value_or(""));
	}
	return result;
}

app_config config_parser::load(const std::string &path_) {
	toml::table data = toml::parse_file(path_);

	auto input = data["main"]["input"].value<std::string>();
	auto output = data["main"]["output"].value<std::string>();
	auto filename_mask =
		parse_string_array(data["main"]["filename_mask"].as_array());
	if (!input) {
		throw std::runtime_error("параметр input не задан");
	}
	return app_config{
		.input = input.value(),
		.output = output.value_or("output"),
		.filename_mask = filename_mask
	};
}
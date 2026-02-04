#include "median_calculator.hpp"
#include <optional>
#include <boost/accumulators/statistics/median.hpp>

std::optional<double>
median_calculator::calculate_next(price_timestemp_data data_) {
	_accumulator_set(data_.price);
	double median = acc::incremental_median(_accumulator_set);
	if (median != _last_median) {
		_last_median = median;
		return median;
	}
	return std::nullopt;
};
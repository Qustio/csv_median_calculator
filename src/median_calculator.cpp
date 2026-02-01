#include "median_calculator.hpp"
#include <boost/accumulators/statistics/median.hpp>
#include <optional>

std::optional<double> MedianCalculator::calculate_next(PriceTimestempData _data) {
	_accumulator_set(_data.price);
	double median = acc::incremental_median(_accumulator_set);
	if (median != _last_median){
		_last_median = median;
		return median;
	}
	return std::nullopt;
};
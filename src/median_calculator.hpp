#ifndef MEDIAN_CALCULATOR
#define MEDIAN_CALCULATOR

#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/framework/features.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>

#include "price_timestemp_data.hpp"
#include "incremental_median.hpp"

namespace acc = boost::accumulators;

class MedianCalculator{
public:
	std::optional<double> calculate_next(PriceTimestempData _data);
private:
	acc::accumulator_set<double, acc::features<acc::tag::median, acc::tag::incremental_median>> _accumulator_set;
	double _last_median = 0;
};

#endif // MEDIAN_CALCULATOR
#ifndef INCREMENTAL_MEDIAN
#define INCREMENTAL_MEDIAN

#include <queue>
#include <boost/accumulators/accumulators.hpp>

// https://www.boost.org/doc/libs/1_84_0/doc/html/accumulators/user_s_guide.html#accumulators.user_s_guide.the_accumulators_framework.extending_the_accumulators_framework

namespace boost::accumulators {
	namespace impl {
		template <typename Sample>
		struct incremental_median : accumulator_base {
			using result_type = Sample;

			template <typename Args> incremental_median(Args const &args_) {}

			template <typename Args> void operator()(Args const &args_) {
				if (_lower.empty() || args_[sample] <= _lower.top()) {
					_lower.push(args_[sample]);
				} else {
					_upper.push(args_[sample]);
				}

				if (_lower.size() > _upper.size() + 1) {
					_upper.push(_lower.top());
					_lower.pop();
				} else if (_upper.size() > _lower.size()) {
					_lower.push(_upper.top());
					_upper.pop();
				}
			}

			result_type result(dont_care /*unused*/) const {
				if (_lower.empty()) {
					return 0;
				}
				if (_lower.size() == _upper.size()) {
					return (_lower.top() + _upper.top()) / 2;
				}
				return _lower.top();
			}

		private:
			std::priority_queue<Sample>
				_lower; // Heap for the smaller half of the data
			std::priority_queue<
				Sample,
				std::vector<Sample>,
				std::greater<Sample>>
				_upper; // Heap for the larger half of the data
		};
	} // namespace impl

	namespace tag {
		struct incremental_median : depends_on<> {
			using impl = accumulators::impl::incremental_median<mpl::_1>;
		};
	} // namespace tag

	namespace extract {
		extractor<tag::incremental_median> const incremental_median = {};
	}

	using extract::incremental_median;
} // namespace boost::accumulators

#endif // INCREMENTAL_MEDIAN
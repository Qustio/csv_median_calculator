#include <boost/accumulators/accumulators.hpp>
#include <queue>

// https://www.boost.org/doc/libs/1_84_0/doc/html/accumulators/user_s_guide.html#accumulators.user_s_guide.the_accumulators_framework.extending_the_accumulators_framework

namespace boost::accumulators {
	namespace impl {
		template<typename Sample>
		struct incremental_median: accumulator_base
		{
			using result_type = Sample;

			template<typename Args>
			incremental_median(Args const & args){}

			template<typename Args>
			void operator ()(Args const & args) {
				if (lower.empty() || args[sample] <= lower.top()) {
					lower.push(args[sample]);
				} else {
					upper.push(args[sample]);
				}

				if (lower.size() > upper.size() + 1) {
					upper.push(lower.top());
					lower.pop();
				} else if (upper.size() > lower.size()) {
					lower.push(upper.top());
					upper.pop();
				}
			}

			result_type result(dont_care /*unused*/) const {
				if(lower.empty()){
					return 0;
				}
				if (lower.size() == upper.size()) {
					return (lower.top() + upper.top()) / 2;
				}
				return lower.top();
			}
		private:
			std::priority_queue<Sample> lower; // Heap for the smaller half of the data
			std::priority_queue<Sample, std::vector<Sample>, std::greater<Sample> > upper; // Heap for the larger half of the data
		};
	}
	namespace tag {
		struct incremental_median: depends_on<>
		{
			using impl = accumulators::impl::incremental_median< mpl::_1 >;
		};
	}
	namespace extract {
		extractor< tag::incremental_median > const incremental_median = {};
	}
	using extract::incremental_median;
}
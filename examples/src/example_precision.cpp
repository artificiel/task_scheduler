#include <ssts/task_scheduler.hpp>
#include <spdlog/spdlog.h>
#include "utils/utils.hpp"

template <typename Duration>
std::tuple<double, double, double> meanStdDevAndLargest(const std::vector<Duration>& durations) {
	if (durations.empty()) {
		throw std::invalid_argument("Vector of durations is empty");
	}

	double sum = 0.0;
	for (const auto& d : durations) {
		sum += d.count();
	}
	double mean = sum / durations.size();

	double variance = 0.0;
	for (const auto& d : durations) {
		variance += (d.count() - mean) * (d.count() - mean);
	}
	variance /= durations.size();
	double stdDev = std::sqrt(variance);

	auto largest = std::max_element(durations.begin(), durations.end(),
		[](const auto & a, const auto & b) {
			return a.count() < b.count();
		});
	
	return {mean, stdDev, largest->count()};
}

auto test(std::string label, ssts::clock::duration warm = 0ms) {
	
	ssts::task_scheduler s(8, warm); // modified scheduler
	s.set_duplicate_allowed(false);
	s.start();
	
	std::optional<std::chrono::time_point<std::chrono::steady_clock>> prev;
	std::vector<std::chrono::microseconds> diffs {};

	std::cout << "        Starting: " << label << std::endl;

	for (size_t i=0; i<10; i++) {
		s.in(1ms * (100*i), [&prev, &diffs]{
			spdlog::info("mark");
			auto now = ssts::clock::now();
			if (prev) diffs.push_back(std::chrono::duration_cast<std::chrono::microseconds>(now-*prev));
			prev = now;
		});
	};
	std::this_thread::sleep_for(2s);
	s.stop();
	
	auto [mean, stddev, large] = meanStdDevAndLargest(diffs);
	std::cout << std::fixed << std::setprecision(6) <<  std::setfill(' ');
	std::cout << "         Samples: " << diffs.size() << " intervals\n";
	std::cout << "            Mean: " << std::setw(10) << mean / 1000 << " ms\n";
	std::cout << "         Largest: " << std::setw(10) << large / 1000 << " ms\n";
	std::cout << "          Stddev: " << std::setw(10) << stddev / 1000 << " ms\n";
	std::cout << " ============================\n\n";
}

int main()
{
    spdlog::set_pattern("[%H:%M:%S.%f] %v");
    spdlog::info(ssts::version());
	
	test("non-warmed");
	test("warmed 5ms", 5ms);
	test("non-warmed");
	test("warmed 2ms", 2ms);
	test("non-warmed");
	test("warmed 10ms", 10ms);
	test("non-warmed");
	test("warmed 5ms", 5ms);
	test("non-warmed");
	test("warmed 4ms", 4ms);
	test("non-warmed");
	test("warmed 6ms", 6ms);
	test("non-warmed");
	test("warmed 7ms", 7ms);
	test("non-warmed");
	test("warmed 3ms", 3ms);
	test("non-warmed");
	test("warmed 8ms", 8ms);
	test("non-warmed");

    return 0;
}

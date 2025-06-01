#include "../src/PriorityQueue.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace std::chrono;

std::mt19937 rng(std::random_device{}());

struct BenchmarkResult {
  double avg_insert = 0;
  double avg_extract = 0;
  double avg_peek = 0;
  double avg_decrease = 0;
};

BenchmarkResult run_benchmark(size_t n) {
  std::vector<int> values(n);
  iota(values.begin(), values.end(), 0);
  shuffle(values.begin(), values.end(), rng);

  std::uniform_int_distribution<size_t> priority_dist(1, n * 10);
  std::vector<size_t> priorities;
  for (size_t i = 0; i < n; i++) {
    priorities.push_back(priority_dist(rng));
  }

  std::vector<long> insert_times, extract_times, peek_times, decrease_times;

  for (int iter = 0; iter < 5; iter++) {
    PriorityQueue<int> pq;

    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < n; i++) {
      pq.Enqueue(values[i], priorities[i]);
    }
    auto t2 = high_resolution_clock::now();
    insert_times.push_back(duration_cast<microseconds>(t2 - t1).count());

    t1 = high_resolution_clock::now();
    auto [p, v] = pq.Peek();
    t2 = high_resolution_clock::now();
    peek_times.push_back(duration_cast<microseconds>(t2 - t1).count());

    std::uniform_int_distribution<size_t> index_dist(0, n - 1);
    size_t idx = index_dist(rng);
    size_t new_priority = priorities[idx] / 2;

    t1 = high_resolution_clock::now();
    pq.DecreasePriority(values[idx], new_priority);
    t2 = high_resolution_clock::now();
    decrease_times.push_back(duration_cast<microseconds>(t2 - t1).count());

    t1 = high_resolution_clock::now();
    while (!pq.IsEmpty()) {
      pq.Dequeue();
    }
    t2 = high_resolution_clock::now();
    extract_times.push_back(duration_cast<microseconds>(t2 - t1).count());
  }

  auto filter_avg = [](std::vector<long> &times) {
    if (times.empty())
      return 0.0;
    auto max_it = max_element(times.begin(), times.end());
    times.erase(max_it);
    return accumulate(times.begin(), times.end(), 0.0) / times.size();
  };

  return {filter_avg(insert_times), filter_avg(extract_times),
          filter_avg(peek_times), filter_avg(decrease_times)};
}

int main() {
  std::vector<size_t> sizes = {1000, 2000, 4000, 8000, 16000, 32000, 64000};
  std::ofstream out("benchmark_results.csv");

  out << "Size,Insert(us),Extract(us),Peek(us),Decrease(us)\n";

  for (size_t n : sizes) {
    std::cout << "Benchmarking n = " << n << "..." << std::endl;
    auto result = run_benchmark(n);

    out << n << "," << result.avg_insert << "," << result.avg_extract << ","
        << result.avg_peek << "," << result.avg_decrease << "\n";
  }

  std::cout << "Results saved to benchmark_results.csv" << std::endl;
  return 0;
}

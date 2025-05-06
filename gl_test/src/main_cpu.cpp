#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#define NUM 100000000

int main(int argc, char* argv[]) {
  std::cout << "number of elements: " << NUM << std::endl;
  std::chrono::high_resolution_clock::time_point start  = std::chrono::high_resolution_clock::now();
  std::vector<int> data(NUM);
// #pragma omp parallel for
  for (uint32_t i = 0; i < NUM; ++i) {
    float x = static_cast<float>(rand()) / RAND_MAX;
    float y = static_cast<float>(rand()) / RAND_MAX;
    if ((x * x + y * y) <= 1.0) {
      data[i] = 1;
    }
  }
  std::chrono::high_resolution_clock::time_point send = std::chrono::high_resolution_clock::now();
  double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(send - start).count() / 1000.0);
  std::cout << "cpu loop time : " << time << " ms" << std::endl;
  size_t sum = std::accumulate(data.begin(), data.end(), 0LL);
  std::cout << "sum : " << sum << std::endl;
  std::cout << "pi : " << 4.0 * sum / NUM << std::endl;
  double stime =
      static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0);
  std::cout << "all time : " << stime << " ms" << std::endl;
}
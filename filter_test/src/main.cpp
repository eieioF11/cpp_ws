// デバック設定
// #define SHOW_DEBUG_IMAGE
// #define DEBUG_OUTPUT
// std
#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <stdio.h>
#include <unistd.h>
#include <vector>
// matplotlibcpp17
#include <matplotlibcpp17/pyplot.h>
// OpenMP
#include <omp.h>
// Eigen
#include <Eigen/Dense>
// utility
#include "math_util.hpp"

int main(int argc, char *argv[])
{
  std::cout << "MAX threads NUM:" << omp_get_max_threads() << std::endl;

  return 0;
}

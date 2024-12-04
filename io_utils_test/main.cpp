#include "io_util.hpp"
#include <iostream>

int main() {
  using namespace common_utils;
  logger.set_log_level(LogLevel::DEBUG);
  logger.println(LogLevel::ERROR, "Hello, World!");
  std::vector<int> data = {1, 2, 3, 4, 5};
  logger.out(LogLevel::DEBUG) << to_string(data) << " Hello, World!" << Logger::endl;
  logger.out(LogLevel::INFO) << to_string(data) << " Hello, World!" << Logger::endl;
  logger.out(LogLevel::WARN) << to_string(data) << " Hello, World!" << Logger::endl;
  logger.out(LogLevel::ERROR) << to_string(data) << " Hello, World!" << Logger::endl;
  std::cout << LogLevel::DEBUG << to_string(data)<< " Hello, World!" << Logger::endl;
  // std::cout << " Hello, World!" << std::endl;
  std::cout << LogLevel::INFO << to_string(data) << " Hello, World!" << Logger::endl;
  std::cout << LogLevel::WARN << to_string(data) << " Hello, World!" << Logger::endl;
  std::cout << LogLevel::ERROR <<to_string(data) << " Hello, World!" << Logger::endl;
  return 0;
}
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(nao_report2)

add_compile_options(-O3)
add_compile_options(-pg)

find_package(Eigen3 REQUIRED)

find_package(Python3 COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)
find_package(matplotlibcpp17)

# For OpenMP
find_package(OpenMP REQUIRED)

if(OpenMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ./include)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen ${Python3_LIBRARIES} pybind11::embed matplotlibcpp17::matplotlibcpp17)
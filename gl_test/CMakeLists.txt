cmake_minimum_required(VERSION 3.14)
project(gl_test)

set(CMAKE_CXX_STANDARD 17)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

add_executable(gl_test src/main_gpu.cpp)
set_target_properties(gl_test PROPERTIES LINK_FLAGS "$ENV{MYLDFLAGS}")
target_link_libraries(gl_test PRIVATE  -lglfw -lGL Qt5::Widgets ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})

add_executable(pi_cpu src/main_cpu.cpp)
set_target_properties(pi_cpu PROPERTIES LINK_FLAGS "$ENV{MYLDFLAGS}")
target_link_libraries(pi_cpu PRIVATE  -lglfw -lGL Qt5::Widgets ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
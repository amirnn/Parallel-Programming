# CMakeLists.txt
project(demo)
cmake_minimum_required(VERSION 3.10)

find_package(OpenMP)
set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")

add_executable(demo demo.c)
target_link_libraries(demo ${OpenMP_LIBRARIES})
add_executable(helloWorld helloworld.cc)

add_executable(helloWorldParallel helloworldParallel.cc)
target_link_libraries(helloWorldParallel ${OpenMP_LIBRARIES})
cmake_minimum_required(VERSION 3.20)
project(space-shooter C CXX)

include_directories(src/lib)
include_directories(ext/include)

add_executable(space-shooter)
add_subdirectory(src)
add_compile_definitions(USE_WIRINGPI_LIB)
target_link_libraries(space-shooter wiringPi)
cmake_minimum_required(VERSION 3.17.3)
project(struct_array VERSION 1.0.0)

add_library(struct_array INTERFACE)

find_package(Boost REQUIRED)
target_include_directories(struct_array INTERFACE ${Boost_INCLUDE_DIRS})

target_include_directories(struct_array INTERFACE include/)
target_compile_features(struct_array INTERFACE cxx_std_20)
target_compile_definitions(struct_array INTERFACE -DSOA_MAX_BINDINGS=5)

add_executable(struct_array_test test/struct_array_test.cpp)

target_compile_features(struct_array_test PRIVATE cxx_std_20)
target_link_libraries(struct_array_test struct_array)
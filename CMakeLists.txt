cmake_minimum_required(VERSION 3.17.3)
project(struct_array VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(struct_array INTERFACE)

target_include_directories(struct_array INTERFACE include/)
target_compile_features(struct_array INTERFACE cxx_std_20)

add_executable(struct_array_test test/struct_array_test.cpp)

target_compile_features(struct_array_test PRIVATE cxx_std_20)
target_link_libraries(struct_array_test struct_array)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

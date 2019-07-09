cmake_minimum_required(VERSION 3.9)
project(cpp-practice LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

enable_testing()

add_subdirectory(error-code)
add_subdirectory(move-semantics)

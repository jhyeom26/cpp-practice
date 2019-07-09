# mylib
add_library(mylib
  STATIC
    src/mylib.cpp
    src/mylib_error.cpp
    src/seats.cpp
    src/flights.cpp
)

target_include_directories(mylib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(mylib
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/error-code/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/error-code/lib")

# Unit Test
add_executable(error-code-test
  mylib_test.cpp
)

target_link_libraries(error-code-test
  mylib
  CONAN_PKG::gtest)

set_target_properties(error-code-test
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/error-code/bin"
    OUTPUT_NAME testapp)

get_target_property(test-dir error-code-test RUNTIME_OUTPUT_DIRECTORY)

add_test(
  NAME error-code-test
  COMMAND testapp
  WORKING_DIRECTORY ${test-dir})

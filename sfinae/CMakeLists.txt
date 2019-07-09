# Unit Test
add_executable(sfinae-test
  sfinae_test.cpp
)

target_link_libraries(sfinae-test
  CONAN_PKG::gtest)

set_target_properties(sfinae-test
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/sfinae/bin"
    OUTPUT_NAME testapp)

get_target_property(test-dir sfinae-test RUNTIME_OUTPUT_DIRECTORY)

add_test(
  NAME sfinae-test
  COMMAND testapp
  WORKING_DIRECTORY ${test-dir})

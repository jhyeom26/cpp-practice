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


add_executable(error-code-test
  src/mylib_test.cpp
)

target_link_libraries(error-code-test
  PUBLIC
    mylib)

set_target_properties(error-code-test
  PROPERTIES
    OUTPUT_NAME testapp)

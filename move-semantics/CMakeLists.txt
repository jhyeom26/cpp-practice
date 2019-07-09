add_executable(move-semantics-test main.cc)
set_target_properties(move-semantics-test
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/move-semantics/bin"
      OUTPUT_NAME testapp)

#pragma once

#include <system_error>

enum class FailureSource
{
    // no 0
    BadUserInput = 1,
    SystemError = 2,
    NoSolution = 3,
};

namespace std
{
  template <>
    struct is_error_condition_enum<FailureSource> : true_type {};
}

std::error_condition make_error_condition(FailureSource e);

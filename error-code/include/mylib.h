#pragma once
#include <system_error>

#include "mylib_error.h"

namespace mylib{
  std::error_code func1();
  std::error_code func2();
}

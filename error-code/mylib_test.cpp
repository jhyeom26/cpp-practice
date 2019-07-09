#include "mylib.h"

#include <gtest/gtest.h>

TEST(std_error_code_test, BasicTest)
{
  // mylib을 사용하는 관점에서
  // 내부 error code system의 detail을 알 필요는 없다.
  auto ec = mylib::func1();

  // error_condition을 이용해 mylib 사용자에게 필요한 query를 제공할 수 있다.
  // error_code와 error_condition 간의 comparison은 error_condition 구현부에 숨겨져 있다.
  // error_code가 어떤 enum class로 만들어졌고, 그 내부의 error_code가 무엇무엇이 있는지에 대한 detail은
  // 내부 구현부에 숨겨져 있다. 사용자가 알 필요있는 정보는 error_condition 뿐이다.
  EXPECT_EQ(ec, FailureSource::NoSolution);
}

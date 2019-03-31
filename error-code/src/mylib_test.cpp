#include "mylib.h"
#include <iostream>

int main(){

  // mylib을 사용하는 관점에서
  // 내부 error code system의 detail을 알 필요는 없다.
  auto ec = mylib::func1();
  std::cerr << ec.message() << std::endl;

  // error_condition을 이용해 mylib 사용자에게 필요한 query를 제공할 수 있다.
  if(ec == FailureSource::NoSolution){
    std::error_condition errCond = FailureSource::NoSolution;
    std::cerr << errCond.message() << std::endl;
    return -1;
  }

  return 0;
}

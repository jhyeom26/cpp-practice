#include "mylib.h"

int main(){
  if(mylib::func1() == FailureSource::NoSolution &&
    mylib::func2() == FailureSource::NoSolution
  ){
    return -1;
  }
  return 0;
}

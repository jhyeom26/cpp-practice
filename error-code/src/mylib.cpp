#include "mylib.h"

#include "flights.h"
#include "seats.h"

namespace mylib{
  std::error_code func1(){
    return FlightsErrc::NoFlightsFound;
  }
  std::error_code func2(){
    return SeatsErrc::NonexistentClass;
  }
}

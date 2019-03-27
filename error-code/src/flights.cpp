#include "flights.h"
  
namespace { // anonymous namespace
   
struct FlightsErrCategory : std::error_category
{
  const char* name() const noexcept override;
  std::string message(int ev) const override;
};
   
const char* FlightsErrCategory::name() const noexcept
{
  return "flights";
}
   
std::string FlightsErrCategory::message(int ev) const
{
  switch (static_cast<FlightsErrc>(ev))
  {
  case FlightsErrc::NonexistentLocations:
    return "nonexistent airport name in request";
    
  case FlightsErrc::DatesInThePast:
    return "request for a date from the past";
   
  case FlightsErrc::InvertedDates:
    return "requested flight return date before departure date";
   
  case FlightsErrc::NoFlightsFound:
    return "no filight combination found";
   
  case FlightsErrc::ProtocolViolation:
    return "received malformed request";
   
  case FlightsErrc::ConnectionError:
    return "could not connect to server";
   
  case FlightsErrc::ResourceError:
    return "insufficient resources";
   
  case FlightsErrc::Timeout:
    return "processing timed out";
   
  default:
    return "(unrecognized error)";
  }
}

// 이 전역변수의 존재는 error-category가 전역적으로 유일하기 위해
// (third-party들에 존재하는 error-category를 포함)
// 전역변수의 주소를 구분자로 쓰는 technique을 위해 선언한 것.
// std::error_category의 default ctor가 constexpr이므로,
// 상속된 category의 default ctor 또한 constexpr이 되고
// 이것은 constant-intialization에 초기화되는 것을 의미한다.
// 즉 static initialization order로부터 자유롭다.
const FlightsErrCategory theFlightsErrCategory {};
   
} // anonymous namespace
  
std::error_code make_error_code(FlightsErrc e)
{
  return {static_cast<int>(e), theFlightsErrCategory};
}

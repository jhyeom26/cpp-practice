#pragma once

#include <system_error>
  
enum class FlightsErrc
{
  // no 0
  NonexistentLocations = 10, // requested airport doesn't exist
  DatesInThePast,            // booking flight for yesterday
  InvertedDates,             // returning before departure
  NoFlightsFound       = 20, // did not find any combination
  ProtocolViolation    = 30, // e.g., bad XML
  ConnectionError,           // could not connect to server
  ResourceError,             // service run short of resources
  Timeout,                   // did not respond in time
};

// std::error_code ec = FlightsErrc::NonexistentLocations;
// 같은 것들이 먹히려면 아래 두가지 작업이 필요하다.

namespace std
{
  template <>
    struct is_error_code_enum<FlightsErrc> : true_type {};
}

std::error_code make_error_code(FlightsErrc);

// std::error_code의 = operator는 std::error_code make_error_code를 통해 동작

// 내가 error_code로 사용하려는 enum class를 입력으로 받는
// make_error_code free function을 overload하면,
// argument-dependent lookup에 따라 접근가능하게 된다.

// overload의 선수조건은 std::is_error_code_enum<ErrorCodeEnum>::value == true이다.
// 그래서 위의 is_erro_code_enum의 specialization을 추가해주었다.

#include "mylib_error.h"

#include "flights.h"
#include "seats.h"

namespace {

class FailureSourceCategory : public std::error_category
{
public:
  const char* name() const noexcept override;
  std::string message(int ev) const override;
  bool equivalent(
      const std::error_code& code,
      int condition) const noexcept override;
};

const char* FailureSourceCategory::name() const noexcept
{
    return "failure-source";
}

std::string FailureSourceCategory::message(int ev) const
{
  switch (static_cast<FailureSource>(ev))
  {
  case FailureSource::BadUserInput:
    return "invalid user request";

  case FailureSource::SystemError:
    return "internal error";

  case FailureSource::NoSolution:
    return "no solution found for specified request";

  default:
    return "(unrecognized condition)";
  }
}

bool FailureSourceCategory::equivalent(
      const std::error_code& ec,
      int cond) const noexcept
{
  const std::error_category& FlightsCat =
    std::error_code{FlightsErrc{}}.category();

  switch (static_cast<FailureSource>(cond))
  {
  case FailureSource::BadUserInput:
    if (ec == SeatsErrc::NonexistentClass)
      return true;
    if (ec.category() == FlightsCat)
      return ec.value() >= 10 && ec.value() < 20;
    return false;

  case FailureSource::SystemError:
    if (ec == SeatsErrc::InvalidRequest ||
        ec == SeatsErrc::CouldNotConnect ||
        ec == SeatsErrc::InternalError ||
        ec == SeatsErrc::NoResponse)
      return true;

    if (ec.category() == FlightsCat)
      return ec.value() >= 30 && ec.value() < 40;

    return false;

  case FailureSource::NoSolution:
    if (ec == SeatsErrc::NoSeatAvailable)
      return true;
    if (ec.category() == FlightsCat)
      return ec.value() >= 20 && ec.value() < 30;
    return false;

  default:
    return false;
  }
}

const FailureSourceCategory theFailureSourceCategory {};

} // anonymous namespace

std::error_condition make_error_condition(FailureSource e)
{
  return {static_cast<int>(e), theFailureSourceCategory};
}

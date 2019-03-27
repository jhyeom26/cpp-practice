#include "seats.h"
  
namespace { // anonymous namespace
   
struct SeatsErrCategory : std::error_category
{
  const char* name() const noexcept override;
  std::string message(int ev) const override;
};
   
const char* SeatsErrCategory::name() const noexcept
{
  return "seats";
}
   
std::string SeatsErrCategory::message(int ev) const
{
  switch (static_cast<SeatsErrc>(ev))
  {      
  case SeatsErrc::InvalidRequest:
    return "received malformed request";
  
  case SeatsErrc::CouldNotConnect:
    return "could not connect to server";
     
  case SeatsErrc::InternalError:
    return "insufficient resources";
     
  case SeatsErrc::NoResponse:
    return "processing timed out";
           
  case SeatsErrc::NonexistentClass:
    return "requested class does not exist";
    
  case SeatsErrc::NoSeatAvailable:
    return "all seats booked";
   
  default:
    return "(unrecognized error)";
  }
}
   
const SeatsErrCategory theSeatsErrCategory {};
} // anonymous namespace
  
std::error_code make_error_code(SeatsErrc e)
{
  return {static_cast<int>(e), theSeatsErrCategory};
}

# Your Own error condition

우리가 std::error_code로 하고자 하는 것
* if stmt없이 loging하는 것.
* 내가 분류해놓은 어떤 조건에 빠지는지 확인하기 위함.
  * error code를 case by case로 핸들링하기 위함이 아니다.

여기에서 classification을 구현하는데 사용되는 것이 std::error_condition이다.

구현 방식은 std::error_code를 구현하는 것과 하등차이가 없다.
std::is_error_condition_enum을 speciaize해 stl이 enum class를
std::error_condition으로 사용할 수 있게 구현한다.

그렇다면 std::error_code와 std::error_condition의 차이는 뭘까?

* std::error_code는 error_code를 가공없이 저장, 전달하는 용도.
* std::error_condition은 error_code를 분류해 error_code에 대한 query를 제공하는 용도

```cpp
std::error_code ec = FlightsErrc::InvertedDates;

// 이렇게 == operator로 비교하는 것이 실제 같은지를 비교하는 것은 아니기 때문에
// 잘못된 연산자 사용이라고 생각할 수도 있을 것이다.
// 하지만 이게 stl이 정의한 error_condition을 체킹하는 법이다.
// 혹은 하나의 error_condition에 여러 error_code를 매칭시키는 
// 나의 사용법이 잘못된 것일 수도 있겠다.
assert(ec == FailureSource::BadUserInput);
```

## Defining semantics for conditions

error_code와 error_condition의 matching을 정의하기 위해서는
또다른 error_category가 필요하다.

```cpp
namespace
{
struct FailureSourceCategory : std::error_category
{
    const char* name() const noexcept override;
    std::string message(int ev) const override;
    bool equivalent(
        const std::error_code& code, int condition) const noexcept override;
}

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


// 여기에서의 철학은 default로 condition과 error_code는 같지 않다는 점이다.
// equivalent를 구현해주어야 비교가 가능하다.
bool FailureSourceCategory::equivalent(
    const std::error_code& ec, int cond) const noexcept
{
  // FlightsErrc의 category가 private detail로 감춰져 있긴 하지만,
  // 다음과 같이 얻어서 사용 할 수 있다.
  const std::error_category& FlightsCat = 
    std::error_code{FlightsErrc{}}.category();

  switch (static_cast<FailureSource>(cond))
  {
  case FailureSource::BadUserInput:
    if (ec == SeatsErrc::NonexistentClass)
      return true;
    // category에 대한 비교는 object address를 비교하는 것이다.
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
}
```

근데 그냥 is_system_error(), is_bad_input(), is_no_solution()
이런거 안쓰고 굳이 enumeration을 만들어 customize해서 사용할까?

함수를 이용한 방법은 당장 내부의 error system안에서는 문제 없이 동작하겠지만,
외부에서 해당 library를 사용한다고 할 때, 저 방법은 adaptable하지 않다.

C++ error-code framework는 새로운 error_condition이 
기존에 존재하는 error_code를 알아보도록 가르칠수도 있고,

새로 만든 error_code가 기존에 존재하는 error_conditon에 의해 분류되도록 만들수도 있다.

```cpp
// std::error_category에 있는 아래의 또다른 virtual function을 구현함으로써
// 다른 error_code나 condition을 서로 adapting할 수 있다.
// 똑같은 함수 이름이지만 parameter가 다르다.
bool equivalent(int code, const error_condition& cond) const noexcept;

// 그래서 operator ==는 다음과 같이 두가지 방식을 모두 고려하게끔 구현되어있다.
bool operator==(const error_code& lhs,
                const error_condition& rhs) noexcept
{
  return lhs.category().equivalent(lhs.value(), rhs) || 
         rhs.category().equivalent(lhs, rhs.value());
}
```
# Your Own error code

std::error_code 는 하나의 {int, domain} pair이다.

std::error_code로 하려고 하는 것은 다음과 같다.
* application 상위 layer에서 변환한 값이 아닌, 생성된 그대로의 error code를 로깅하기 위함.
* 어떤 질의에 답변할 때 사용.

## why use std::error_code instead of exceptions
* exception 대신에 error_code를 쓰자고 하는 것이 아니다.
  * program의 failure는 exception을 통해서 하는 것이 옳을 수 있다.
* Exception에 string을 저장하고 파싱하는 것이 아니라, error_code를 담아, 쉽게 inspect하고 싶다.
* 그리고 많은 type의 exception의 필요성도 잘 모르겠다. 한 위치에서(혹은 두) catch하고 error_code를 inspecting해서 다른 situation을 전달할 수 있다.

## std::error_code

enum class를 error_code로 동작하게 하기 위해서는
* std::is_error_code_enum<MyErrcClass>::true_type 를 specialize해야한다.
* std::make_error_code를 overloading 해주어야 한다.

## defining error-category

std::error_code는 {value, domain} pair라고 했는데,
이 때 domain을 std는 error-category라고 표현했다.

하지만 domainID는 one machine word에 저장되어야하는데,
third-party의 error-category를 포함해 어떻게 전역적으로 충돌하지 않고
유일하게 만들수 있을까?

std::error_code는 domainID를 error-category 전역변수의 주소를 사용함으로써 해결했다.

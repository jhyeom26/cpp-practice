// SFINAE : Substitution Failure Is Not An Error.
// The example below was written reffering https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html
// Overload resolution for a function call in c++
//   * Name lookup
//   * Template Argument Deduction
//     * for function templates, the template arg values are deduced
//       from the types of the actual arg passed in to the function.
//       * All occurrences of the template parameter
//         (in the return type and parameters types) are substituted with those deduced types.
//       * When this process leads to invalid type,
//         the particular function is removed from the overload resolution set. (SFINAE)
//   * Final Viable Function set
//   * Selection of the best viable function

// Disadvantages of SFINAE
//   * ugly error message
//   * Readability
//   * Nested template won't work in enable_if stmt

// Alternatives to SFINAE
//   * tag dispatching
//   * static_if
//   * concepts - c++20..

#include <type_traits>

enum class Polygon {
  Triangle,
  Rectangle,
  Pentagon
};

class DrawableWindow {
public:
  void viewWindow() {}
  void drawPolygon(Polygon polygon) {}
};

class NonDrawableWindow {
public:
  void viewWindow() {}
};

namespace detail {

template <typename T>
struct HasViewWindowMethod {
private:
  template<typename U>
  static auto test(int) ->
    std::enable_if_t<
      std::is_same<
        decltype(std::declval<U>().viewWindow()),
        void>::value,
      std::true_type>;

  // in case of failing to find test<T>, redirect to test(...)
  template<typename>
  static std::false_type test(...);
public:
  static constexpr bool value =
    std::is_same<decltype(test<T>(0)),std::true_type>::value;
};

template <typename T>
struct HasDrawPolygonMethod {
private:
  // template function with template parameter U
  // utilizing duck typing, we can check type U has a method
  // which type is void(*)(Polygon) and name is drawPolygon
  template<typename U>
  static auto test(int) ->
    std::enable_if_t<
      std::is_same<
        decltype(std::declval<U>().drawPolygon(Polygon())),
        void>::value,
      std::true_type>;

  // in case of failing to find test<T>, redirect to test(...)
  template<typename>
  static std::false_type test(...);
public:
  static constexpr bool value =
    std::is_same<decltype(test<T>(0)),std::true_type>::value;
};
}

template <
  typename Window,
  std::enable_if_t<
    detail::HasViewWindowMethod<Window>::value &&
    detail::HasDrawPolygonMethod<Window>::value, int> = 0> // int is dummy type
Window openDrawableWinwow(){
  return Window{};
}

template <
  typename Window,
  std::enable_if_t<
    detail::HasViewWindowMethod<Window>::value &&
    !detail::HasDrawPolygonMethod<Window>::value, int> = 0> // int is dummy type
Window openNonDrawableWinwow(){
  return Window{};
}

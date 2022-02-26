
#ifndef TEST_MTT_TRAITS_HPP
#define TEST_MTT_TRAITS_HPP

import<string_view>;

import mtt.traits;

#include "test_mtt_base.hpp"

namespace mtt::test {
using std::same_as;

double TestFunc(int, char) {
  return 0.;
};

using namespace mtt::literals;

template <string_literal>
struct Bars {
  using type = int;
};
template <>
struct Bars<"hello world"_sl> {
  using type = double;
};

template <named_tag>
struct Food {};
template <>
struct Food<"my name is Food"_tag> : std::true_type {};

TEST(Tpl) {
  std::tuple tpl(5, 10u, "char*", nullptr);
  constexpr auto count_of_integral = apply(
      []<typename... Types>(Types && ...) { return ((std::is_integral_v<std::remove_cvref_t<Types>>)+...); },
      tpl);
  static_assert(count_of_integral == 2);
}
TEST(StringLiterals) {
  constexpr auto sl1 = "Hello"_sl;
  constexpr auto sl2 = " world"_sl;
  static_assert((sl1 + sl2) == "Hello world"_sl);
  static_assert(std::is_same_v<double, typename Bars<"hello world">::type>);
  static_assert(std::is_same_v<decltype(string_literal("Hello")), decltype("Hello"_sl)>);
  static_assert(std::is_same_v<named_tag<"Hello">, decltype("Hello"_tag)>);
  static_assert(!std::is_same_v<named_tag<"Hello">, decltype("HellO"_tag)>);
  static_assert(Food<"my name is Food"_tag>::value);
  static_assert(std::is_same_v<decltype("String"_sl),
                               decltype(string_literal(std::array{'S', 't', 'r', 'i', 'n', 'g'}))>);
  constexpr auto v = string_literal<char, 4>(std::string_view("str").data());
}

}  // namespace mtt::test

#endif  // !TEST_MTT_TRAITS_HPP

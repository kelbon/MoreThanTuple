
#ifndef TEST_MTT_TUPLE_HPP
#define TEST_MTT_TUPLE_HPP

#include <type_traits>

import mtt.tuple;

#include "test_mtt_base.hpp"

namespace mtt::test {

struct some_struct {
  using enable_magic = bool;
  float h;
  char c;
};

struct ref_aggr {
  using enable_magic = void;
  float& a;
  int b;
};

TEST(TupleCatting) {
  constexpr auto lh = mtt::magic_tuple_size_v<ref_aggr>;
  float x = 3.14f;
  ref_aggr refa(x, 10);
  std::tuple tpl1(10, 20, "hello");
  mtt::tuple tpl2('c', nullptr, "world");
  verify(tpl2 == tpl2 && !(tpl2 != tpl2));
  some_struct aggr(5.f, '\n');
  auto result = ::mtt::tuple_cat(tpl1, refa, tpl2, aggr);
  ::mtt::visit(aggr, [](auto& value) { value = 0; });
  verify(aggr.h == 0);
  verify(aggr.c == '\0');
  static_assert(std::is_same_v<decltype(result), mtt::tuple<int, int, char const*, float&, int, char,
                                                               std::nullptr_t, const char*, float, char>>);
}

TEST(Constructing) {
  mtt::tuple tpl1("hello", 'c', 10);
  auto tpl2 = tpl1;
  verify(tpl2 == tpl1);
}

TEST(Algos) {
  constexpr size_t hm = apply([](auto... args) { return (args + ...); }, make_index_list<100>{});
  static_assert(hm == 4950);

  std::tuple my_tuple(5, 'c', 10.);
  mtt::tuple not_my_tuple(150, nullptr);
  std::pair pair(std::vector<int>(), "Hello world");
  std::array arr = {1, 2, 3, 4, 5};
  auto cat = mtt::tuple_cat(my_tuple, not_my_tuple, pair, arr);
  constexpr size_t sz = mtt::apply(
      []<typename... Types>(Types && ...) { return (sizeof(Types) + ...); }, cat);
  static_assert(sz == sizeof(int) + sizeof(char) + sizeof(double) + sizeof(int) + sizeof(nullptr_t) +
                          sizeof(std::vector<int>) + sizeof(const char*) + 5 * sizeof(int));
  constexpr auto check = mtt::apply(
      []<typename... Types>(Types && ...) {
        if constexpr ((mtt::is_implicit_default_constructible_v<Types> && ...)) {
          return std::false_type{};
        } else {
          return std::true_type{};
        }
      },
      cat);
  static_assert(std::is_same_v<decltype(check), const std::true_type>);
}
}  // namespace mtt::test

#endif TEST_MTT_TUPLE_HPP

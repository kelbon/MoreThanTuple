
#ifndef TEST_MTT_SERIALIZATION_HPP
#define TEST_MTT_SERIALIZATION_HPP

#include <map>
#include <memory>
#include <ranges>
#include <set>
#include <sstream>
#include <vector>

#include "test_mtt_base.hpp"

import mtt.traits;
import mtt.tuple;
import mtt.serialization;

namespace mtt::test {

TEST(SerializeConcepts) {
  enum teste : char {};
  enum class test2 : int {};
  enum test3 : unsigned char {};

  constexpr bool v00 =
      !is_compile_time_sized<std::vector<int>>() && is_compile_time_sized<std::pair<int, double>>() &&
      !is_compile_time_sized<std::vector<std::map<double, std::vector<int[10][15]>[3][4]>>>() &&
      !is_compile_time_sized<std::pair<std::vector<std::map<int, std::set<int>>>, float>>() &&
      !is_compile_time_sized<std::pair<std::tuple<std::array<std::set<int>, 10>>, float>>() &&
      is_compile_time_sized<std::pair<std::tuple<std::array<int, 10>>, float>>();
  static_assert(v00);

  constexpr bool v01 = runtime_sized_range<std::vector<int>> &&
                       !runtime_sized_range<std::pair<int, double>> &&
                       runtime_sized_range<std::vector<std::map<double, std::vector<int[10][15]>[3][4]>>> &&
                       !runtime_sized_range<std::pair<std::vector<std::map<int, std::set<int>>>, float>>;
  static_assert(v01);

  constexpr bool v0 = guaranteed_size_enum<std::byte> && guaranteed_size_enum<const volatile std::byte> &&
                      !guaranteed_size_enum<int> && !guaranteed_size_enum<test2> &&
                      guaranteed_size_enum<teste> && guaranteed_size_enum<test3> &&
                      !guaranteed_size_enum<unsigned char>;
  static_assert(v0);

  constexpr bool v1 =
      !guaranteed_size_type<int> && guaranteed_size_type<const std::byte> && !guaranteed_size_type<float> &&
      !guaranteed_size_type<int32_t> && guaranteed_size_type<char> && guaranteed_size_type<std::byte> &&
      guaranteed_size_type<const volatile unsigned char> && !guaranteed_size_type<const volatile char*> &&
      guaranteed_size_type<volatile char32_t> && guaranteed_size_type<test3> && !guaranteed_size_type<test2>;
  static_assert(v1);

  constexpr bool v2 =
      serializable_base_requirement<size_t> && serializable_base_requirement<std::byte> &&
      !serializable_base_requirement<mtt::tuple<double, char*>>  // tuple has more then 1 non empty
                                                                    // base class, no standard_layout
      && !serializable_base_requirement<void*> && !serializable_base_requirement<nullstruct> &&
      !serializable_base_requirement<nullptr_t> && !serializable_base_requirement<const float&>;
  static_assert(v2);

  using enum mode;
  constexpr bool v3 =
      serializable_not_pack<std::byte, network> &&
      serializable_not_pack<const int, network_with_int_aliases> && serializable_not_pack<double, network> &&
      serializable_not_pack<const volatile double, single_machine> &&
      serializable_not_pack<float const, network> && serializable_not_pack<const volatile teste, network> &&
      serializable_not_pack<volatile test2, network_with_int_aliases> &&
      !serializable_not_pack<std::pair<float, char>, single_machine> &&
      !serializable_not_pack<nullstruct, network> && !serializable_not_pack<nullptr_t, single_machine> &&
      !serializable_not_pack<const void*&, network_with_int_aliases> &&
      !serializable_not_pack<std::map<test3, mtt::tuple<double, std::byte const, float>>, network>;
  static_assert(v3);

  constexpr bool v4 =
      set_of_serializable_types<std::tuple<int, double>, single_machine> &&
      set_of_serializable_types<std::pair<std::vector<std::map<int, std::set<int>>>, float>,
                                network_with_int_aliases> &&
      set_of_serializable_types<std::tuple<std::vector<std::pair<char, double>>>, network> &&
      set_of_serializable_types<std::vector<int>, single_machine> &&
      set_of_serializable_types<std::pair<std::array<float, 10>, test3[10][30][150]>, network> &&
      set_of_serializable_types<std::vector<std::map<double, std::vector<int[10][15]>[3][4]>>,
                                network_with_int_aliases> &&
      !set_of_serializable_types<std::tuple<std::vector<std::pair<int, double>>>, network> &&
      !set_of_serializable_types<int, network_with_int_aliases> &&
      !set_of_serializable_types<std::pair<nullptr_t, float>, single_machine>;
  static_assert(v4);

  constexpr bool v5 =
      serializable<std::pair<std::vector<std::map<int, std::set<int>>>, float>, single_machine> &&
      serializable<std::tuple<std::vector<std::pair<int, double>>>, single_machine> &&
      serializable<std::byte const, network> &&
      serializable<std::map<test3, mtt::tuple<double, std::byte const, float>>, network> &&
      serializable<std::vector<std::map<double, std::vector<int[10][15]>[3][4]>>, network_with_int_aliases> &&
      serializable<float&, network> && !serializable<volatile const char*, network> &&
      serializable<std::pair<std::vector<std::map<int, std::set<int>>>, float>, network_with_int_aliases>;
  static_assert(v5);
}

TEST(SerializationTuple) {
  using check_type = std::tuple<std::vector<int>, double, float, std::pair<int, char>,
                                mtt::tuple<int, double, std::set<double>>>;
  std::vector<int> v1;
  v1.resize(5, 15);
  double v2 = 10;
  float v3 = 4;
  std::pair<int, char> v4(157, '0');
  std::set<double> v51;
  v51.insert(5);
  v51.insert(10);
  mtt::tuple v5(144, 143., v51);
  check_type value(v1, v2, v3, v4, v5);

  const std::string buffer = serialize<mode::single_machine>(value);
  auto result = deserialize<check_type, mode::single_machine>(buffer);
  verify(result == value);
}

TEST(StrictMode) {
  using serialize_type =
      std::tuple<mtt::tuple<std::pair<std::array<std::tuple<std::vector<double>, char, std::byte>, 15>,
                                         std::map<double, std::byte>>,
                               char8_t, char8_t>,
                 float>;
  serialize_type value{};
  auto& map = std::get<0>(value).get<0>().second;
  auto& arr = std::get<0>(value).get<0>().first;
  for (auto& vect : arr | std::views::elements<0>) {
    size_t sz = rand() % 20;
    for (size_t i = 0; i < sz; ++i) {
      vect.push_back(static_cast<double>(i * (rand() % 10)));
    }
  }
  map.insert(std::make_pair(20., std::byte(1)));
  map.insert(std::make_pair(25., std::byte(1)));

  std::vector<std::byte> buffer;
  serialize_to<mode::single_machine>(std::back_inserter(buffer), value);
  serialize_type result = deserialize<serialize_type, mode::single_machine>(buffer);
  verify(result == value);
}

TEST(HardCaseAssociativeRanges) {
  using test_t = std::pair<std::vector<std::map<int32_t, std::set<int32_t>>>, float>;
  test_t value;
  auto& [vect, f] = value;
  f = 3.14f;
  vect.resize(static_cast<size_t>(rand() % 10) + static_cast<size_t>(5));
  for (auto& map : vect) {
    int count = rand() % 10 + 1;
    for (int i = 0; i < count; ++i) {
      std::set<int> set;
      for (int j = 0; j < count; ++j) {
        set.insert(j * 10);
      }
      map.insert(std::make_pair(i, std::move(set)));
    }
  }
  std::vector<std::byte> buf;
  struct Magic {
    using enable_magic = void;
    constexpr auto operator<=>(const Magic&) const = default;
    int x;
    float y;
    double z;
  };
  const auto buffer_value = serialize<mode::network_with_int_aliases>(value);
  std::vector<Magic> vee;
  vee.resize(100, Magic(10, 20, 30));
  const auto buffer_vee = serialize<mode::network_with_int_aliases>(vee);
  const auto mvee = deserialize<std::vector<Magic>, mode::network_with_int_aliases>(buffer_vee);
  verify(mvee == vee);
  const auto result = deserialize<test_t, mode::network_with_int_aliases>(buffer_value);
  verify(result == value);
}
}  // namespace mtt::test

#endif  // !TEST_MTT_SERIALIZATION_HPP

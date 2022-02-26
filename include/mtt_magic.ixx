
module;
#include <climits>
export module mtt.tuple : magic;

import<functional>;  // only for placeholders lol

import mtt.traits;

namespace noexport {
// HACK for non error when _X identifier do not located in scope of type deductor
struct name_lookup_saver {};
constexpr name_lookup_saver _0;
constexpr name_lookup_saver _1;
constexpr name_lookup_saver _2;
constexpr name_lookup_saver _3;
constexpr name_lookup_saver _4;
constexpr name_lookup_saver _5;
constexpr name_lookup_saver _6;
constexpr name_lookup_saver _7;
constexpr name_lookup_saver _8;
constexpr name_lookup_saver _9;
constexpr name_lookup_saver _10;
constexpr name_lookup_saver _11;
constexpr name_lookup_saver _12;
constexpr name_lookup_saver _13;
constexpr name_lookup_saver _14;
constexpr name_lookup_saver _15;
constexpr name_lookup_saver _16;
constexpr name_lookup_saver _17;
constexpr name_lookup_saver _18;
constexpr name_lookup_saver _19;
constexpr name_lookup_saver _20;
constexpr name_lookup_saver _21;
constexpr name_lookup_saver _22;
constexpr name_lookup_saver _23;
constexpr name_lookup_saver _24;
constexpr name_lookup_saver _25;
constexpr name_lookup_saver _26;
constexpr name_lookup_saver _27;
constexpr name_lookup_saver _28;
constexpr name_lookup_saver _29;

template <typename T>
concept magicable = std::is_aggregate_v<T> && !std::is_empty_v<T>;

template <size_t>
struct any_type {
  template <typename T>
  operator T&();
  template <typename T>
  operator T&&();
};

template <typename T, size_t... Is>
consteval auto field_count(std::index_sequence<Is...>) {
  if constexpr (std::is_constructible_v<T, any_type<Is>...>) {
    return sizeof...(Is);
  } else {
    return field_count<T>(std::make_index_sequence<sizeof...(Is) - 1>{});
  }
}

template <typename... Args>
struct view_tuple : std::tuple<Args&...> {
 private:
  using base_t = std::tuple<Args&...>;

 public:
  using base_t::base_t;
  using list = mtt::type_list<Args...>;
};

#define DEDUCT_TYPE(index)                                                                                  \
  template <typename U>                                                                                     \
  struct type_deductor<index, U> {                                                                          \
    using type = std::remove_reference_t<decltype([]<typename T>(T&& value) {                               \
      constexpr auto sz = tuple_size_v<T>;                                                            \
      if constexpr (sz == 1 && index <= 1) {                                                                \
        auto&& [_0] = std::forward<T>(value);                                                               \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 2 && index <= 2) {                                                         \
        auto&& [_0, _1] = std::forward<T>(value);                                                           \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 3 && index <= 3) {                                                         \
        auto&& [_0, _1, _2] = std::forward<T>(value);                                                       \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 4 && index <= 4) {                                                         \
        auto&& [_0, _1, _2, _3] = std::forward<T>(value);                                                   \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 5 && index <= 5) {                                                         \
        auto&& [_0, _1, _2, _3, _4] = std::forward<T>(value);                                               \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 6 && index <= 6) {                                                         \
        auto&& [_0, _1, _2, _3, _4, _5] = std::forward<T>(value);                                           \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 7 && index <= 7) {                                                         \
        auto&& [_0, _1, _2, _3, _4, _5, _6] = std::forward<T>(value);                                       \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 8 && index <= 8) {                                                         \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7] = std::forward<T>(value);                                   \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 9 && index <= 9) {                                                         \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8] = std::forward<T>(value);                               \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 10 && index <= 10) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9] = std::forward<T>(value);                           \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 11 && index <= 11) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10] = std::forward<T>(value);                      \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 12 && index <= 12) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11] = std::forward<T>(value);                 \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 13 && index <= 13) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12] = std::forward<T>(value);            \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 14 && index <= 14) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13] = std::forward<T>(value);       \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 15 && index <= 15) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14] = std::forward<T>(value);  \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 16 && index <= 16) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15] =                     \
            std::forward<T>(value);                                                                         \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 17 && index <= 17) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16] =                \
            std::forward<T>(value);                                                                         \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 18 && index <= 18) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17] =           \
            std::forward<T>(value);                                                                         \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 19 && index <= 19) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18] =      \
            std::forward<T>(value);                                                                         \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 20 && index <= 20) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19] = \
            std::forward<T>(value);                                                                         \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 21 && index <= 21) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20] = std::forward<T>(value);                                                              \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 22 && index <= 22) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20, _21] = std::forward<T>(value);                                                         \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 23 && index <= 23) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20, _21, _22] = std::forward<T>(value);                                                    \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 24 && index <= 24) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20, _21, _22, _23] = std::forward<T>(value);                                               \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 25 && index <= 25) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20, _21, _22, _23, _24] = std::forward<T>(value);                                          \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 26 && index <= 26) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20, _21, _22, _23, _24, _25] = std::forward<T>(value);                                     \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 27 && index <= 27) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20, _21, _22, _23, _24, _25, _26] = std::forward<T>(value);                                \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 28 && index <= 28) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20, _21, _22, _23, _24, _25, _26, _27] = std::forward<T>(value);                           \
        return std::type_identity<decltype(_##index)>{};                                                    \
      } else if constexpr (sz == 29 && index <= 29) {                                                       \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,   \
                _20, _21, _22, _23, _24, _25, _26, _27, _28] = std::forward<T>(value);                      \
        return std::type_identity<decltype(_##index)>{};                                                    \
      }                                                                                                     \
    }(std::declval<U>()))>::type;                                                                           \
  }
}  // namespace noexport
using namespace noexport;

namespace std {
template <size_t I, typename... Args>
struct tuple_element<I, ::noexport::view_tuple<Args...>> {
  using type = typename ::noexport::view_tuple<Args...>::list::template get<I>;
};
template <typename... Args>
struct tuple_size<::noexport::view_tuple<Args...>> : integral_constant<size_t, sizeof...(Args)> {};

// view tuple, so even rvalue tuple can return lvalue& from get
// accepted by value, because view...
template <size_t I, typename... Args>
constexpr auto& get(::noexport::view_tuple<Args...> tpl) noexcept {
  return ::std::get<I>(static_cast<::std::tuple<Args&...>&>(tpl));
}
}  // namespace std

export namespace mtt {

// CONCEPT tuple_alike
template <typename T>
concept tuple_alike = requires(T) {
  typename std::tuple_size<std::remove_cvref_t<T>>;
  typename std::tuple_element_t<0, std::remove_cvref_t<T>>;
};

// customization point enable_magic_for
template <typename T>
struct enable_magic_for : std::false_type {};

template <typename T>
concept magic_tuple_alike =
    !tuple_alike<std::remove_cvref_t<T>> && magicable<std::remove_cvref_t<T>> && (requires(T) {
      typename std::remove_cvref_t<T>::enable_magic;
    } || enable_magic_for<std::remove_cvref_t<T>>::value);

// TEMPLATE VARIABLE tuple_size_v

template <typename T>
constexpr size_t tuple_size_v =  // 30 supported, but bit fields possible
    field_count<std::remove_cvref_t<T>>(std::make_index_sequence<50>{});

// TEMPLATE FUNCTION magic_get

#define frwd(name) std::forward<decltype(name)>(name)

// do not work for C arrays in aggregates
template <size_t Index, magic_tuple_alike T>
constexpr auto& magic_get(T&& value) noexcept {
  constexpr auto sz = tuple_size_v<T>;

  if constexpr (sz == 1) {
    auto&& [_0] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_0)));
    return getted_value;
  } else if constexpr (sz == 2) {
    auto&& [_0, _1] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_0), frwd(_1)));
    return getted_value;
  } else if constexpr (sz == 3) {
    auto&& [_0, _1, _2] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2)));
    return getted_value;
  } else if constexpr (sz == 4) {
    auto&& [_0, _1, _2, _3] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3)));
    return getted_value;
  } else if constexpr (sz == 5) {
    auto&& [_0, _1, _2, _3, _4] = std::forward<T>(value);
    auto&& getted_value =
        std::get<Index>(std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4)));
    return getted_value;
  } else if constexpr (sz == 6) {
    auto&& [_0, _1, _2, _3, _4, _5] = std::forward<T>(value);
    auto&& getted_value =
        std::get<Index>(std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5)));
    return getted_value;
  } else if constexpr (sz == 7) {
    auto&& [_0, _1, _2, _3, _4, _5, _6] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6)));
    return getted_value;
  } else if constexpr (sz == 8) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3),
                                                                frwd(_4), frwd(_5), frwd(_6), frwd(_7)));
    return getted_value;
  } else if constexpr (sz == 9) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8)));
    return getted_value;
  } else if constexpr (sz == 10) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9)));
    return getted_value;
  } else if constexpr (sz == 11) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10] = std::forward<T>(value);
    auto&& getted_value =
        std::get<Index>(std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5),
                                              frwd(_6), frwd(_7), frwd(_8), frwd(_9), frwd(_10)));
    return getted_value;
  } else if constexpr (sz == 12) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11] = std::forward<T>(value);
    auto&& getted_value =
        std::get<Index>(std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5),
                                              frwd(_6), frwd(_7), frwd(_8), frwd(_9), frwd(_10), frwd(_11)));
    return getted_value;
  } else if constexpr (sz == 13) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7),
                              frwd(_8), frwd(_9), frwd(_10), frwd(_11), frwd(_12)));
    return getted_value;
  } else if constexpr (sz == 14) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7),
                              frwd(_8), frwd(_9), frwd(_10), frwd(_11), frwd(_12), frwd(_13)));
    return getted_value;
  } else if constexpr (sz == 15) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7),
                              frwd(_8), frwd(_9), frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14)));
    return getted_value;
  } else if constexpr (sz == 16) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15)));
    return getted_value;
  } else if constexpr (sz == 17) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16] =
        std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16)));
    return getted_value;
  } else if constexpr (sz == 18) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17] =
        std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16), frwd(_17)));
    return getted_value;
  } else if constexpr (sz == 19) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18] =
        std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16), frwd(_17), frwd(_18)));
    return getted_value;
  } else if constexpr (sz == 20) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19] =
        std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7),
                              frwd(_8), frwd(_9), frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14),
                              frwd(_15), frwd(_16), frwd(_17), frwd(_18), frwd(_19)));
    return getted_value;
  } else if constexpr (sz == 21) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20] =
        std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7),
                              frwd(_8), frwd(_9), frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14),
                              frwd(_15), frwd(_16), frwd(_17), frwd(_18), frwd(_19), frwd(_20)));
    return getted_value;
  } else if constexpr (sz == 22) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,
            _21] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7),
                              frwd(_8), frwd(_9), frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14),
                              frwd(_15), frwd(_16), frwd(_17), frwd(_18), frwd(_19), frwd(_20), frwd(_21)));
    return getted_value;
  } else if constexpr (sz == 23) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,
            _21, _22] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16), frwd(_17), frwd(_18),
        frwd(_19), frwd(_20), frwd(_21), frwd(_22)));
    return getted_value;
  } else if constexpr (sz == 24) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,
            _21, _22, _23] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16), frwd(_17), frwd(_18),
        frwd(_19), frwd(_20), frwd(_21), frwd(_22), frwd(_23)));
    return getted_value;
  } else if constexpr (sz == 25) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,
            _21, _22, _23, _24] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16), frwd(_17), frwd(_18),
        frwd(_19), frwd(_20), frwd(_21), frwd(_22), frwd(_23), frwd(_24)));
    return getted_value;
  } else if constexpr (sz == 26) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,
            _21, _22, _23, _24, _25] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16), frwd(_17), frwd(_18),
        frwd(_19), frwd(_20), frwd(_21), frwd(_22), frwd(_23), frwd(_24), frwd(_25)));
    return getted_value;
  } else if constexpr (sz == 27) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,
            _21, _22, _23, _24, _25, _26] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16), frwd(_17), frwd(_18),
        frwd(_19), frwd(_20), frwd(_21), frwd(_22), frwd(_23), frwd(_24), frwd(_25), frwd(_26)));
    return getted_value;
  } else if constexpr (sz == 28) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,
            _21, _22, _23, _24, _25, _26, _27] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9),
        frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14), frwd(_15), frwd(_16), frwd(_17), frwd(_18),
        frwd(_19), frwd(_20), frwd(_21), frwd(_22), frwd(_23), frwd(_24), frwd(_25), frwd(_26), frwd(_27)));
    return getted_value;
  } else if constexpr (sz == 29) {
    auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,
            _21, _22, _23, _24, _25, _26, _27, _28] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_0), frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7),
                              frwd(_8), frwd(_9), frwd(_10), frwd(_11), frwd(_12), frwd(_13), frwd(_14),
                              frwd(_15), frwd(_16), frwd(_17), frwd(_18), frwd(_19), frwd(_20), frwd(_21),
                              frwd(_22), frwd(_23), frwd(_24), frwd(_25), frwd(_26), frwd(_27), frwd(_28)));
    return getted_value;
  }
}

// TEMPLATE FUNCTION magic_view(converts aggregate into special tuple
// of & on aggregate fields with stored types info about aggregate)

template <typename T>
constexpr auto magic_view(T&& value) noexcept {
  return [&]<size_t... Is>(std::index_sequence<Is...>) {
    return view_tuple<tuple_element_t<Is, std::remove_reference_t<T>>...>(
        magic_get<Is>(std::forward<T>(value))...);
  } // INVOKE HERE
  (std::make_index_sequence<tuple_size_v<T>>{});
}

// TRAIT tuple_element_t

template <size_t I, typename T>
struct type_deductor;

DEDUCT_TYPE(0);
DEDUCT_TYPE(1);
DEDUCT_TYPE(2);
DEDUCT_TYPE(3);
DEDUCT_TYPE(4);
DEDUCT_TYPE(5);
DEDUCT_TYPE(6);
DEDUCT_TYPE(7);
DEDUCT_TYPE(8);
DEDUCT_TYPE(9);
DEDUCT_TYPE(10);
DEDUCT_TYPE(11);
DEDUCT_TYPE(12);
DEDUCT_TYPE(13);
DEDUCT_TYPE(14);
DEDUCT_TYPE(15);
DEDUCT_TYPE(16);
DEDUCT_TYPE(17);
DEDUCT_TYPE(18);
DEDUCT_TYPE(19);
DEDUCT_TYPE(20);
DEDUCT_TYPE(21);
DEDUCT_TYPE(22);
DEDUCT_TYPE(23);
DEDUCT_TYPE(24);
DEDUCT_TYPE(25);
DEDUCT_TYPE(26);
DEDUCT_TYPE(27);
DEDUCT_TYPE(28);
DEDUCT_TYPE(29);

template <size_t I, typename T>
using tuple_element_t = typename type_deductor<I, std::remove_reference_t<T>>::type;

// TEMPLATE FUNCTION magic(converts aggrevate into tuple)

template <magic_tuple_alike T>
constexpr auto magic(T&& value) {
  return []<typename U, size_t... Is>(U && value_, std::index_sequence<Is...>) {
    return std::tuple<tuple_element_t<Is, U>...>(magic_get<Is>(std::forward<T>(value_))...);
  } // INVOKE HERE
  (std::forward<T>(value), std::make_index_sequence<tuple_size_v<T>>{});
}

}  // namespace mtt

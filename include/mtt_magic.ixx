
module;
#include <climits>
export module mtt.tuple : magic;

import<functional>;  // only for placeholders lol

import mtt.traits;

namespace noexport {
// HACK for non error when _X identifier do not located in scope of type deductor
using namespace std::placeholders;

constexpr inline unsigned max_supported_aggregate_size = 10;

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

#define DEDUCT_TYPE(index)                                                        \
  template <typename U>                                                           \
  struct type_deductor<index, U> {                                                \
    using type = std::remove_reference_t<decltype([]<typename T>(T&& value) {     \
      constexpr auto sz = magic_tuple_size_v<T>;                                  \
      if constexpr (sz == 1 && index <= 1) {                                      \
        auto&& [_0] = std::forward<T>(value);                                     \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == 2 && index <= 2) {                               \
        auto&& [_0, _1] = std::forward<T>(value);                                 \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == 3 && index <= 3) {                               \
        auto&& [_0, _1, _2] = std::forward<T>(value);                             \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == 4 && index <= 4) {                               \
        auto&& [_0, _1, _2, _3] = std::forward<T>(value);                         \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == 5 && index <= 5) {                               \
        auto&& [_0, _1, _2, _3, _4] = std::forward<T>(value);                     \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == 6 && index <= 6) {                               \
        auto&& [_0, _1, _2, _3, _4, _5] = std::forward<T>(value);                 \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == 7 && index <= 7) {                               \
        auto&& [_0, _1, _2, _3, _4, _5, _6] = std::forward<T>(value);             \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == 8 && index <= 8) {                               \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7] = std::forward<T>(value);         \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == 9 && index <= 9) {                               \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8] = std::forward<T>(value);     \
        return std::type_identity<decltype(_##index)>{};                          \
      } else if constexpr (sz == max_supported_aggregate_size && index <= 10) {   \
        auto&& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9] = std::forward<T>(value); \
        return std::type_identity<decltype(_##index)>{};                          \
      }                                                                           \
    }(std::declval<U>()))>::type;                                                 \
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

// TEMPLATE VARIABLE magic_tuple_size_v

template <typename T>
constexpr size_t magic_tuple_size_v =
    field_count<std::remove_cvref_t<T>>(std::make_index_sequence<sizeof(T) * CHAR_BIT>{});

// TEMPLATE FUNCTION get(magic)

#define frwd(name) std::forward<decltype(name)>(name)

// do not work for C arrays in aggregates
template <size_t Index, magic_tuple_alike T>
constexpr auto& magic_get(T&& value) noexcept {
  constexpr auto sz = magic_tuple_size_v<T>;

  if constexpr (sz == 1) {
    auto&& [_1] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_1)));
    return getted_value;
  } else if constexpr (sz == 2) {
    auto&& [_1, _2] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_1), frwd(_2)));
    return getted_value;
  } else if constexpr (sz == 3) {
    auto&& [_1, _2, _3] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_1), frwd(_2), frwd(_3)));
    return getted_value;
  } else if constexpr (sz == 4) {
    auto&& [_1, _2, _3, _4] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_1), frwd(_2), frwd(_3), frwd(_4)));
    return getted_value;
  } else if constexpr (sz == 5) {
    auto&& [_1, _2, _3, _4, _5] = std::forward<T>(value);
    auto&& getted_value =
        std::get<Index>(std::forward_as_tuple(frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5)));
    return getted_value;
  } else if constexpr (sz == 6) {
    auto&& [_1, _2, _3, _4, _5, _6] = std::forward<T>(value);
    auto&& getted_value =
        std::get<Index>(std::forward_as_tuple(frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6)));
    return getted_value;
  } else if constexpr (sz == 7) {
    auto&& [_1, _2, _3, _4, _5, _6, _7] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(
        std::forward_as_tuple(frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7)));
    return getted_value;
  } else if constexpr (sz == 8) {
    auto&& [_1, _2, _3, _4, _5, _6, _7, _8] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(frwd(_1), frwd(_2), frwd(_3), frwd(_4),
                                                                frwd(_5), frwd(_6), frwd(_7), frwd(_8)));
    return getted_value;
  } else if constexpr (sz == 9) {
    auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9)));
    return getted_value;
  } else if constexpr (sz == max_supported_aggregate_size) {
    auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10] = std::forward<T>(value);
    auto&& getted_value = std::get<Index>(std::forward_as_tuple(
        frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9), frwd(_10)));
    return getted_value;
  }
}

// TEMPLATE FUNCTION magic_view(converts aggregate into special tuple
// of & on aggregate fields with stored types info about aggregate)

template <typename T>
constexpr auto magic_view(T&& value) noexcept {
  constexpr auto sz = magic_tuple_size_v<T>;
#define t(x) decltype(x)
#undef frwd
#define frwd(x) x
  if constexpr (sz == 1) {
    auto&& [_1] = std::forward<T>(value);
    return view_tuple<t(_1)>(frwd(_1));
  } else if constexpr (sz == 2) {
    auto&& [_1, _2] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2)>(frwd(_1), frwd(_2));
  } else if constexpr (sz == 3) {
    auto&& [_1, _2, _3] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2), t(_3)>(frwd(_1), frwd(_2), frwd(_3));
  } else if constexpr (sz == 4) {
    auto&& [_1, _2, _3, _4] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2), t(_3), t(_4)>(frwd(_1), frwd(_2), frwd(_3), frwd(_4));
  } else if constexpr (sz == 5) {
    auto&& [_1, _2, _3, _4, _5] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2), t(_3), t(_4), t(_5)>(frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5));
  } else if constexpr (sz == 6) {
    auto&& [_1, _2, _3, _4, _5, _6] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2), t(_3), t(_4), t(_5), t(_6)>(frwd(_1), frwd(_2), frwd(_3), frwd(_4),
                                                                frwd(_5), frwd(_6));
  } else if constexpr (sz == 7) {
    auto&& [_1, _2, _3, _4, _5, _6, _7] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2), t(_3), t(_4), t(_5), t(_6), t(_7)>(frwd(_1), frwd(_2), frwd(_3), frwd(_4),
                                                                       frwd(_5), frwd(_6), frwd(_7));
  } else if constexpr (sz == 8) {
    auto&& [_1, _2, _3, _4, _5, _6, _7, _8] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2), t(_3), t(_4), t(_5), t(_6), t(_7), t(_8)>(
        frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8));
  } else if constexpr (sz == 9) {
    auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2), t(_3), t(_4), t(_5), t(_6), t(_7), t(_8), t(_9)>(
        frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9));
  } else if constexpr (sz == max_supported_aggregate_size) {
    auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10] = std::forward<T>(value);
    return view_tuple<t(_1), t(_2), t(_3), t(_4), t(_5), t(_6), t(_7), t(_8), t(_9), t(_10)>(
        frwd(_1), frwd(_2), frwd(_3), frwd(_4), frwd(_5), frwd(_6), frwd(_7), frwd(_8), frwd(_9), frwd(_10));
  }
}

// TRAIT magic_element_t

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

template <size_t I, typename T>
using magic_element_t = typename type_deductor<I, std::remove_reference_t<T>>::type;

// TEMPLATE FUNCTION magic(converts aggrevate into tuple)

template <magic_tuple_alike T>
constexpr auto magic(T&& value) {
  return []<typename U, size_t... Is>(U && value_, std::index_sequence<Is...>) {
    return std::tuple<magic_element_t<Is, U>...>(magic_get<Is>(std::forward<T>(value_))...);
  }
  (std::forward<T>(value), std::make_index_sequence<magic_tuple_size_v<T>>{});
}

}  // namespace mtt

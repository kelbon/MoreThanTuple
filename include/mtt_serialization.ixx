

export module mtt.serialization;

import<ranges>;
import<algorithm>;

import mtt.tuple;
import mtt.traits;

namespace noexport {
template <typename T>
struct find_final_underlying {
 private:
  template <typename U>
  static consteval auto remove_layer_by_layer() noexcept {
    if constexpr (std::ranges::range<U>)
      return remove_layer_by_layer<std::ranges::range_value_t<U>>();
    else
      return std::type_identity<U>{};
  }

 public:
  using type = typename decltype(remove_layer_by_layer<T>())::type;
};

// for cases when C array of C++ containers, which containts C array etc etc...
// example : std::vector<int[10][15]> bad_thing[2][3] -> int
template <typename T>
using final_underlying_t =
    std::remove_all_extents_t<typename find_final_underlying<std::remove_all_extents_t<T>>::type>;

template <typename T>
concept associative_range = std::ranges::sized_range<T> && requires(T) {
  typename std::remove_reference_t<T>::key_type;
};
template <typename T>
concept have_reserve = requires(T value) {
  value.reserve(10);
};

}  // namespace noexport
using namespace noexport;

export namespace mtt {
// always write in in little endian for perfomance(but works for big endian too)

using size_of_range_t = std::uintmax_t;

// CONCEPT runtime_sized_range - true for types like std::vector<int>
template <typename T>
consteval bool is_compile_time_sized() noexcept {
  if constexpr (magic_tuple_alike<T>) {
    return is_compile_time_sized<std::remove_reference_t<decltype(magic(std::declval<T>()))>>();
  }
  if constexpr (std::ranges::range<T>) {
    if constexpr (std::ranges::sized_range<T>) {
      if constexpr (tuple_alike<T> && is_compile_time_sized<std::ranges::range_value_t<T>>()) {
        return true;
      } else {  // runtime sized type
        return false;
      }
    } else {  // unreachable end of range
      return false;
    }
  } else if constexpr (tuple_alike<T>) {
    return apply(
        []<typename... Types>(std::type_identity<Types>...) {
          return ((is_compile_time_sized<Types>()) && ...);
        },
        extract_type_list_from_tuplealike<T>{});
  } else {  // not range && not tuple alike (end of recursion)
    return true;
  }
}

template <typename T>
concept runtime_sized_range = std::ranges::sized_range<T> && !is_compile_time_sized<std::remove_cvref_t<T>>();

// cannot be calculated on compile time bcs may be std::vector or smth
template <typename T>
constexpr size_t sizeof_for_binary_serialization(T&& value) {
  using Thing = std::remove_reference_t<T>;
  if constexpr (magic_tuple_alike<Thing>) {
    return sizeof_for_binary_serialization(magic(std::forward<T>(value)));
  }
  size_t result = 0;
  if constexpr (std::ranges::range<Thing>) {
    if constexpr (std::ranges::sized_range<Thing>) {
      if constexpr (runtime_sized_range<Thing>) {
        const auto range_size = std::ranges::size(std::forward<T>(value));
        for (auto&& v : std::forward<T>(value)) {
          result += sizeof_for_binary_serialization(v);
        }
        result += sizeof(size_of_range_t);
        return result;
      } else if constexpr (tuple_alike<Thing>) {  // compile time known size
        return apply(
            []<typename... T>(T && ... args) {
              return (sizeof_for_binary_serialization(std::forward<T>(args)) + ...);
            },
            std::forward<T>(value));
      } else {
        static_assert(always_false<T>(), "Must be never here");
      }
    } else {
      static_assert(always_false<T>, "Unreachable end of range");
    }
  } else if constexpr (tuple_alike<Thing>) {
    result = 0;
    ::mtt::visit_recursive(std::forward<T>(value), [&result]<typename U>(U&& v) {
      result += sizeof_for_binary_serialization(std::forward<U>(v));
    });
    return result;
  } else {  // Thing is just a type
    return sizeof(Thing);
  }
}
// clang-format off
template <typename T>
requires(is_compile_time_sized<T>)
consteval size_t sizeof_for_binary_serialization() {
  // clang-format on
  using Thing = std::remove_reference_t<T>;
  if constexpr (magic_tuple_alike<Thing>)
    return sizeof_for_binary_serialization<decltype(magic(std::declval<T>()))>();
  if constexpr (tuple_alike<Thing>)
    return apply(
        []<typename... Types>(std::type_identity<Types> && ...) {
          return (sizeof_for_binary_serialization<Types>() + ...);
        },
        extract_type_list_from_tuplealike<Thing>{});
  else
    return sizeof(Thing);
}

enum class mode {
  // when you know, that reader will be in same programm or on same OS and same compiler(same sizes for types)
  single_machine,
  // you dont have any additional guarantees
  network,
  // by using this you confirm that the int types that you passed for serialization will have
  // the same size for the reader as for the sender. For example std::int32_t, std::uint16_t etc
  network_with_int_aliases
};

// CONCEPT serializable

template <typename T>  // std::integral for overloading(it must be stricter requirement)
concept guaranteed_size_integral_type = std::integral<T> && one_of < std::remove_cv_t<T>,
bool, char, signed char, unsigned char, char8_t, char16_t, char32_t > ;

template <typename T>
concept guaranteed_size_enum = enumeration<T> && guaranteed_size_integral_type<std::underlying_type_t<T>>;

template <typename T>
concept guaranteed_size_type = guaranteed_size_integral_type<T> || guaranteed_size_enum<T>;

template <typename T>
concept serializable_base_requirement =
    std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T> && !std::is_pointer_v<T> &&
    !std::is_empty_v<T> && !std::is_null_pointer_v<T> && !std::is_reference_v<T>;

// checks if it just serializable type or range of such type or range of ranges of such type etc (or C
// array...). For cases with ranges with tuple alike value types they are NOT serializable by that thing
template <typename T, mode Mode>
concept serializable_not_pack =
    !tuple_alike<T> && !magic_tuple_alike<T> && serializable_base_requirement<final_underlying_t<T>> &&
    (std::floating_point<final_underlying_t<T>> ||
     (  // some boolean algebra - applies only one of three requirements depending on write mode
         (Mode == mode::network &&
          guaranteed_size_type<final_underlying_t<T>>) ||  // good while only integral type can be under enum
         (Mode == mode::network_with_int_aliases &&
          (std::integral<final_underlying_t<T>> || enumeration<final_underlying_t<T>>)) ||
         Mode == mode::single_machine  // no additional requirement for this mode
         ));

template <typename T, mode Mode>
consteval bool tuple_alike_of_serializable_types_recursive() noexcept {
  if constexpr (magic_tuple_alike<T>) {
    return tuple_alike_of_serializable_types_recursive<
        std::remove_reference_t<decltype(magic(std::declval<T>()))>, Mode>();
  }
  if constexpr (tuple_alike<T>) {
    return []<typename... Types>(type_list<Types...>) {
      return ((tuple_alike_of_serializable_types_recursive<Types, Mode>()) && ...);
    }
    (extract_type_list_from_tuplealike<T>());  // INVOKE HERE
  } else if constexpr (std::ranges::range<T>) {
    return tuple_alike_of_serializable_types_recursive<final_underlying_t<T>, Mode>();
  } else {  // T is just a type
    return serializable_not_pack<T, Mode>;
  }
}

// tuple alike stores serializable values or tuple of tuple alikes, which stores such types or range with of
// such types
template <typename T, mode Mode>
concept set_of_serializable_types =
    (tuple_alike<T> || std::ranges::range<T> ||
     magic_tuple_alike<T>)&&tuple_alike_of_serializable_types_recursive<T, Mode>();

template <typename T, mode Mode>
concept serializable = serializable_not_pack<std::remove_cvref_t<T>, Mode> ||
    set_of_serializable_types<std::remove_cvref_t<T>, Mode>;

// TEMPLATE FUNCTIONS serialize / serialize_to

template <mode Mode, typename OutIter, typename T>
constexpr bool value_serializator(OutIter& out_iter, T&& value) noexcept {
  using Thing = std::remove_reference_t<T>;
  if constexpr (magic_tuple_alike<Thing>) {
    return value_serializator<Mode>(out_iter, magic(std::forward<T>(value)));
  }
  if constexpr (std::ranges::range<Thing>) {
    if constexpr (std::ranges::sized_range<Thing>) {
      if constexpr (tuple_alike<Thing>) {
        // do not write size of range, its useless
      } else {  // write size of range for reading
        value_serializator<Mode>(out_iter, size_of_range_t(std::ranges::size(std::forward<T>(value))));
      }
      bool result = true;
      for (auto&& v : std::views::all(std::forward<T>(value))) {
        result = result && value_serializator<Mode>(out_iter, std::forward<decltype(v)>(v));
      }
      return result;
    } else {
      static_assert(always_false<T>, "Unreachable end of range");
    }
  } else if constexpr (tuple_alike<Thing>) {
    bool result = true;
    ::mtt::visit_recursive(std::forward<T>(value), [&](auto&& v) noexcept {
      result = result && value_serializator<Mode>(out_iter, std::forward<decltype(v)>(v));
    });
    return result;
  } else {  // Thing is just a type
    using iter_value_type = decltype(std::invoke([]() consteval {
      if constexpr (std::output_iterator<OutIter, char>)
        return std::type_identity<const char>{};
      else if constexpr (std::output_iterator<OutIter, unsigned char>)
        return std::type_identity<const unsigned char>{};
      else if constexpr (std::output_iterator<OutIter, std::byte>)
        return std::type_identity<const std::byte>{};
      else
        static_assert(always_false<T>);
    }))::type;
    auto* begin = reinterpret_cast<iter_value_type*>(std::addressof(value));
    if constexpr (std::endian::native == std::endian::little)
      out_iter = std::copy(begin, begin + sizeof(Thing), out_iter);
    else if constexpr (std::endian::native == std::endian::big)  // always write in little endian
      out_iter = std::reverse_copy(begin, begin + sizeof(Thing), out_iter);
    else
      static_assert(always_false<Thing>, "mixed endian is not supported");
    return true;
  }
}

// clang-format off
template <mode Mode = mode::network, typename OutIter, serializable<Mode> T>
requires(std::output_iterator<OutIter, char>
|| std::output_iterator<OutIter, std::byte>
|| std::output_iterator<OutIter, unsigned char>)
constexpr void serialize_to(OutIter out_iter, T&& value) {
  // clang-format on
  value_serializator<Mode>(out_iter, std::forward<T>(value));
}

template <mode Mode = mode::network, serializable<Mode> T>
constexpr std::string serialize(T&& value) {
  std::string result;
  if constexpr (is_compile_time_sized<T>()) {
    constexpr auto required_size = sizeof_for_binary_serialization<T>();
    result.reserve(required_size);
  } else {
    result.reserve(sizeof_for_binary_serialization(std::forward<T>(value)));
  }
  auto out_iter = std::back_inserter(result);
  serialize_to<Mode>(out_iter, std::forward<T>(value));
  return result;
}

// TEMPLATE FUNCTION deserialize

template <mode Mode, std::input_iterator InIter, typename T>
constexpr bool value_deserializator(InIter& in_iter, T& value) noexcept {
  using Thing = std::remove_reference_t<T>;

  if constexpr (magic_tuple_alike<Thing>) {
    auto magic_tuple = magic_view(value);
    bool res = value_deserializator<Mode>(in_iter, magic_tuple);
    if (res == true) [[likely]] {
      [&value, &magic_tuple ]<size_t... Is>(std::index_sequence<Is...>) {
        ((magic_get<Is>(value) = std::get<Is>(magic_tuple)), ...);
      }
      (std::make_index_sequence<magic_tuple_size_v<std::remove_reference_t<T>>>{});
    }
    return res;
  }
  if constexpr (std::ranges::range<Thing>) {
    if constexpr (std::ranges::sized_range<Thing>) {
      if constexpr (tuple_alike<Thing>) {  // range which knowns size on compile time (array for example)
        bool result = true;
        for (auto&& v : std::views::all(value)) {
          result = result && value_deserializator<Mode>(in_iter, std::forward<decltype(v)>(v));
        }
        return result;
      } else {
        static_assert(std::is_default_constructible_v<std::ranges::range_value_t<Thing>>);

        size_of_range_t range_size = 0;
        value_deserializator<Mode>(in_iter, range_size);  // reading size

        if constexpr (have_reserve<Thing>) {
          value.reserve(range_size);
        }
        if constexpr (associative_range<Thing>) {
          if constexpr (tuple_alike<std::ranges::range_value_t<Thing>>) {  // maps or multiindex containers
                                                                           // etc
            using key_t = std::remove_const_t<typename Thing::key_type>;
            using tpl_args_non_const_key = merge_type_lists_t<
                type_list<key_t>,
                remove_first_arg_t<extract_type_list_from_tuplealike<std::ranges::range_value_t<Thing>>>>;
            using value_for_reading_t = insert_type_list_t<std::tuple, tpl_args_non_const_key>;

            for (size_of_range_t i = 0; i < range_size; ++i) {
              value_for_reading_t value_for_reading;
              value_deserializator<Mode>(in_iter, value_for_reading);
              std::forward<T>(value).emplace(
                  ::mtt::make_from_tuple<std::ranges::range_value_t<Thing>>(value_for_reading));
            }
          } else {  // sets or smth with only key
            std::remove_const_t<typename Thing::key_type> value_for_reading;
            for (size_of_range_t i = 0; i < range_size; ++i) {
              value_deserializator<Mode>(in_iter, value_for_reading);
              value.emplace(std::move(value_for_reading));
            }
          }
        } else {
          for (size_of_range_t i = 0; i < range_size; ++i) {
            std::ranges::range_value_t<Thing> value_for_reading;
            value_deserializator<Mode>(in_iter, value_for_reading);
            value.push_back(value_for_reading);
          }
        }
        return true;
      }
    } else {  // not sized range
      static_assert(always_false<T>, "Unreachable end of range");
    }
  } else if constexpr (tuple_alike<Thing>) {
    bool result = true;
    ::mtt::visit_recursive(value, [&](auto&& v) {
      result = result && value_deserializator<Mode>(in_iter, std::forward<decltype(v)>(v));
    });
    return result;
  } else {  // Thing is just a type
    auto begin = in_iter;
    std::advance(in_iter, sizeof(Thing));
    if constexpr (std::endian::native == std::endian::little)
      std::copy(begin, in_iter, reinterpret_cast<typename std::iterator_traits<InIter>::value_type*>(std::addressof(value)));
    else if constexpr (std::endian::native == std::endian::big)  // always write in little endian
      std::reverse_copy(begin, in_iter, reinterpret_cast<typename std::iterator_traits<InIter>::value_type*>(std::addressof(value)));
    else
      static_assert(always_false<Thing>, "mixed endian is not supported");
    return true;
  }
}

// clang-format off
template <typename T, mode Mode = mode::network, std::ranges::range Range>
requires(serializable<T, Mode> && one_of<std::ranges::range_value_t<Range>, std::byte, char, unsigned char>)
constexpr T deserialize(Range&& range, T init = T{}) { // init - possibility for no default constructible T
  // clang-format on
  T result = std::move(init);
  auto in_iter = std::ranges::begin(range);
  value_deserializator<Mode>(in_iter, result);
  return result;
}

}  // namespace mtt
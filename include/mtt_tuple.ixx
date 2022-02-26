
export module mtt.tuple;

export import<tuple>;
export import : magic;

import mtt.traits;

namespace noexport {

// value_in_tuple
template <typename T, size_t Index>
struct value_in_tuple {
  constexpr auto operator<=>(const value_in_tuple&) const = default;
  T value{};
};
// clang-format off
template <typename T, size_t Index> requires(std::is_empty_v<T>)
struct value_in_tuple<T, Index> {
  // clang-format on
  constexpr auto operator<=>(const value_in_tuple&) const = default;

  // fake constructor for same interface
  constexpr value_in_tuple(T) noexcept {
  }
  static constexpr T value = T{};
};
// tuple_alike_with_magic and magic_filter for integrating with mtt:magic
template <typename T>
concept tuple_alike_with_magic = mtt::tuple_alike<T> || mtt::magic_tuple_alike<T>;

template <typename T>
constexpr decltype(auto) magic_filter(T&& value) noexcept {
  if constexpr (mtt::magic_tuple_alike<T>)
    return mtt::magic_tie(std::forward<T>(value));
  else
    return std::forward<T>(value);
}

// types_extractor
template <mtt::tuple_alike T, size_t... Is>
consteval auto types_extractor(T&&, mtt::index_list<Is...>) noexcept {
  return mtt::type_list<std::tuple_element_t<Is, std::remove_cvref_t<T>>...>{};
}

}  // namespace noexport
using namespace noexport;

export namespace mtt {

// TEMPLATE tuple

template <typename...>
struct tuple_base;
#if defined(_MSC_VER)
#define MSVC_EMPTY_BASES_WORKAROUND __declspec(empty_bases)
#else
#define MSVC_EMPTY_BASES_WORKAROUND
#endif
template <size_t... Indexes, typename... Types>
struct MSVC_EMPTY_BASES_WORKAROUND tuple_base<std::index_sequence<Indexes...>, Types...>
    : value_in_tuple<Types, Indexes>... {
 private:
  using type_array = type_list<Types...>;

  template <size_t index>
  using get_type = typename type_array::template get<index>;

 public:
  constexpr auto operator<=>(const tuple_base&) const = default;

  template <size_t index>
  constexpr const get_type<index>& get() const& noexcept {
    return static_cast<const value_in_tuple<get_type<index>, index>* const>(this)->value;
  }
  template <size_t index>
  constexpr get_type<index>& get() & noexcept {
    return static_cast<value_in_tuple<get_type<index>, index>*>(this)->value;
  }
  template <size_t index>
  constexpr decltype(auto) get() && noexcept {
    return std::move(static_cast<value_in_tuple<get_type<index>, index>*>(this)->value);
  }
  template <size_t index>
  constexpr const get_type<index>&& get() const&& noexcept {
    return std::move(static_cast<value_in_tuple<get_type<index>, index>*>(this)->value);
  }
};

template <typename... Types>
struct tuple : tuple_base<std::make_index_sequence<sizeof...(Types)>, Types...> {
 private:
  using base_t = tuple_base<std::make_index_sequence<sizeof...(Types)>, Types...>;

 public:
  template <typename... Args>
  constexpr tuple(Args&&... args) : base_t{std::forward<Args>(args)...} {
  }
};

export template <typename... Types>
tuple(Types...) -> tuple<Types...>;

}  // namespace mtt

// structure binding

export namespace std {

template <size_t index, typename... Types>
struct tuple_element<index, ::mtt::tuple<Types...>> {
  using type = typename ::mtt::type_list<Types...>::template get<index>;
};
template <typename... Types>
struct tuple_size<::mtt::tuple<Types...>> {
  static constexpr size_t value = sizeof...(Types);
};
template <size_t index, ::mtt::instance_of<::mtt::tuple> KelbonTuple>
constexpr decltype(auto) get(KelbonTuple&& tupl) noexcept {
  return forward<KelbonTuple>(tupl).template get<index>();
}
template <typename T, ::mtt::instance_of<::mtt::tuple> KelbonTuple>
constexpr decltype(auto) get(KelbonTuple&& tupl) noexcept {
  using types_from_tuple = ::mtt::extract_type_list_t<KelbonTuple>;
  constexpr size_t check_summ = []<typename... Types>(::mtt::type_list<Types...>) {
    size_t result = 0;
    ((result += (is_same_v<T, Types> ? sizeof...(Types) * 1000 : result < sizeof...(Types) * 1000)), ...);
    return result;
  }
  (types_from_tuple{});
  constexpr size_t tuple_sz = types_from_tuple::size;
  constexpr size_t number_in_pack = check_summ % (tuple_sz * 1000);
  constexpr size_t count_in_pack = (check_summ - number_in_pack) / (tuple_sz * 1000);

  if constexpr (count_in_pack == 0) {
    static_assert(::mtt::always_false<T>(), "std::get for mtt::tuple : Types... do not contain T");
  } else if constexpr (count_in_pack > 1) {
    static_assert(::mtt::always_false<T>(),
                  "std::get for mtt::tuple : ambigious call Types... contain T more then 1 time");
  } else {  // count_in_pack == 1
    return forward<KelbonTuple>(tupl).template get<number_in_pack>();
  }
}

}  // namespace std

// TUPLE ALGORITHMS

export namespace mtt {

// TEMPLATE FUNCTION visit T - some tuple alike thing, F - functor with template operator() and 1 argument

template <tuple_alike_with_magic T, typename F>
constexpr void visit(T&& tpl, F&& todo) {
  if constexpr (magic_tuple_alike<T>) {
    return ::mtt::visit(magic_filter(std::forward<T>(tpl)), std::forward<F>(todo));
  } else {
    [&]<size_t... Is>(std::index_sequence<Is...>) {
      ((std::forward<F>(todo)(std::get<Is>(std::forward<T>(tpl)))), ...);
    }
    (std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<T>>>{});  // INVOKE HERE
  }
}

// TEMPLATE FUNCTION visit_resursive T - some tuple alike thing, F - functor with template operator() and 1
// argument

template <typename T, typename F>
constexpr void visit_recursive(T&& tpl, F&& todo) {
  if constexpr (magic_tuple_alike<T>) {
    return ::mtt::visit_recursive(magic_filter(std::forward<T>(tpl)), std::forward<F>(todo));
  } else {
    if constexpr (tuple_alike<T>) {
      [&]<size_t... Is>(std::index_sequence<Is...>) {
        (((::mtt::visit_recursive(std::get<Is>(std::forward<T>(tpl)), std::forward<F>(todo))), ...));
      }
      (std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<T>>>{});  // INVOKE HERE
    } else {
      std::forward<F>(todo)(std::forward<T>(tpl));
    }
  }
}

// TRAIT extract_type_list_from_tuplealike

template <tuple_alike T>
using extract_type_list_from_tuplealike = decltype(types_extractor(
    std::declval<T>(), make_index_list<std::tuple_size_v<std::remove_cvref_t<T>>>{}));

// TEMPLATE FUNCTION apply

template <tuple_alike_with_magic T, typename F>
constexpr decltype(auto) apply(F&& todo, T&& tpl) {
  if constexpr (magic_tuple_alike<T>) {
    return ::mtt::apply(std::forward<F>(todo), magic_filter(std::forward<T>(tpl)));
  } else {
    return []<size_t... Is>(T && ttpl, F && ttodo, index_list<Is...>) {
      return std::forward<F>(ttodo)(std::get<Is>(std::forward<T>(ttpl))...);
    }
    (std::forward<T>(tpl), std::forward<F>(todo),
     make_index_list<std::tuple_size_v<std::remove_cvref_t<T>>>{});  // INVOKE HERE
  }
}

// TEMPLATE FUNCTION make_from_tuple

template <typename T, tuple_alike_with_magic Tuple>
[[nodiscard]] constexpr T make_from_tuple(Tuple&& tpl) {
  if constexpr (magic_tuple_alike<Tuple>) {
    return ::mtt::make_from_tuple(magic_filter(std::forward<Tuple>(tpl)));
  } else {
    return apply(
        []<typename... Args>(Args && ... args) { return T(std::forward<Args>(args)...); },
        std::forward<Tuple>(tpl));
  }
}

// TEMPLATE FUNCTION tuple_cat

// clang-format off
template <tuple_alike... Tuples>
[[nodiscard]] constexpr auto tuple_cat(Tuples&&... tuples)
noexcept(std::is_nothrow_constructible_v<tuple<Tuples...>, Tuples&&...>)
{
  using indexes_in_tuples = merge_value_lists_t<make_index_list<std::tuple_size_v<std::remove_cvref_t<Tuples>>>...>;
  using repeated_tuple_indexes = decltype(
      []<size_t... TupleIndexes>(std::index_sequence<TupleIndexes...>)
  {
    return merge_value_lists_t<make_index_list<std::tuple_size_v<std::remove_cvref_t<Tuples>>, TupleIndexes, 0>...>{};
  }(std::make_index_sequence<sizeof...(Tuples)>{}));  // PSEUDO INVOKE HERE

  using result_tuple = insert_type_list_t<tuple, merge_type_lists_t<extract_type_list_from_tuplealike<Tuples>...>>;
  return []<size_t... TupleIndexes, size_t... InTupleIndexes, typename Megatuple>
      (Megatuple&& megatuple, index_list<TupleIndexes...>, index_list<InTupleIndexes...>)
  {
    return result_tuple(std::get<InTupleIndexes>(std::get<TupleIndexes>(std::forward<Megatuple>(megatuple)))...);
  }
  (tuple<Tuples&&...>(std::forward<Tuples>(tuples)...), repeated_tuple_indexes{}, indexes_in_tuples{});  // INVOKE HERE
}

template <tuple_alike_with_magic... Tuples>
requires(magic_tuple_alike<Tuples> || ...)
[[nodiscard]] constexpr auto tuple_cat(Tuples&&... tuples) {
  return ::mtt::tuple_cat(magic_filter(std::forward<Tuples>(tuples))...);
}
// clang-format on
}  // namespace mtt

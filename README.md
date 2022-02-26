# MoreThanTuple
C++20 library for binary serialization of arbitrary aggregates or tuples. The library also unifies work with tuples and aggregates
* [`How to build?`](#build)

## like std:: tuple interface, but for aggregates too
* ['tuple'](#tuple)
* ['tuple_size_v'](#tuple_size_v)
* ['tuple_element_t'](#tuple_element_t)
* ['tuple_cat'](#tuple_cat)
* ['make_from_tuple'](#make_from_tuple)
* ['apply'](#apply)
## additional
* ['visit'](#visit)
* ['visit_recursive'](#visit_recursive)
## for aggregates only
* ['magic_tie'](#magic_tie)
* ['to_tuple'](#to_tuple)
## binary serialization / deserialization
* ['serialize'](#serialize)
* ['deserialize'](#deserialize)

## tuple
similar to std::tuple, but values in memory stored same order as types in tuple template arguments, uses
EBO optimization even in MSVC(yes EBO do not work on msvc for std::tuple...) and other small differences 
### tuple_size_v
### tuple_element_t
### tuple_cat
### make_from_tuple
### apply

similar to std:: analogues, but works for aggregates too
```C++

struct YourType {
int x;
double y;
// magic for your type enabled!
// Now you can use any tuple algorithms with this type
using enable_magic = void;
};
// or another way
template<>
enable_magic_for<MyType> : std::true_type {};
```

## visit
applies function to all fields of aggregate or members of tuple

Simple example:
```C++
struct A {
  int i = 10;
  double y = 20;
  const char* c = "hello";

  using enable_magic = void;
};

void SimpleExample() {
  A value;
  mtt::visit(value, [](auto& x) { std::cout << typeid(x).name() << '\n'; });
}
possible output:
int
double
char const *
```


## visit_recursive
Same as visit, but applies the function recursively to all field fields

example:
```C++
struct A {
  int i = 10;
  double y = 20;
  const char* c = "hello";

  using enable_magic = void;
};
struct B {
  A value;
  bool i = 10;
  float y = 20;
  const char* c = "hello";
  using enable_magic = void;
};

void BB() {
  static_assert(mtt::tuple_size_v<B> == 4);
  B value;
  mtt::visit_recursive(value, [](auto& x) { std::cout << typeid(x).name() << '\n'; });
}
possible output:
int
double
char const *
bool
float
char const *
```

## magic_tie

returns tuple of references to accepted aggregate fields

## to_tuple

converts aggregate into std::tuple

## serialize
There are three modes for serialization -
* network - default, strict requirements for the same size of types on different machines
* network_with_int_aliases(when
you guarantee that you use int32_t / int64_t etc and not just int/long for same sizeof on other machines)
* single_machine


interface:
```C++
template <mode Mode = mode::network, typename OutIter, serializable<Mode> T>
requires(*OutIter check, must be for std::byte/char/unsigned char*)
void serialize_to(OutIter out_iter, T&& value);

template <mode Mode = mode::network, serializable<Mode> T>
std::string serialize(T&& value)

```
example:
```C++
// some bad type
  using check_type = std::tuple<std::vector<int>, double, float, std::pair<int, char>,
                                mtt::tuple<int, double, std::set<double>>>;
//... fill this mega type
  check_type value(/*something*/);
  // its all! Ready!
  const std::string buffer = serialize<mode::single_machine>(value);
  // and deserialize
  auto result = deserialize<check_type, mode::single_machine>(buffer);
```
## deserialize
interface:
```C++
template <typename T, mode Mode = mode::network, std::ranges::range Range>
requires(serializable<T, Mode> && one_of<std::ranges::range_value_t<Range>, std::byte, char, unsigned char>)
constexpr T deserialize(Range&& range)
```

## `build`
```
git clone https://github.com/kelbon/kelcoro
cd MoreThanTuple
cmake . -B build OR cmake . -DENABLE_TESTS=ON -B build # with tests and main.cpp to try something
cmake --build build
```
_Note: cmake now may not support C++ modules(and clang...)_ 

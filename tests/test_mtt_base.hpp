
#ifndef TEST_MTT_BASE_HPP
#define TEST_MTT_BASE_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

import mtt.traits;

#define CONCAT(a, b) a##b
#define TEST(test_name)                                                                                 \
  inline void CONCAT(test_name, Test)();                                                                \
  const inline ::mtt::test::detail::test_adder CONCAT(adder_for_, test_name) = CONCAT(test_name, Test); \
  inline void CONCAT(test_name, Test)()

#define verify(expr)                                                                             \
  do {                                                                                           \
    if (!(expr)) [[unlikely]] {                                                                  \
      throw ::mtt::test::test_failed(                                                            \
          ::mtt::test::detail::concat("Failed ", __FUNCTION__, " on line #", __LINE__).c_str()); \
    }                                                                                            \
  } while (false)

namespace mtt::test {

class test_failed : public std::exception {
 public:
  test_failed(const char* const message) noexcept : std::exception(message) {
  }
};

class test_room {
 private:
  std::vector<std::function<void()>> tests;
  test_room() = default;

 public:
  static test_room& get_tester() noexcept {
    static test_room tester;
    return tester;
  }

  template <std::invocable T>
  void add_test(T&& task) {
    tests.emplace_back(std::forward<T>(task));
  }

  static void run_all_tests() {
    for (auto& test : get_tester().tests) {
      try {
        if (test)
          test();
      } catch (test_failed& error) {
        std::cerr << error.what() << std::endl;
      } catch (...) {
        std::cerr << "Unexpected exception, something bad here" << std::endl;
        throw;
      }
    }
  }
};

namespace detail {
// for test adding on static initialization
template <std::invocable F>
struct test_adder {
  test_adder(F&& func) {
    test_room::get_tester().add_test(std::forward<F>(func));
  }
};

std::string concat(auto... values) {
  std::stringstream result;
  ((result << values), ...);
  return result.str().c_str();
}

}  // namespace detail

}  // namespace mtt::test

#endif  // !TEST_MTT_BASE_HPP

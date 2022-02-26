
#include "test_mtt_serialization.hpp"
#include "test_mtt_traits.hpp"
#include "test_mtt_tuple.hpp"

//import<chrono>;
#include <chrono>

int main() {
  using namespace std::chrono;
  const auto start = steady_clock::now();
  mtt::test::test_room::run_all_tests();
  std::cout << duration_cast<milliseconds>(steady_clock::now() - start);
}
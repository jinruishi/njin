#include <catch2/catch_test_macros.hpp>
#include "math/njVec3.h"

using namespace njin::math;
TEST_CASE("njVec3", "[njVec3]") {
  SECTION("construcs without throwing") {
    REQUIRE_NOTHROW(njVec3(0.0f, 0.0f, 0.0f));
  }
}
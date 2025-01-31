#include "math/njMat4.h"

#include <numbers>

#include <catch2/catch_test_macros.hpp>

#include <cmath>

using namespace njin::math;

TEST_CASE("njMat4", "[njMat4]") {
    SECTION("construction with njVec4<float> does not throw") {
        njVec4 vec_1{ 1.0f, 2.0f, 3.0f, 4.0f };
        njVec4<float> vec_2{ 1.0f, 2.0f, 3.0f, 4.0f };
        njVec4<float> vec_3{ 1.0f, 2.0f, 3.0f, 4.0f };
        njVec4<float> vec_4{ 1.0f, 2.0f, 3.0f, 4.0f };

        REQUIRE_NOTHROW(njMat4<float>{ vec_1, vec_2, vec_3, vec_4 });
    }

    SECTION("default construction") {
        njMat4<float> float_mat{};
        njMat4<float> float_expected{ { 1.f, 0.f, 0.f, 0.f },
                                      { 0.f, 1.f, 0.f, 0.f },
                                      { 0.f, 0.f, 1.f, 0.f },
                                      { 0.f, 0.f, 0.f, 1.f } };

        REQUIRE(float_mat == float_expected);

        njMat4<int> int_mat{};
        njMat4<int> int_expected{ { 1, 0, 0, 0 },
                                  { 0, 1, 0, 0 },
                                  { 0, 0, 1, 0 },
                                  { 0, 0, 0, 1 } };

        REQUIRE(int_mat == int_expected);
    }

    SECTION("addition") {
        njMat4<float> mat{ { 1.0f, 1.0f, 1.0f, 1.0f },
                           { 1.0f, 1.0f, 1.0f, 1.0f },
                           { 1.0f, 1.0f, 1.0f, 1.0f },
                           { 1.0f, 1.0f, 1.0f, 1.0f } };

        njMat4 result{ mat + mat };

        njMat4<float> expected{ { 2.0f, 2.0f, 2.0f, 2.0f },
                                { 2.0f, 2.0f, 2.0f, 2.0f },
                                { 2.0f, 2.0f, 2.0f, 2.0f },
                                { 2.0f, 2.0f, 2.0f, 2.0f } };

        REQUIRE(result == expected);
    }

    SECTION("subtraction") {
        njMat4<float> mat{ { 1.0f, 1.0f, 1.0f, 1.0f },
                           { 1.0f, 1.0f, 1.0f, 1.0f },
                           { 1.0f, 1.0f, 1.0f, 1.0f },
                           { 1.0f, 1.0f, 1.0f, 1.0f } };

        njMat4<float> expected{ { 0.0f, 0.0f, 0.0f, 0.0f },
                                { 0.0f, 0.0f, 0.0f, 0.0f },
                                { 0.0f, 0.0f, 0.0f, 0.0f },
                                { 0.0f, 0.0f, 0.0f, 0.0f } };

        njMat4 result{ mat - mat };

        REQUIRE(result == expected);
    }

    SECTION("multiplication") {
        njMat4<float> mat{ { 1.f, 2.f, 3.f, 4.f },
                           { 1.f, 2.f, 3.f, 4.f },
                           { 1.f, 2.f, 3.f, 4.f },
                           { 1.f, 2.f, 3.f, 4.f } };

        njMat4<float> expected{ { 10.f, 20.f, 30.f, 40.f },
                                { 10.f, 20.f, 30.f, 40.f },
                                { 10.f, 20.f, 30.f, 40.f },
                                { 10.f, 20.f, 30.f, 40.f } };

        njMat4 result{ mat * mat };
        REQUIRE(result == expected);
    }

    SECTION("row retrieval") {
        njMat4<float> mat{ { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f },
                           { 13.f, 14.f, 15.f, 16.f } };

        njVec4 expected{ 5.f, 6.f, 7.f, 8.f };
        njVec4 row{ mat.row(1) };

        REQUIRE(row == expected);
    }

    SECTION("column retrieval") {
        njMat4<float> mat{ { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f },
                           { 13.f, 14.f, 15.f, 16.f } };

        njVec4 expected{ 2.f, 6.f, 10.f, 14.f };
        njVec4 col{ mat.col(1) };

        REQUIRE(col == expected);
    }

    SECTION("float matrix size is 16-byte aligned") {
        njMat4<float> mat{};

        size_t size{ sizeof(mat) };

        REQUIRE(size == 64);
    }

    SECTION("construction from translation vector") {
        njVec3f t{ 1.f, 2.f, 3.f };

        njMat4<float> mat{ njMat4Type::Translation, t };

        njMat4<float> expected{ { 1.f, 0.f, 0.f, 1.f },
                                { 0.f, 1.f, 0.f, 2.f },
                                { 0.f, 0.f, 1.f, 3.f },
                                { 0.f, 0.f, 0.f, 1.f } };

        REQUIRE(mat == expected);
    }

    SECTION("construction from unit quaternion") {
        // rotation about the x-axis by 45 degrees
        float phi{ ((45.f / 2.f) * std::numbers::pi_v<float>) / 180.f };
        float sin{ std::sin(phi) };
        float cos{ std::cos(phi) };
        njVec4 r{ 1.f * sin, 0.f * sin, 0.f * sin, cos };

        njMat4<float> mat{ r };

        njMat4<float> expected{
            { 1.f, 0.f, 0.f, 0.f },
            { 0.f, 1 - 2 * (std::pow(sin, 2.f)), -2.f * cos * sin, 0.f },
            { 0.f, 2 * cos * sin, 1 - 2 * std::pow(sin, 2.f), 0.f },
            { 0.f, 0.f, 0.f, 1.f }
        };

        REQUIRE(mat == expected);
    }

    SECTION("construction from scale vector") {
        njVec3f s{ 1.f, 2.f, 3.f };

        njMat4<float> mat{ njMat4Type::Scale, s };
        njMat4<float> expected{ { 1.f, 0.f, 0.f, 0.f },
                                { 0.f, 2.f, 0.f, 0.f },
                                { 0.f, 0.f, 3.f, 0.f },
                                { 0.f, 0.f, 0.f, 1.f } };

        REQUIRE(mat == expected);
    }
}

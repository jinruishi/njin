#include "njVec3.h"

#include <cmath>

namespace njin::math {
    template<typename T>
    njMat4<T>::njMat4() :
        njMat4{ { 1, 0, 0, 0 },
                { 0, 1, 0, 0 },
                { 0, 0, 1, 0 },
                { 0, 0, 0, 1 } } {}

    template<typename T>
    njMat4<T>::njMat4(const njVec4<T>& row_1,
                      const njVec4<T>& row_2,
                      const njVec4<T>& row_3,
                      const njVec4<T>& row_4) {
        matrix_[0][0] = row_1.x;
        matrix_[0][1] = row_1.y;
        matrix_[0][2] = row_1.z;
        matrix_[0][3] = row_1.w;

        matrix_[1][0] = row_2.x;
        matrix_[1][1] = row_2.y;
        matrix_[1][2] = row_2.z;
        matrix_[1][3] = row_2.w;

        matrix_[2][0] = row_3.x;
        matrix_[2][1] = row_3.y;
        matrix_[2][2] = row_3.z;
        matrix_[2][3] = row_3.w;

        matrix_[3][0] = row_4.x;
        matrix_[3][1] = row_4.y;
        matrix_[3][2] = row_4.z;
        matrix_[3][3] = row_4.w;
    }

    template<typename T>
    njMat4<T>::njMat4(njMat4Type type, const njVec3f& data) {
        Row row_1{};
        Row row_2{};
        Row row_3{};
        Row row_4{};
        if (type == njMat4Type::Translation) {
            row_1 = { 1.f, 0.f, 0.f, data.x };
            row_2 = { 0.f, 1.f, 0.f, data.y };
            row_3 = { 0.f, 0.f, 1.f, data.z };
            row_4 = { 0.f, 0.f, 0.f, 1.f };
        } else if (type == njMat4Type::Scale) {
            row_1 = { data.x, 0.f, 0.f, 0.f };
            row_2 = { 0.f, data.y, 0.f, 0.f };
            row_3 = { 0.f, 0.f, data.z, 0.f };
            row_4 = { 0.f, 0.f, 0.f, 1.f };
        }

        matrix_[0] = row_1;
        matrix_[1] = row_2;
        matrix_[2] = row_3;
        matrix_[3] = row_4;
    }

    template<typename T>
    njMat4<T>::njMat4(const njVec4f& quaternion) {
        // njVec4 is a unit quaternion
        float qx{ quaternion.x };
        float qy{ quaternion.y };
        float qz{ quaternion.z };
        float qw{ quaternion.w };

        Row row_0{ 1 - 2 * (std::pow(qy, 2.f) + std::pow(qz, 2.f)),
                   2 * (qx * qy - qw * qz),
                   2 * (qx * qz + qw * qy),
                   0 };

        Row row_1{ 2 * (qx * qy + qw * qz),
                   1 - 2 * (std::pow(qx, 2.f) + std::pow(qz, 2.f)),
                   2 * (qy * qz - qw * qx),
                   0 };

        Row row_2{ 2 * (qx * qz - qw * qy),
                   2 * (qy * qz + qw * qx),
                   1 - 2 * (std::pow(qx, 2.f) + std::pow(qy, 2.f)),
                   0 };

        Row row_3{ 0, 0, 0, 1 };

        matrix_[0] = row_0;
        matrix_[1] = row_1;
        matrix_[2] = row_2;
        matrix_[3] = row_3;
    }

    template<typename T>
    njMat4<T>& njMat4<T>::operator+(const njMat4& other) {
        for (int i{ 0 }; i < 4; ++i) {
            for (int j{ 0 }; j < 4; ++j) {
                this->matrix_[i][j] += other.matrix_[i][j];
            }
        }
        return *this;
    };

    template<typename T>
    bool njMat4<T>::operator==(const njMat4& other) const {
        for (int i{ 0 }; i < 4; ++i) {
            for (int j{ 0 }; j < 4; ++j) {
                if (std::fabs(this->matrix_[i][j] - other.matrix_[i][j]) >
                    1e-6) {
                    return false;
                }
            }
        }

        return true;
    }

    template<typename T>
    njMat4<T>& njMat4<T>::operator-(const njMat4& other) {
        for (int i{ 0 }; i < 4; ++i) {
            for (int j{ 0 }; j < 4; ++j) {
                this->matrix_[i][j] -= other.matrix_[i][j];
            }
        }
        return *this;
    }

    template<typename T>
    njMat4<T>& njMat4<T>::operator*(const njMat4& other) {
        njMat4<T> result{};
        for (int i{ 0 }; i < 4; ++i) {
            for (int j{ 0 }; j < 4; ++j) {
                njVec4<T> row{ this->row(i) };
                njVec4<T> col{ other.col(j) };

                result.matrix_[i][j] = dot(row, col);
            }
        }
        matrix_ = result.matrix_;

        return *this;
    }

    template<typename T>
    njVec4<T> njMat4<T>::row(int row_index) const {
        return matrix_[row_index];
    }

    template<typename T>
    njVec4<T> njMat4<T>::col(int col_index) const {
        njVec4<T> result{};
        result.x = matrix_[0][col_index];
        result.y = matrix_[1][col_index];
        result.z = matrix_[2][col_index];
        result.w = matrix_[3][col_index];

        return result;
    }

    template<typename T>
    njMat4<T> njMat4<T>::Identity() {
        return { { 1, 0, 0, 0 },
                 { 0, 1, 0, 0 },
                 { 0, 0, 1, 0 },
                 { 0, 0, 0, 1 } };
    }

}  // namespace njin::math

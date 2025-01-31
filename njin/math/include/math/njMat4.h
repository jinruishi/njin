#pragma once
#include <array>

#include "math.h"
#include "math/njVec3.h"
#include "math/njVec4.h"

namespace njin::math {
    enum class njMat4Type {
        Translation,
        Scale
    };

    template<typename T>
    class njMat4 {
        public:
        njMat4();

        static njMat4<T> Identity();

        /**
         * From four row vectors
         * @param row_0 Zeroth row vector
         * @param row_1 First row vector
         * @param row_2 Second row vector
         * @param row_3 Third row vector
         */
        njMat4(const njVec4<T>& row_0,
               const njVec4<T>& row_1,
               const njVec4<T>& row_2,
               const njVec4<T>& row_3);

        /**
         * From either a translation or scale vector
         * @param type Type of vector
         * @param data Vector data
         * @note This vector must be a 3D vector in the local coordinate system
         * @note Construction of these two kinds of matrices are grouped
         * together because they are both njVec3
         */
        explicit njMat4(njMat4Type type, const njVec3f& data);

        /**
         * From a unit quaternion
         * @param quaternion Unit quaternion
         * @note The imaginary part must be relative to the local coordinate system
         */
        explicit njMat4(const njVec4f& quaternion);

        njMat4<T>& operator+(const njMat4& other);

        njMat4<T>& operator-(const njMat4& other);

        njMat4<T>& operator*(const njMat4& other);

        /**
         * Retrieve a row of this njMat4
         * @param row_index 0-indexed row of matrix
         * @return Row of matrix
         */
        njVec4<T> row(int row_index) const;

        /**
         * Retrieve a column of this njMat4
         * @param col_index 0-indexed column of matrix
         * @return Column of matrix
         */
        njVec4<T> col(int col_index) const;

        bool operator==(const njMat4& other) const;

        private:
        using Row = std::array<T, 4>;
        using Matrix = std::array<Row, 4>;

        Matrix matrix_{};
    };

    using njMat4f = njMat4<float>;
}  // namespace njin::math

#include "math/njMat4.tpp"

#include <gtest/gtest.h>

#include <stdexcept>

#include "securebuild/tensor_ops.h"

using securebuild::Tensor;

TEST(TensorOps, ElementwiseAdd) {
  const Tensor lhs{{2, 2}, {1.0, 2.0, 3.0, 4.0}};
  const Tensor rhs{{2, 2}, {5.0, 6.0, 7.0, 8.0}};

  const Tensor result = securebuild::elementwise_add(lhs, rhs);

  EXPECT_EQ(result.shape, (securebuild::Shape{2, 2}));
  EXPECT_EQ(result.values, (securebuild::TensorData{6.0, 8.0, 10.0, 12.0}));
}

TEST(TensorOps, ElementwiseMultiply) {
  const Tensor lhs{{2, 2}, {1.0, 2.0, 3.0, 4.0}};
  const Tensor rhs{{2, 2}, {5.0, 6.0, 7.0, 8.0}};

  const Tensor result = securebuild::elementwise_multiply(lhs, rhs);

  EXPECT_EQ(result.values, (securebuild::TensorData{5.0, 12.0, 21.0, 32.0}));
}

TEST(TensorOps, DotProduct) {
  const Tensor lhs{{4}, {1.0, 2.0, 3.0, 4.0}};
  const Tensor rhs{{4}, {5.0, 6.0, 7.0, 8.0}};

  EXPECT_DOUBLE_EQ(securebuild::dot_product(lhs, rhs), 70.0);
}

TEST(TensorOps, Transpose) {
  const Tensor matrix{{2, 3}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}};

  const Tensor result = securebuild::transpose(matrix);

  EXPECT_EQ(result.shape, (securebuild::Shape{3, 2}));
  EXPECT_EQ(result.values, (securebuild::TensorData{1.0, 4.0, 2.0, 5.0, 3.0, 6.0}));
}

TEST(TensorOps, MatrixMultiply) {
  const Tensor lhs{{2, 3}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}};
  const Tensor rhs{{3, 2}, {7.0, 8.0, 9.0, 10.0, 11.0, 12.0}};

  const Tensor result = securebuild::matrix_multiply(lhs, rhs);

  EXPECT_EQ(result.shape, (securebuild::Shape{2, 2}));
  EXPECT_EQ(result.values, (securebuild::TensorData{58.0, 64.0, 139.0, 154.0}));
}

TEST(TensorOps, RejectsShapeMismatch) {
  const Tensor lhs{{2, 2}, {1.0, 2.0, 3.0, 4.0}};
  const Tensor rhs{{2, 3}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}};

  EXPECT_THROW((securebuild::elementwise_add(lhs, rhs)), std::invalid_argument);
}


#include "securebuild/tensor_ops.h"

#include <sstream>
#include <stdexcept>
#include <utility>

namespace securebuild {
namespace {

std::invalid_argument shape_error(const std::string& message) {
  return std::invalid_argument("SecureBuild tensor error: " + message);
}

void ensure_same_shape(const Tensor& lhs, const Tensor& rhs, const char* operation) {
  if (lhs.shape != rhs.shape) {
    std::ostringstream oss;
    oss << operation << " requires matching shapes, got "
        << shape_to_string(lhs.shape) << " and " << shape_to_string(rhs.shape);
    throw shape_error(oss.str());
  }
}

void ensure_rank(const Tensor& tensor, std::size_t expected_rank, const char* operation) {
  if (tensor.rank() != expected_rank) {
    std::ostringstream oss;
    oss << operation << " requires rank " << expected_rank << ", got " << tensor.rank()
        << " for shape " << shape_to_string(tensor.shape);
    throw shape_error(oss.str());
  }
}

}  // namespace

Tensor::Tensor(Shape shape_in, TensorData values_in)
    : shape(std::move(shape_in)), values(std::move(values_in)) {
  validate_tensor(*this);
}

Tensor::Tensor(std::initializer_list<std::size_t> shape_in,
               std::initializer_list<double> values_in)
    : shape(shape_in), values(values_in) {
  validate_tensor(*this);
}

std::size_t Tensor::element_count() const noexcept {
  std::size_t total = 1;
  for (const std::size_t dimension : shape) {
    total *= dimension;
  }
  return shape.empty() ? 1 : total;
}

std::size_t Tensor::rank() const noexcept {
  return shape.size();
}

void validate_tensor(const Tensor& tensor) {
  if (tensor.shape.empty()) {
    if (tensor.values.size() != 1U) {
      throw shape_error("scalars must contain exactly one value");
    }
    return;
  }

  const std::size_t count = tensor.element_count();
  if (count != tensor.values.size()) {
    std::ostringstream oss;
    oss << "shape " << shape_to_string(tensor.shape) << " requires " << count
        << " values, got " << tensor.values.size();
    throw shape_error(oss.str());
  }

  return;
}

Tensor elementwise_add(const Tensor& lhs, const Tensor& rhs) {
  ensure_same_shape(lhs, rhs, "elementwise_add");
  Tensor result(lhs.shape, TensorData(lhs.values.size()));
  for (std::size_t index = 0; index < lhs.values.size(); ++index) {
    result.values[index] = lhs.values[index] + rhs.values[index];
  }
  return result;
}

Tensor elementwise_multiply(const Tensor& lhs, const Tensor& rhs) {
  ensure_same_shape(lhs, rhs, "elementwise_multiply");
  Tensor result(lhs.shape, TensorData(lhs.values.size()));
  for (std::size_t index = 0; index < lhs.values.size(); ++index) {
    result.values[index] = lhs.values[index] * rhs.values[index];
  }
  return result;
}

double dot_product(const Tensor& lhs, const Tensor& rhs) {
  ensure_rank(lhs, 1U, "dot_product");
  ensure_rank(rhs, 1U, "dot_product");
  ensure_same_shape(lhs, rhs, "dot_product");

  double total = 0.0;
  for (std::size_t index = 0; index < lhs.values.size(); ++index) {
    total += lhs.values[index] * rhs.values[index];
  }
  return total;
}

Tensor transpose(const Tensor& tensor) {
  ensure_rank(tensor, 2U, "transpose");

  const std::size_t rows = tensor.shape[0];
  const std::size_t cols = tensor.shape[1];
  Tensor result({cols, rows}, TensorData(rows * cols));
  for (std::size_t row = 0; row < rows; ++row) {
    for (std::size_t col = 0; col < cols; ++col) {
      result.values[col * rows + row] = tensor.values[row * cols + col];
    }
  }
  return result;
}

Tensor matrix_multiply(const Tensor& lhs, const Tensor& rhs) {
  ensure_rank(lhs, 2U, "matrix_multiply");
  ensure_rank(rhs, 2U, "matrix_multiply");

  const std::size_t lhs_rows = lhs.shape[0];
  const std::size_t lhs_cols = lhs.shape[1];
  const std::size_t rhs_rows = rhs.shape[0];
  const std::size_t rhs_cols = rhs.shape[1];

  if (lhs_cols != rhs_rows) {
    std::ostringstream oss;
    oss << "matrix_multiply requires lhs columns to match rhs rows, got "
        << lhs_cols << " and " << rhs_rows;
    throw shape_error(oss.str());
  }

  Tensor result({lhs_rows, rhs_cols}, TensorData(lhs_rows * rhs_cols, 0.0));
  for (std::size_t row = 0; row < lhs_rows; ++row) {
    for (std::size_t col = 0; col < rhs_cols; ++col) {
      double cell = 0.0;
      for (std::size_t inner = 0; inner < lhs_cols; ++inner) {
        cell += lhs.values[row * lhs_cols + inner] * rhs.values[inner * rhs_cols + col];
      }
      result.values[row * rhs_cols + col] = cell;
    }
  }
  return result;
}

std::string shape_to_string(const Shape& shape) {
  std::ostringstream oss;
  oss << '[';
  for (std::size_t index = 0; index < shape.size(); ++index) {
    oss << shape[index];
    if (index + 1U < shape.size()) {
      oss << ", ";
    }
  }
  oss << ']';
  return oss.str();
}

}  // namespace securebuild

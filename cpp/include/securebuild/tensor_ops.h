#pragma once

#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

namespace securebuild {

using Shape = std::vector<std::size_t>;
using TensorData = std::vector<double>;

struct Tensor {
  Shape shape;
  TensorData values;

  Tensor() = default;
  Tensor(Shape shape, TensorData values);
  Tensor(std::initializer_list<std::size_t> shape, std::initializer_list<double> values);

  std::size_t element_count() const noexcept;
  std::size_t rank() const noexcept;
};

void validate_tensor(const Tensor& tensor);
Tensor elementwise_add(const Tensor& lhs, const Tensor& rhs);
Tensor elementwise_multiply(const Tensor& lhs, const Tensor& rhs);
double dot_product(const Tensor& lhs, const Tensor& rhs);
Tensor transpose(const Tensor& tensor);
Tensor matrix_multiply(const Tensor& lhs, const Tensor& rhs);

std::string shape_to_string(const Shape& shape);

}  // namespace securebuild

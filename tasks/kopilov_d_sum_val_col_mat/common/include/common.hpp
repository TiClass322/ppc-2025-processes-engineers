#pragma once

#include <string>
#include <vector>
#include <tuple>

#include "task/include/task.hpp"

namespace kopilov_d_sum_val_col_mat {

struct Matrix {
  std::vector<int> data;
  int rows;
  int cols;
};

using InType = Matrix;
using OutType = std::vector<int>;
using TestType = std::tuple<int, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kopilov_d_sum_val_col_mat

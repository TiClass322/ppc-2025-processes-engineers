#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace kopilov_d_sum_val_col_mat {

using InType = int;
using OutType = int;
using TestType = std::tuple<int, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kopilov_d_sum_val_col_mat

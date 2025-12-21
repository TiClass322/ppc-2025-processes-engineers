#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "task/include/task.hpp"

namespace kopilov_d_ring_2 {

struct Input {
  int value;
};

struct Output {
  int value;
};

using InType = Input;
using OutType = Output;

using TestType = std::tuple<int>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kopilov_d_ring_2

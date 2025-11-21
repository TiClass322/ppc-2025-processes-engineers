#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"

#include <stdexcept>
#include <vector>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"

namespace kopilov_d_sum_val_col_mat {

KopilovDSumValColMatSEQ::KopilovDSumValColMatSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;

  GetOutput() = OutType{};
}

bool KopilovDSumValColMatSEQ::ValidationImpl() {
  const auto &in = GetInput();

  if (in.rows <= 0 || in.cols <= 0) {
    return false;
  }

  if ((int)in.data.size() != in.rows * in.cols) {
    return false;
  }

  return true;
}

bool KopilovDSumValColMatSEQ::PreProcessingImpl() {
  auto &out = GetOutput();
  out.col_sum.assign(GetInput().cols, 0.0);
  return true;
}

bool KopilovDSumValColMatSEQ::RunImpl() {
  const auto &in = GetInput();
  auto &out = GetOutput().col_sum;

  const int rows = in.rows;
  const int cols = in.cols;

  const auto &mat = in.data;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      out[c] += mat[r * cols + c];
    }
  }

  return true;
}

bool KopilovDSumValColMatSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kopilov_d_sum_val_col_mat

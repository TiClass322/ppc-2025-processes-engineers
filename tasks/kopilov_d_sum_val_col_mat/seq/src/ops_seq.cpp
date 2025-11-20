#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"

#include <vector>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"

namespace kopilov_d_sum_val_col_mat {

KopilovDSumValColMatSEQ::KopilovDSumValColMatSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput().resize(GetInput().cols, 0);
}

bool KopilovDSumValColMatSEQ::ValidationImpl() {
  return GetInput().rows > 0 && GetInput().cols > 0 &&
         GetInput().data.size() == (size_t)GetInput().rows * GetInput().cols &&
         GetOutput().size() == (size_t)GetInput().cols;
}

bool KopilovDSumValColMatSEQ::PreProcessingImpl() {
  for (size_t i = 0; i < GetOutput().size(); ++i) {
    GetOutput()[i] = 0;
  }
  return true;
}

bool KopilovDSumValColMatSEQ::RunImpl() {
  for (int j = 0; j < GetInput().cols; ++j) {
    for (int i = 0; i < GetInput().rows; ++i) {
      GetOutput()[j] += GetInput().data[i * GetInput().cols + j];
    }
  }
  return true;
}

bool KopilovDSumValColMatSEQ::PostProcessingImpl() { return true; }

}  // namespace kopilov_d_sum_val_col_mat

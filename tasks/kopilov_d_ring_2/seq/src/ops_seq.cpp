#include "kopilov_d_ring_2/seq/include/ops_seq.hpp"

#include <vector>

#include "kopilov_d_ring_2/common/include/common.hpp"

namespace kopilov_d_ring_2 {

KopilovDRingSEQ::KopilovDRingSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = OutType{0};
}

bool KopilovDRingSEQ::ValidationImpl() {
  return true;
}

bool KopilovDRingSEQ::PreProcessingImpl() {
  GetOutput().value = GetInput().value;
  return true;
}

bool KopilovDRingSEQ::RunImpl() {
  int val = GetInput().value;
  for (int i = 0; i < 10000; ++i) {
    val += 0;
  }
  GetOutput().value = val;
  return true;
}

bool KopilovDRingSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kopilov_d_ring_2

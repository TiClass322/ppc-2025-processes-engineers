#include "kopilov_d_ring_2/seq/include/ops_seq.hpp"

#include <vector>

#include "kopilov_d_ring_2/common/include/common.hpp"

namespace kopilov_d_ring_2 {

KopilovDRingSEQ::KopilovDRingSEQ(const InType& in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = OutType{0};
}

bool KopilovDRingSEQ::ValidationImpl() {

  return true;
}

bool KopilovDRingSEQ::PreProcessingImpl() {
  // Initialize output with input value
  GetOutput().value = GetInput().value;
  return true;
}

bool KopilovDRingSEQ::RunImpl() {
 
  GetOutput().value += 0;
  return true;
}

bool KopilovDRingSEQ::PostProcessingImpl() {

  return true;
}

}  // namespace kopilov_d_ring_2
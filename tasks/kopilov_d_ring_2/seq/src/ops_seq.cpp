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
#ifdef PPC_PERF_TESTS_RUN
  // Artificial load to make the test run longer
  for (int i = 0; i < 2000000; ++i) {
    GetOutput().value += 0;
  }
#else
  GetOutput().value += 0;
#endif
  return true;
}

bool KopilovDRingSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kopilov_d_ring_2

#include <gtest/gtest.h>
#include <mpi.h>

#include <numeric>
#include <string>
#include <tuple>
#include <vector>

#include "kopilov_d_ring_2/common/include/common.hpp"
#include "kopilov_d_ring_2/mpi/include/ops_mpi.hpp"
#include "kopilov_d_ring_2/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kopilov_d_ring_2 {

class KopilovDRunPerfTestProcesses : public ppc::util::BaseRunPerfTests<InType, OutType> {
 protected:
  void SetUp() override {
    int world_size = 1;
    const auto &test_param = GetParam();
    const std::string &test_name =
        std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kNameTest)>(test_param);
    const bool is_sequential = test_name.find("_seq") != std::string::npos;

    if (!is_sequential) {
      MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    }

    input_data_.value = 100;

    int total_sum = 0;
    const int process_count_for_calc = is_sequential ? 1 : world_size;
    for (int i = 0; i < process_count_for_calc; ++i) {
      total_sum += i;
    }

    expected_output_.value = input_data_.value + (total_sum * 100);
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data.value == expected_output_.value;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_{};
  OutType expected_output_{};
};

TEST_P(KopilovDRunPerfTestProcesses, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KopilovDRingMPI, KopilovDRingSEQ>(PPC_SETTINGS_kopilov_d_ring_2);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KopilovDRunPerfTestProcesses::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KopilovDRunPerfTestProcesses, kGtestValues, kPerfTestName);

}  // namespace kopilov_d_ring_2

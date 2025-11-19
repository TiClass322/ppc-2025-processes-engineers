#include <gtest/gtest.h>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"
#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"
#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kopilov_d_sum_val_col_mat {

class KopilovDSumValColMatPerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kCount_ = 100;
  InType input_data_{};

  void SetUp() override {
    input_data_ = kCount_;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return input_data_ == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(KopilovDSumValColMatPerfTests, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KopilovDSumValColMatMPI, KopilovDSumValColMatSEQ>(PPC_SETTINGS_kopilov_d_sum_val_col_mat);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KopilovDSumValColMatPerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KopilovDSumValColMatPerfTests, kGtestValues, kPerfTestName);

}  // namespace kopilov_d_sum_val_col_mat

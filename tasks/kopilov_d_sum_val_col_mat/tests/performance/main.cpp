#include <gtest/gtest.h>

#include <cmath>
#include <cstddef>
#include <vector>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"
#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"
#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kopilov_d_sum_val_col_mat {

class KopilovDSumValColMatPerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
 protected:
  const int kMatrixSize = 4000;
  InType inputData;
  OutType expectedOutput;

  void SetUp() override {
    inputData.rows = kMatrixSize;
    inputData.cols = kMatrixSize;
    inputData.data.resize(static_cast<std::size_t>(kMatrixSize) * static_cast<std::size_t>(kMatrixSize));

    for (int row = 0; row < kMatrixSize; ++row) {
      for (int col = 0; col < kMatrixSize; ++col) {
        inputData.data[static_cast<std::size_t>(row) * static_cast<std::size_t>(kMatrixSize) + static_cast<std::size_t>(col)] = static_cast<double>(row + col);
      }
    }

    expectedOutput.col_sum.resize(static_cast<std::size_t>(kMatrixSize));
    for (int col = 0; col < kMatrixSize; ++col) {
      double sum = 0.0;
      for (int row = 0; row < kMatrixSize; ++row) {
        sum += static_cast<double>(row + col);
      }
      expectedOutput.col_sum[static_cast<std::size_t>(col)] = sum;
    }
  }

  InType GetTestInputData() override { return inputData; }

  bool CheckTestOutputData(OutType &output_data) override {
    if (output_data.col_sum.size() != expectedOutput.col_sum.size()) return false;
    for (std::size_t i = 0; i < expectedOutput.col_sum.size(); ++i) {
      if (std::abs(output_data.col_sum[i] - expectedOutput.col_sum[i]) > 1e-9) return false;
    }
    return true;
  }
};

TEST_P(KopilovDSumValColMatPerfTests, RunPerfModes) { ExecuteTest(GetParam()); }

const auto kAllPerfTasks = ppc::util::MakeAllPerfTasks<InType, KopilovDSumValColMatMPI, KopilovDSumValColMatSEQ>(
    PPC_SETTINGS_kopilov_d_sum_val_col_mat);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);
const auto kPerfTestName = KopilovDSumValColMatPerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KopilovDSumValColMatPerfTests, kGtestValues, kPerfTestName);

}  // namespace kopilov_d_sum_val_col_mat
#include <gtest/gtest.h>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"
#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"
#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kopilov_d_sum_val_col_mat {

class KopilovDSumValColMatPerfTests : public ppc::util::BaseRunPerfTests<InType, OutType> {
 protected:
  int size_ = 1000;
  InType input_data_;
  OutType expected_;

  void SetUp() override {
    input_data_.rows = size_;
    input_data_.cols = size_;
    input_data_.data.resize(size_ * size_);

    for (int i = 0; i < size_; ++i) {
      for (int j = 0; j < size_; ++j) {
        input_data_.data[i * size_ + j] = static_cast<double>(i + j);
      }
    }

    expected_.col_sum.resize(size_);
    for (int j = 0; j < size_; ++j) {
      double sum = 0;
      for (int i = 0; i < size_; ++i) {
        sum += (i + j);
      }
      expected_.col_sum[j] = sum;
    }
  }

  InType GetTestInputData() override {
    return input_data_;
  }

  bool CheckTestOutputData(OutType &output_data) override {
    if (output_data.col_sum.size() != expected_.col_sum.size()) {
      return false;
    }
    for (size_t i = 0; i < expected_.col_sum.size(); ++i) {
      if (std::abs(output_data.col_sum[i] - expected_.col_sum[i]) > 1e-9) {
        return false;
      }
    }
    return true;
  }
};

TEST_P(KopilovDSumValColMatPerfTests, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks = ppc::util::MakeAllPerfTasks<InType, KopilovDSumValColMatMPI, KopilovDSumValColMatSEQ>(
    PPC_SETTINGS_kopilov_d_sum_val_col_mat);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KopilovDSumValColMatPerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KopilovDSumValColMatPerfTests, kGtestValues, kPerfTestName);

}  // namespace kopilov_d_sum_val_col_mat

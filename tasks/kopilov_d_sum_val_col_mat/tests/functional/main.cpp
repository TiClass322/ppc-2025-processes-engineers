#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>
#include <tuple>
#include <vector>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"
#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"
#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"

namespace kopilov_d_sum_val_col_mat {

class KopilovDSumValColMatTests : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &p) {
    return std::to_string(std::get<0>(p)) + "_" + std::get<1>(p);
  }

 protected:
  void SetUp() override {
    size_ = std::get<0>(std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam()));

    input_.rows = size_;
    input_.cols = size_;
    input_.data.resize(size_ * size_);

    for (int i = 0; i < size_; i++) {
      for (int j = 0; j < size_; j++) {
        input_.data[i * size_ + j] = i + j;
      }
    }

    expected_.col_sum.resize(size_);
    for (int j = 0; j < size_; j++) {
      double sum = 0;
      for (int i = 0; i < size_; i++) {
        sum += (i + j);
      }
      expected_.col_sum[j] = sum;
    }
  }

  InType GetTestInputData() override {
    return input_;
  }

  bool CheckTestOutputData(OutType &output_data) override {
    if (output_data.col_sum.size() != expected_.col_sum.size()) {
      return false;
    }

    for (size_t i = 0; i < expected_.col_sum.size(); i++) {
      if (std::abs(output_data.col_sum[i] - expected_.col_sum[i]) > 1e-9) {
        return false;
      }
    }
    return true;
  }

 private:
  int size_ = 0;
  InType input_;
  OutType expected_;
};

namespace {

TEST_P(KopilovDSumValColMatTests, ColumnSumTest) {
  ExecuteTest(GetParam());
}

// Набор размеров матриц для тестирования
const std::array<TestType, 5> kTestParam = {std::make_tuple(3, "3"), std::make_tuple(5, "5"), std::make_tuple(7, "7"),
                                            std::make_tuple(9, "9"), std::make_tuple(12, "6")};

const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<KopilovDSumValColMatMPI, InType>(kTestParam, PPC_SETTINGS_kopilov_d_sum_val_col_mat),
    ppc::util::AddFuncTask<KopilovDSumValColMatSEQ, InType>(kTestParam, PPC_SETTINGS_kopilov_d_sum_val_col_mat));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);
const auto kPerfTestName = KopilovDSumValColMatTests::PrintFuncTestName<KopilovDSumValColMatTests>;

INSTANTIATE_TEST_SUITE_P(ColumnSumTests, KopilovDSumValColMatTests, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace kopilov_d_sum_val_col_mat

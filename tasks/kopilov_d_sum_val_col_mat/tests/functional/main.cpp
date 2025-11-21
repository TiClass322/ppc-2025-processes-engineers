#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"
#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"
#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"

namespace kopilov_d_sum_val_col_mat {

class KopilovDSumValColMatTests : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &param) {
    return std::to_string(std::get<0>(param)) + "_" + std::get<1>(param);
  }

 protected:
  void SetUp() override {
    const TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    matrixSize = std::get<0>(params);

    input.rows = matrixSize;
    input.cols = matrixSize;
    input.data.resize(static_cast<std::size_t>(matrixSize) * static_cast<std::size_t>(matrixSize));

    for (int row = 0; row < matrixSize; ++row) {
      for (int col = 0; col < matrixSize; ++col) {
        input.data[static_cast<std::size_t>(row) * static_cast<std::size_t>(matrixSize) +
                   static_cast<std::size_t>(col)] = static_cast<double>(row + col);
      }
    }

    expected.col_sum.resize(static_cast<std::size_t>(matrixSize));
    for (int col = 0; col < matrixSize; ++col) {
      double sum = 0.0;
      for (int row = 0; row < matrixSize; ++row) {
        sum += static_cast<double>(row + col);
      }
      expected.col_sum[static_cast<std::size_t>(col)] = sum;
    }
  }

  InType GetTestInputData() override {
    return input;
  }

  bool CheckTestOutputData(OutType &output_data) override {
    if (output_data.col_sum.size() != expected.col_sum.size()) {
      return false;
    }
    for (std::size_t i = 0; i < expected.col_sum.size(); ++i) {
      if (std::abs(output_data.col_sum[i] - expected.col_sum[i]) > 1e-9) {
        return false;
      }
    }
    return true;
  }

 private:
  int matrixSize = 0;
  InType input;
  OutType expected;
};

namespace {

TEST_P(KopilovDSumValColMatTests, ColumnSumTest) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 3> kTestParam = {std::make_tuple(3, "3"), std::make_tuple(5, "5"), std::make_tuple(7, "7")};

const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<KopilovDSumValColMatMPI, InType>(kTestParam, PPC_SETTINGS_kopilov_d_sum_val_col_mat),
    ppc::util::AddFuncTask<KopilovDSumValColMatSEQ, InType>(kTestParam, PPC_SETTINGS_kopilov_d_sum_val_col_mat));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);
const auto kPerfTestName = KopilovDSumValColMatTests::PrintFuncTestName<KopilovDSumValColMatTests>;

INSTANTIATE_TEST_SUITE_P(ColumnSumTests, KopilovDSumValColMatTests, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace kopilov_d_sum_val_col_mat

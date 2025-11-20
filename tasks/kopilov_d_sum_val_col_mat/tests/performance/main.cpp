#include <gtest/gtest.h>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"
#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"
#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace kopilov_d_sum_val_col_mat {

class KopilovDSumValColMatPerfTests
    : public ppc::util::BaseRunPerfTests<InType, OutType> {
 public:
  // Размер большой матрицы для perf теста
  const int N = 1000;  // 1000 x 1000 → миллион элементов

  InType matrix_;
  OutType expected_;

  void SetUp() override {
    matrix_.rows = N;
    matrix_.cols = N;
    matrix_.data.resize(N * N);

    // Заполняем последовательными числами
    std::iota(matrix_.data.begin(), matrix_.data.end(), 1);

    // Вычисляем эталонное значение (SEQ)
    expected_.resize(N, 0);

    for (int j = 0; j < N; ++j)
      for (int i = 0; i < N; ++i)
        expected_[j] += matrix_.data[i * N + j];
  }

  bool CheckTestOutputData(OutType& output_data) final {
    return output_data == expected_;
  }

  InType GetTestInputData() final {
    return matrix_;
  }
};

TEST_P(KopilovDSumValColMatPerfTests, RunPerfModes) {
  ExecuteTest(GetParam());
}

// Создаём perf-задачи для MPI и SEQ
const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType,
                                KopilovDSumValColMatMPI,
                                KopilovDSumValColMatSEQ>(
        PPC_SETTINGS_kopilov_d_sum_val_col_mat);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName =
    KopilovDSumValColMatPerfTests::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests,
                         KopilovDSumValColMatPerfTests,
                         kGtestValues,
                         kPerfTestName);

}  // namespace kopilov_d_sum_val_col_mat

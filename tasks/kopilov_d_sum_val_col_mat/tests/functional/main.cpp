#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"
#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"
#include "kopilov_d_sum_val_col_mat/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace kopilov_d_sum_val_col_mat {

class KopilovDSumValColMatTests
    : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType& test_param) {
    return std::to_string(std::get<0>(test_param)) + "_" + std::get<1>(test_param);
  }

 protected:
  // Генерация входной матрицы
  void SetUp() override {
    const auto params =
        std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(
            GetParam());
    int size = std::get<0>(params);  // квадратная матрица size x size

    input_.rows = size;
    input_.cols = size;
    input_.data.resize(size * size);

    // Заполняем детерминированно, чтобы SEQ и MPI совпадали
    std::iota(input_.data.begin(), input_.data.end(), 1);

    // Ожидаемый результат
    expected_output_.resize(size, 0);

    for (int j = 0; j < size; ++j)
      for (int i = 0; i < size; ++i)
        expected_output_[j] += input_.data[i * size + j];
  }

  bool CheckTestOutputData(OutType& output_data) final {
    return output_data == expected_output_;
  }

  InType GetTestInputData() final { return input_; }

 private:
  InType input_;
  OutType expected_output_;
};

namespace {

TEST_P(KopilovDSumValColMatTests, SumColumnsMatrix) {
  ExecuteTest(GetParam());
}

// Параметры тестов
const std::array<TestType, 3> kTestParam = {
    std::make_tuple(3, "3"),
    std::make_tuple(5, "5"),
    std::make_tuple(7, "7"),
};

// Список MPI и SEQ задач
const auto kTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<KopilovDSumValColMatMPI, InType>(
        kTestParam, PPC_SETTINGS_kopilov_d_sum_val_col_mat),
    ppc::util::AddFuncTask<KopilovDSumValColMatSEQ, InType>(
        kTestParam, PPC_SETTINGS_kopilov_d_sum_val_col_mat));

const auto kValues = ppc::util::ExpandToValues(kTasksList);
const auto kTestName =
    KopilovDSumValColMatTests::PrintFuncTestName<KopilovDSumValColMatTests>;

INSTANTIATE_TEST_SUITE_P(FunctionalTests,
                         KopilovDSumValColMatTests,
                         kValues,
                         kTestName);

}  // namespace
}  // namespace kopilov_d_sum_val_col_mat

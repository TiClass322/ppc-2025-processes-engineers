#include <gtest/gtest.h>
#include <mpi.h>

#include <array>
#include <cstddef>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

#include "kopilov_d_ring_2/common/include/common.hpp"
#include "kopilov_d_ring_2/mpi/include/ops_mpi.hpp"
#include "kopilov_d_ring_2/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace kopilov_d_ring_2 {

class KopilovDRunFuncTestsProcesses : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    auto initial_value = std::get<0>(test_param);
    std::string name = "val_" + std::to_string(std::abs(initial_value));
    if (initial_value < 0) {
      name = "neg_" + name;
    }
    return name;
  }

 protected:
  void SetUp() override {
    int world_size = 1;
    const auto &test_param = GetParam();
    const std::string &test_name = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kNameTest)>(test_param);
    const bool is_sequential = test_name.find("_seq") != std::string::npos;

    if (!is_sequential) {
      MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    }

    const auto &params = std::get<static_cast<size_t>(ppc::util::GTestParamIndex::kTestParams)>(test_param);
    const auto initial_value = std::get<0>(params);

    input_data_.value = initial_value;

    int total_sum = 0;
    const int process_count_for_calc = is_sequential ? 1 : world_size;
    for (int i = 0; i < process_count_for_calc; ++i) {
      total_sum += i;
    }
    expected_output_.value = input_data_.value + total_sum;
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

namespace {

TEST_P(KopilovDRunFuncTestsProcesses, RingTopologySum) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 5> kTestParam = {std::make_tuple(10), std::make_tuple(0), std::make_tuple(-5),
                                            std::make_tuple(100), std::make_tuple(42)};

const auto kTestTasksList =
    std::tuple_cat(ppc::util::AddFuncTask<KopilovDRingMPI, InType>(kTestParam, PPC_SETTINGS_kopilov_d_ring_2),
                   ppc::util::AddFuncTask<KopilovDRingSEQ, InType>(kTestParam, PPC_SETTINGS_kopilov_d_ring_2));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kTestName = KopilovDRunFuncTestsProcesses::PrintFuncTestName<KopilovDRunFuncTestsProcesses>;

INSTANTIATE_TEST_SUITE_P(RingTopologyTests, KopilovDRunFuncTestsProcesses, kGtestValues, kTestName);

}  // namespace

}  // namespace kopilov_d_ring_2

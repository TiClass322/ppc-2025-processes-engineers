#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"
#include <mpi.h>
#include <vector>
#include <numeric>

namespace kopilov_d_sum_val_col_mat {

KopilovDSumValColMatMPI::KopilovDSumValColMatMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput().resize(GetInput().cols, 0);
}

bool KopilovDSumValColMatMPI::ValidationImpl() {
  return GetInput().rows > 0 && GetInput().cols > 0 &&
         GetInput().data.size() == (size_t)GetInput().rows * GetInput().cols &&
         GetOutput().size() == (size_t)GetInput().cols;
}

bool KopilovDSumValColMatMPI::PreProcessingImpl() {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
      for (size_t i = 0; i < GetOutput().size(); ++i) {
        GetOutput()[i] = 0;
      }
  }
  return true;
}

bool KopilovDSumValColMatMPI::RunImpl() {
  int rank = 0;
  int world_size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int dims[2]; // 0: rows, 1: cols
  if (rank == 0) {
    dims[0] = GetInput().rows;
    dims[1] = GetInput().cols;
  }
  MPI_Bcast(dims, 2, MPI_INT, 0, MPI_COMM_WORLD);
  const int rows = dims[0];
  const int cols = dims[1];

  const int elements_per_proc = rows / world_size;
  const int remainder = rows % world_size;

  std::vector<int> sendcounts;
  std::vector<int> displs;
  
  if (rank == 0) {
      sendcounts.resize(world_size);
      displs.resize(world_size);
      int current_displ = 0;
      for (int i = 0; i < world_size; i++) {
        int rows_for_proc = elements_per_proc + (i < remainder ? 1 : 0);
        sendcounts[i] = rows_for_proc * cols;
        displs[i] = current_displ;
        current_displ += sendcounts[i];
      }
  }

  int local_rows = elements_per_proc + (rank < remainder ? 1 : 0);
  std::vector<int> local_data(local_rows * cols);
  
  const int* input_data_ptr = (rank == 0) ? GetInput().data.data() : nullptr;

  MPI_Scatterv(input_data_ptr, sendcounts.data(), displs.data(), MPI_INT,
               local_data.data(), local_data.size(), MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> local_sum(cols, 0);
  for (int j = 0; j < cols; ++j) {
    for (int i = 0; i < local_rows; ++i) {
      local_sum[j] += local_data[i * cols + j];
    }
  }

  std::vector<int> global_sum;
  if (rank == 0) {
    global_sum.resize(cols);
  }

  MPI_Reduce(local_sum.data(), global_sum.data(), cols, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    GetOutput() = global_sum;
  }

  return true;
}

bool KopilovDSumValColMatMPI::PostProcessingImpl() {
  return true;
}

} // namespace kopilov_d_sum_val_col_mat

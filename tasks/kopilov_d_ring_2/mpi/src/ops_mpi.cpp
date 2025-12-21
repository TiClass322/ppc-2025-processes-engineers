#include "kopilov_d_ring_2/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <stdexcept>
#include <vector>

#include "kopilov_d_ring_2/common/include/common.hpp"

namespace kopilov_d_ring_2 {

KopilovDRingMPI::KopilovDRingMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = OutType{0};
}

bool KopilovDRingMPI::ValidationImpl() {
  return true;
}

bool KopilovDRingMPI::PreProcessingImpl() {
  return true;
}

bool KopilovDRingMPI::RunImpl() {
  int rank = 0;
  int size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const int num_iterations = 10000;
  for (int i = 0; i < num_iterations; ++i) {
    if (size == 1) {
      GetOutput().value = GetInput().value + rank;
      continue;
    }

    int current_value = 0;
    const int next_rank = (rank + 1) % size;
    const int prev_rank = (rank == 0) ? size - 1 : rank - 1;

    if (rank == 0) {
      current_value = GetInput().value;
      current_value += rank;
      MPI_Send(&current_value, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
      MPI_Recv(&current_value, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
      MPI_Recv(&current_value, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      current_value += rank;
      MPI_Send(&current_value, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }

    MPI_Bcast(&current_value, 1, MPI_INT, 0, MPI_COMM_WORLD);
    GetOutput().value = current_value;
  }
  return true;
}

bool KopilovDRingMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kopilov_d_ring_2

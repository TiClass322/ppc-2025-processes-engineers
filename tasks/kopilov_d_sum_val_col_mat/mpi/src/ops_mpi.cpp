#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <stdexcept>
#include <vector>

#include "kopilov_d_sum_val_col_mat/common/include/common.hpp"

namespace kopilov_d_sum_val_col_mat {

KopilovDSumValColMatMPI::KopilovDSumValColMatMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = OutType{};
}

bool KopilovDSumValColMatMPI::ValidationImpl() {
  return true;
}

bool KopilovDSumValColMatMPI::PreProcessingImpl() {
  return true;
}

bool KopilovDSumValColMatMPI::RunImpl() {
  int worldRank = 0;
  int worldSize = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

  int rows = 0;
  int cols = 0;
  const double *sendBufferPtr = nullptr;

  if (worldRank == 0) {
    rows = GetInput().rows;
    cols = GetInput().cols;
    const std::size_t expectedSize = static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols);
    if (GetInput().data.size() != expectedSize) {
      return false;
    }
    sendBufferPtr = GetInput().data.data();
  }

  MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rows == 0 || cols == 0) {
    GetOutput().col_sum.clear();
    return true;
  }

  // compute rows per process (by rows, not by elements) to avoid fractional rows
  std::vector<int> rowsPerRank(static_cast<std::size_t>(worldSize), 0);
  const int baseRows = rows / worldSize;
  const int remainderRows = rows % worldSize;
  for (int pid = 0; pid < worldSize; ++pid) {
    rowsPerRank[static_cast<std::size_t>(pid)] = baseRows + (pid < remainderRows ? 1 : 0);
  }

  // sendcounts/displs are in number of doubles (elements)
  std::vector<int> sendCounts(static_cast<std::size_t>(worldSize), 0);
  std::vector<int> displacements(static_cast<std::size_t>(worldSize), 0);
  int offset = 0;
  for (int pid = 0; pid < worldSize; ++pid) {
    sendCounts[static_cast<std::size_t>(pid)] = rowsPerRank[static_cast<std::size_t>(pid)] * cols;
    displacements[static_cast<std::size_t>(pid)] = offset;
    offset += sendCounts[static_cast<std::size_t>(pid)];
  }

  const int recvCount = sendCounts[static_cast<std::size_t>(worldRank)];
  std::vector<double> recvBuffer(static_cast<std::size_t>(recvCount), 0.0);

  MPI_Scatterv(sendBufferPtr, sendCounts.data(), displacements.data(), MPI_DOUBLE, recvBuffer.data(), recvCount,
               MPI_DOUBLE, 0, MPI_COMM_WORLD);

  const int localRows = (cols == 0) ? 0 : static_cast<int>(recvBuffer.size()) / cols;
  std::vector<double> localColSum(static_cast<std::size_t>(cols), 0.0);

  for (int row = 0; row < localRows; ++row) {
    for (int col = 0; col < cols; ++col) {
      localColSum[static_cast<std::size_t>(col)] +=
          recvBuffer[static_cast<std::size_t>(row) * static_cast<std::size_t>(cols) + static_cast<std::size_t>(col)];
    }
  }

  std::vector<double> globalColSum(static_cast<std::size_t>(cols), 0.0);
  MPI_Reduce(localColSum.data(), globalColSum.data(), cols, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  // Broadcast result to all processes so tests can validate on any rank
  MPI_Bcast(globalColSum.data(), cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  GetOutput().col_sum = std::move(globalColSum);

  return true;
}

bool KopilovDSumValColMatMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kopilov_d_sum_val_col_mat

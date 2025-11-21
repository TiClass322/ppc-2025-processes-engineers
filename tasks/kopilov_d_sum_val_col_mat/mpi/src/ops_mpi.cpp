#include "kopilov_d_sum_val_col_mat/mpi/include/ops_mpi.hpp"

#include <mpi.h>
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
    int rank = 0, size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 0, cols = 0;
    const double* sendbuf_ptr = nullptr;

    if (rank == 0) {
        rows = GetInput().rows;
        cols = GetInput().cols;
        if ((int)GetInput().data.size() != rows * cols) return false;
        sendbuf_ptr = GetInput().data.data();
    }

    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rows == 0 || cols == 0) {
        GetOutput().col_sum.clear();
        return true;
    }

    std::vector<int> rows_per_rank(size, 0);
    int base = rows / size;
    int rem = rows % size;
    for (int r = 0; r < size; ++r) {
        rows_per_rank[r] = base + (r < rem ? 1 : 0);
    }

    std::vector<int> sendcounts(size, 0);
    std::vector<int> displs(size, 0);
    int offset = 0;
    for (int r = 0; r < size; ++r) {
        sendcounts[r] = rows_per_rank[r] * cols;
        displs[r] = offset;
        offset += sendcounts[r];
    }

    int recvcount = sendcounts[rank];
    std::vector<double> recvbuf(recvcount, 0.0);

    MPI_Scatterv(sendbuf_ptr, sendcounts.data(), displs.data(), MPI_DOUBLE,
                 recvbuf.data(), recvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> local_col_sum(cols, 0.0);
    int local_rows = recvbuf.size() / cols;
    for (int r = 0; r < local_rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            local_col_sum[c] += recvbuf[r * cols + c];
        }
    }

    std::vector<double> global_col_sum(cols, 0.0);
    MPI_Reduce(local_col_sum.data(), global_col_sum.data(), cols,
               MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Bcast(global_col_sum.data(), cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    GetOutput().col_sum = global_col_sum;

    return true;
}

bool KopilovDSumValColMatMPI::PostProcessingImpl() {
    return true;
}

}  // namespace kopilov_d_sum_val_col_mat

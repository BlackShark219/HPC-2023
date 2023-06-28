#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int m = 8; // number of rows
    int n = 12; // number of columns
    int r = 3;  // number of rows per process

    // matrix initialization
    double* matrix = malloc(sizeof(double) * m * n);
    int i;
    for (i = 0; i < m * n; i++) {
        matrix[i] = (double) (i + 1);
    }

    // divide matrix per process
    int count = r * n;
    double* local_matrix = malloc(sizeof(double) * count);
    MPI_Scatter(matrix, count, MPI_DOUBLE, local_matrix, count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // search min element per part
    double local_min = local_matrix[0];
    for (i = 0; i < r * n; i++) {
        if (local_matrix[i] < local_min) {
            local_min = local_matrix[i];
        }
    }

    // min element global
    double global_min;
    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // print generated matrix
        printf("Initialized matrix: \n");
        for (i = 0; i < m * n; i++) {
            printf("%.2f ", matrix[i]);
            if ((i + 1) % n == 0) {
                printf("\n");
            }
        }
        // print global min element
        printf("Minimum element: %.2f\n", global_min);
    }

    free(matrix);
    free(local_matrix);
    MPI_Finalize();
    return 0;
}

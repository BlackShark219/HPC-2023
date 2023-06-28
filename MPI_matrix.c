#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int processRank, numProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numRows = 8; // number of rows
    int numCols = 12; // number of columns
    int rowsPerProcess = 3;  // number of rows per process

    // matrix initialization
    double* matrix = malloc(sizeof(double) * numRows * numCols);
    int i;
    for (i = 0; i < numRows * numCols; i++) {
        matrix[i] = (double) rand() / RAND_MAX;
    }

    // divide matrix per process
    int count = rowsPerProcess * numCols;
    double* localMatrix = malloc(sizeof(double) * count);
    MPI_Scatter(matrix, count, MPI_DOUBLE, localMatrix, count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // search max element per part
    double localMax = localMatrix[0];
    for (i = 0; i < rowsPerProcess * numCols; i++) {
        if (localMatrix[i] > localMax) {
            localMax = localMatrix[i];
        }
    }

    // max element global
    double globalMax;
    MPI_Reduce(&localMax, &globalMax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (processRank == 0) {
        // print generated matrix
        printf("Initialized matrix: \n");
        for (i = 0; i < numRows * numCols; i++) {
            printf("%f ", matrix[i]);
            if ((i + 1) % numCols == 0) {
                printf("\n");
            }
        }
        // print global max element
        printf("Maximum element: %f\n", globalMax);
    }

    free(matrix);
    free(localMatrix);
    MPI_Finalize();
    return 0;
}

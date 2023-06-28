#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int processRank, numProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int numRows = 10; // кількість рядків
    int numCols = 20; // кількість стовпців
    int rowsPerProcess = 2;  // кількість рядків на процес

    // ініціалізація матриці
    double* matrix = malloc(sizeof(double) * numRows * numCols);
    int i;
    for (i = 0; i < numRows * numCols; i++) {
        matrix[i] = (double) rand() / RAND_MAX;
    }

    // розподіл матриці між процесами
    int count = rowsPerProcess * numCols;
    double* localMatrix = malloc(sizeof(double) * count);
    MPI_Scatter(matrix, count, MPI_DOUBLE, localMatrix, count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // пошук максимального елемента в кожній частині
    double localMax = localMatrix[0];
    for (i = 0; i < rowsPerProcess * numCols; i++) {
        if (localMatrix[i] > localMax) {
            localMax = localMatrix[i];
        }
    }

    // глобальний максимальний елемент
    double globalMax;
    MPI_Reduce(&localMax, &globalMax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (processRank == 0) {
        // виведення початкової матриці
        printf("Initialized matrix: \n");
        for (i = 0; i < numRows * numCols; i++) {
            printf("%f ", matrix[i]);
            if ((i + 1) % numCols == 0) {
                printf("\n");
            }
        }
        // виведення глобального максимального елемента
        printf("Maximum element: %f\n", globalMax);
    }

    free(matrix);
    free(localMatrix);
    MPI_Finalize();
    return 0;
}

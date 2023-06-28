#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>

#define M_SIZE 1000

// Друк матриці
void printMatrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// Ініціалізація матриці значеннями від 1 до 100
void initializeMatrix(double** matrix, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        matrix[i] = new double[size];
        for (int j = 0; j < size; j++) {
            if (i == j) {
                matrix[i][j] = rand() % 100 + 101; // Діагональні елементи від 101 до 200
            } else {
                matrix[i][j] = rand() % 100 + 1; // Недіагональні елементи від 1 до 100
            }
        }
    }
}


void gramSchmidt(double** matrix, int size) {
    #pragma omp parallel for
    for (int col = 0; col < size; col++) {
        for (int prevCol = 0; prevCol < col; prevCol++) {
            double dotProduct = 0.0;
            #pragma omp parallel for reduction(+:dotProduct)
            for (int row = 0; row < size; row++) {
                dotProduct += matrix[row][prevCol] * matrix[row][col];
            }
            #pragma omp parallel for
            for (int row = 0; row < size; row++) {
                matrix[row][col] -= dotProduct * matrix[row][prevCol];
            }
        }
        double norm = 0.0;
        #pragma omp parallel for reduction(+:norm)
        for (int row = 0; row < size; row++) {
            norm += matrix[row][col] * matrix[row][col];
        }
        norm = std::sqrt(norm);
        #pragma omp parallel for
        for (int row = 0; row < size; row++) {
            matrix[row][col] /= norm;
        }
    }
}

int main() {
    double** matrix = new double*[M_SIZE];
    initializeMatrix(matrix, M_SIZE);

    std::cout << "Initialized matrix: " << std::endl;
    printMatrix(matrix, M_SIZE);

    double startTime = omp_get_wtime();
    gramSchmidt(matrix, M_SIZE);
    double endTime = omp_get_wtime();
    std::cout << "Execution time: " << endTime - startTime << " seconds." << std::endl;

    std::cout << "Result matrix: " << std::endl;
    printMatrix(matrix, M_SIZE);

    for (int i = 0; i < M_SIZE; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}

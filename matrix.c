#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 4  // Default size of the matrix

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

// Structure to pass thread arguments
typedef struct {
    int row;
    int col;
} ThreadArgs;

void fillMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* computeSum(void* args) {
    ThreadArgs* myArgs = (ThreadArgs*)args;
    int row = myArgs->row;
    int col = myArgs->col;
    matSumResult[row][col] = matA[row][col] + matB[row][col];
    free(myArgs);
    pthread_exit(NULL);
}

void* computeDiff(void* args) {
    ThreadArgs* myArgs = (ThreadArgs*)args;
    int row = myArgs->row;
    int col = myArgs->col;
    matDiffResult[row][col] = matA[row][col] - matB[row][col];
    free(myArgs);
    pthread_exit(NULL);
}

void* computeProduct(void* args) {
    ThreadArgs* myArgs = (ThreadArgs*)args;
    int row = myArgs->row;
    int col = myArgs->col;

    // Calculate the product for the specified cell
    int result = 0;
    for (int k = 0; k < MAX; k++) {
        result += matA[row][k] * matB[k][col];
    }
    matProductResult[row][col] = result;

    free(myArgs);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    srand(time(0));

    // Declare a variable for matrix size
    int matrixSize;

    // Check if the matrix size is provided as a command line argument
    if (argc != 2) {
        printf("Usage: %s <matrix_size>\n", argv[0]);
        return 1;
    }

    // Set the matrix size
    matrixSize = atoi(argv[1]);

    // Check if the matrix size is valid
    if (matrixSize <= 0 || matrixSize > MAX) {
        printf("Invalid matrix size. Please choose a size between 1 and %d.\n", MAX);
        return 1;
    }

    fillMatrix(matA);
    fillMatrix(matB);

    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    pthread_t sumThreads[matrixSize][matrixSize];
    pthread_t diffThreads[matrixSize][matrixSize];
    pthread_t productThreads[matrixSize][matrixSize];

    // Create threads for sum
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            pthread_create(&sumThreads[i][j], NULL, computeSum, args);
        }
    }

    // Create threads for diff
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            pthread_create(&diffThreads[i][j], NULL, computeDiff, args);
        }
    }

    // Create threads for product
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            pthread_create(&productThreads[i][j], NULL, computeProduct, args);
        }
    }

    // Wait for sum threads to finish
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            pthread_join(sumThreads[i][j], NULL);
        }
    }

    // Wait for diff threads to finish
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            pthread_join(diffThreads[i][j], NULL);
        }
    }

    // Wait for product threads to finish
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            pthread_join(productThreads[i][j], NULL);
        }
    }

    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILENAME "20_Million.txt"
#define SIZE 20000000
#define TARGET_VALUE 5000000

int sequential_binary_search(int *arr, int size, int key) {
    int low = 0;
    int high = size - 1;
    int mid;
    int result = -1;

    while (low <= high) {
        mid = (low + high) / 2;

        if (key == arr[mid]) {
            result = mid;
            break;
        } else if (key < arr[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return result;
}

int parallel_binary_search(int *arr, int local_size, int key, int rank,
                           int size_mpi) {
    int low = 0;
    int high = local_size - 1;
    int mid, global_mid;
    int local_result = -1;

    while (low <= high) {
        mid = (low + high) / 2;

        if (key == arr[mid]) {
            local_result = mid;
            break;
        } else if (key < arr[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    global_mid = low + rank * local_size;
    MPI_Allreduce(&local_result, &global_mid, 1, MPI_INT, MPI_MAX,
                  MPI_COMM_WORLD);

    if (global_mid == -1) {
        return -1;
    }
    global_mid += rank * local_size;

    return global_mid;
}

int main(int argc, char *argv[]) {
    int rank, size, local_size;
    int *arr;
    int result;
    double start_time, end_time;
    FILE *file;

    file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    arr = (int *)malloc(SIZE * sizeof(int));
    for (int i = 0; i < SIZE; i++) {
        fscanf(file, "%d", &arr[i]);
    }

    // Sequential binary search
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    local_size = SIZE / size;
    
    start_time = MPI_Wtime();
    result = sequential_binary_search(arr, SIZE, TARGET_VALUE);
    end_time = MPI_Wtime();

    if (result != -1) {
        if (rank == 0) {
            printf("Sequential: Element found at index %d\n", result);
            printf("Time taken: %f seconds\n", end_time - start_time);
        }
    } else {
        if (rank == 0) {
            printf("Sequential: Element not found\n");
            printf("Time taken: %f seconds\n", end_time - start_time);
        }
    }
    
    result = -1;
    // MPI binary search
    start_time = MPI_Wtime();
    result = parallel_binary_search(arr, local_size, TARGET_VALUE, rank, size);
    end_time = MPI_Wtime();

    if (result != -1) {
        if (rank == 0) {
            printf("\nMPI: Element found at index %d\n", result);
            printf("Time taken: %f seconds\n", end_time - start_time);
        }
    } else {
        if (rank == 0) {
            printf("\nMPI: Element not found\n");
            printf("Time taken: %f seconds\n", end_time - start_time);
        }
    }

    free(arr);

    MPI_Finalize();

    return 0;
}

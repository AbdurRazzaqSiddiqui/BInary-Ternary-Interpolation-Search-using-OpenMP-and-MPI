#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 2000000
#define FILENAME "2_Million.txt"
#define TARGET_VALUE 999990
int arr[MAX_SIZE];

int ternarySearchSequential(int arr[], int left, int right, int target) {
    if (left <= right) {
        int mid1 = left + (right - left) / 3;
        int mid2 = right - (right - left) / 3;

        if (arr[mid1] == target)
            return mid1;
        if (arr[mid2] == target)
            return mid2;

        if (target < arr[mid1])
            return ternarySearchSequential(arr, left, mid1 - 1, target);
        else if (target > arr[mid2])
            return ternarySearchSequential(arr, mid2 + 1, right, target);
        else
            return ternarySearchSequential(arr, mid1 + 1, mid2 - 1, target);
    }
    return -1;
}
int ternarySearchParallel(int arr[], int left, int right, int target) {
    while (left <= right) {
        int mid1 = left + (right - left) / 3;
        int mid2 = right - (right - left) / 3;

        if (arr[mid1] == target) {
            return mid1;
        }
        if (arr[mid2] == target) {
            return mid2;
        }

        if (target < arr[mid1]) {
            right = mid1 - 1;
        } else if (target > arr[mid2]) {
            left = mid2 + 1;
        } else {
            left = mid1 + 1;
            right = mid2 - 1;
        }
    }

    return -1;
}
int main(int argc, char *argv[]) {
    FILE *file;

    file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        fscanf(file, "%d", &arr[i]);
    }

    double start_time_serial = omp_get_wtime();
    int result_serial =
        ternarySearchSequential(arr, 0, MAX_SIZE - 1, TARGET_VALUE);
    double end_time_serial = omp_get_wtime();

    printf("\nSerial Result: %d\n", result_serial);
    printf("Serial Execution Time: %f seconds\n",
           end_time_serial - start_time_serial);

    int result_parallel;
    double start_time_parallel = omp_get_wtime();
    omp_set_num_threads(atoi(argv[1]));
    #pragma omp parallel
    {
        #pragma omp single
        {
            result_parallel = ternarySearchParallel(arr, 0, MAX_SIZE - 1, TARGET_VALUE);
        }
    }
    double end_time_parallel = omp_get_wtime();

    printf("\nParallel Result: %d\n", result_parallel);
    printf("Parallel Execution Time: %f seconds\n",
           end_time_parallel - start_time_parallel);

    double speedup = (end_time_serial - start_time_serial) /
                     (end_time_parallel - start_time_parallel);
    printf("\nSpeedup: %f\n", speedup);

    return 0;
}

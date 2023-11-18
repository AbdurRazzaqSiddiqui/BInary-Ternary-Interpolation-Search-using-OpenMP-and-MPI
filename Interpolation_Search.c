#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 2000000
#define FILENAME "2_Million.txt"
#define TARGET_VALUE 999990
int arr[MAX_SIZE];

int serialInterpolationSearch(int arr[], int l, int r, int key) {
    while (l <= r && key >= arr[l] && key <= arr[r]) {
        if (l == r) {
            if (arr[l] == key)
                return l;
            return -1;
        }

        int pos = l + (((double)(r - l) / (arr[r] - arr[l])) * (key - arr[l]));

        if (arr[pos] == key)
            return pos;

        if (arr[pos] < key)
            l = pos + 1;
        else
            r = pos - 1;
    }

    return -1;
}

int parallelInterpolationSearch(int arr[], int l, int r, int key) {
    while (l <= r && key >= arr[l] && key <= arr[r]) {
        if (l == r) {
            if (arr[l] == key)
                return l;
            return -1;
        }

        int pos;

        #pragma omp parallel
        {
            #pragma omp single nowait
            { pos = l + (((double)(r - l) / (arr[r] - arr[l])) * (key - arr[l])); }
        }

        if (arr[pos] == key)
            return pos;

        if (arr[pos] < key)
            l = pos + 1;
        else
            r = pos - 1;
    }

    return -1;
}

int main(int argc, char *argv[]) {
    int nos = MAX_SIZE;
    int nthread = atoi(argv[1]);

    FILE *file;

    file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        fscanf(file, "%d", &arr[i]);
    }

    // Serial version
    double start_time_serial = omp_get_wtime();
    int result_serial = serialInterpolationSearch(arr, 0, nos - 1, TARGET_VALUE);
    double end_time_serial = omp_get_wtime();

    printf("\nSerial Result: %d\n", result_serial);
    printf("Serial Execution Time: %f seconds\n",
           end_time_serial - start_time_serial);

    // Parallel version
    omp_set_num_threads(nthread);
    double start_time_parallel = omp_get_wtime();
    int result_parallel =
        parallelInterpolationSearch(arr, 0, nos - 1, TARGET_VALUE);
    double end_time_parallel = omp_get_wtime();

    printf("\nParallel Result: %d\n", result_parallel);
    printf("Parallel Execution Time: %f seconds\n",
           end_time_parallel - start_time_parallel);

    double speedup = (end_time_serial - start_time_serial) /
                     (end_time_parallel - start_time_parallel);
    printf("\nSpeedup: %f\n", speedup);

    return 0;
}

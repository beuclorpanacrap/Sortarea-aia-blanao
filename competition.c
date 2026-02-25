#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAXN 1000000

// Rdix sort for signed 32-bit integers using byte-wise passes
void radix_sort_signed(int *arr, int n) {
    uint32_t *u = (uint32_t *)arr;
    uint32_t *tmp = (uint32_t *)malloc(n * sizeof(uint32_t));

    // Flip sign bit so negatives come before positives in unsigned order
    for (int i = 0; i < n; i++) {
        u[i] ^= 0x80000000u;
    }

    // 4 passes (one per byte)
    for (int pass = 0; pass < 4; pass++) {
        int count[256] = {0};

        int shift = pass * 8;

        // Count occurrences
        for (int i = 0; i < n; i++) {
            count[(u[i] >> shift) & 0xFF]++;
        }

        // Prefix sums
        int sum = 0;
        for (int i = 0; i < 256; i++) {
            int t = count[i];
            count[i] = sum;
            sum += t;
        }

        // Stable distribution
        for (int i = 0; i < n; i++) {
            tmp[count[(u[i] >> shift) & 0xFF]++] = u[i];
        }

        // Copy back
        memcpy(u, tmp, n * sizeof(uint32_t));
    }

    // Restore original sign
    for (int i = 0; i < n; i++) {
        u[i] ^= 0x80000000u;
    }

    free(tmp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s input_file [stdout|output_file]\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    FILE *fin = fopen(input_file, "r");

    if (!fin) {
        printf("Cannot open input file.\n");
        return 1;
    }

    int *arr = (int *)malloc(MAXN * sizeof(int));
    int n = 0;

    // Read integers from file
    while (fscanf(fin, "%d", &arr[n]) == 1) {
        n++;
    }
    fclose(fin);

    // Measure sorting time only
    clock_t start_sort = clock();
    radix_sort_signed(arr, n);
    clock_t end_sort = clock();

    double sort_time = (double)(end_sort - start_sort) / CLOCKS_PER_SEC;

    // Handle output destination
    FILE *fout = NULL;
    int print_stdout = 0;

    if (argc >= 3) {
        if (strcmp(argv[2], "stdout") == 0) {
            print_stdout = 1;
        } else {
            fout = fopen(argv[2], "w");
            if (!fout) {
                printf("Cannot open output file.\n");
                return 1;
            }
        }
    }

    clock_t start_total = clock();

    if (argc >= 3) {
        if (print_stdout) {
            for (int i = 0; i < n; i++) {
                printf("%d\n", arr[i]);
            }
        } else {
            for (int i = 0; i < n; i++) {
                fprintf(fout, "%d\n", arr[i]);
            }
            fclose(fout);
        }
    }

    clock_t end_total = clock();

    double total_time = (double)(end_total - start_total) / CLOCKS_PER_SEC;

    printf("Sorting time only: %.6f seconds\n", sort_time);
    printf("Sorting + output time: %.6f seconds\n", sort_time + total_time);

    free(arr);
    return 0;
}

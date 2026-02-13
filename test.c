#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* ============================================================
   Your existing functions (unchanged)
   ============================================================ */

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    printf("\n");
}

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int digits(int x){
    int i=0;
    do{ x=x/10; i++; } while(x>0);
    return i;
}

int digit(int x,int poz){
    for(int i=0;i<poz;i++) x=x/10;
    return x%10;
}

void radix_sort(int a[],int x){
    int i,ma=0,e,n,r;
    int *aux=(int *)malloc(x*sizeof(int));
    if (!aux) return;

    for(i=0;i<x;i++){
        if(digits(abs(a[i]))>ma) ma=digits(abs(a[i]));
    }
    for(i=0;i<ma;i++){
        e=0;
        for(n=-9;n<=9;n++){
            for(r=0;r<x;r++){
                if(digit(a[r],i)==n){
                    aux[e]=a[r];
                    e++;
                }
            }
        }
        memcpy(a,aux,x*sizeof(int));
    }
    free(aux);
}

void radix_sort2(int a[],int x){
    int i,ma=0,e,n,r;
    int *aux=(int *)malloc(x*sizeof(int));
    if (!aux) return;

    for(i=0;i<x;i++){
        if(digits(a[i])>ma) ma=digits(a[i]);
    }
    for(i=0;i<ma;i++){
        e=0;
        for(n=0;n<=9;n++){
            for(r=0;r<x;r++){
                if(digit(a[r],i)==n){
                    aux[e]=a[r];
                    e++;
                }
            }
        }
        memcpy(a,aux,x*sizeof(int));
    }
    free(aux);
}

int readInputFile(const char *filename, int **outArr, int *outN) {
    FILE *in = fopen(filename, "r");
    if (!in) return 0;

    int n;
    if (fscanf(in, "%d", &n) != 1 || n <= 0) { fclose(in); return 0; }

    int *arr = (int*)malloc(n * sizeof(int));
    if (!arr) { fclose(in); return 0; }

    for (int i = 0; i < n; i++) {
        if (fscanf(in, "%d", &arr[i]) != 1) {
            free(arr);
            fclose(in);
            return 0;
        }
    }

    fclose(in);
    *outArr = arr;
    *outN = n;
    return 1;
}

int writeOutput(const char *dest, int *arr, int n) {
    FILE *out = NULL;

    if (strcmp(dest, "stdout") == 0) {
        out = stdout;
    } else {
        out = fopen(dest, "w");
        if (!out) return 0;
    }

    for (int i = 0; i < n; i++) {
        fprintf(out, "%d", arr[i]);
        if (i + 1 < n) fprintf(out, " ");
    }
    fprintf(out, "\n");

    if (out != stdout) fclose(out);
    return 1;
}

/* ============================================================
   New benchmark/menu + dataset generation + IO selection
   ============================================================ */

typedef enum {
    ALG_BUBBLE = 1,
    ALG_INSERTION,
    ALG_QUICK,
    ALG_HEAP,
    ALG_RADIX_NONNEG,
    ALG_RADIX_SIGNED,
    ALG_ALL
} Algorithm;

typedef enum {
    ORDER_ASC = 1,
    ORDER_DESC,
    ORDER_RAND
} OrderType;

typedef enum {
    INP_GENERATE = 1,
    INP_FILE,
    INP_KEYBOARD
} InputMethod;

void flush_line() {
    int c;
    while ((c = getchar()) != '\n') {}
}

int read_int_safe(int *out) {
    int x;
    if (scanf("%d", &x) != 1) {
        flush_line();
        return 0;
    }
    return (*out = x), 1;
}

int get_size_from_choice(int choice) {
    switch (choice) {
        case 1: return 100;
        case 2: return 1000;
        case 3: return 10000;
        case 4: return 100000;
        case 5: return 1000000;
        default: return 0;
    }
}

void reverse_array(int *a, int n) {
    for (int i = 0, j = n - 1; i < j; i++, j--) swap(&a[i], &a[j]);
}

void fill_random_nonneg(int *a, int n, int max) {
    for (int i = 0; i < n; i++) a[i] = rand() % (max + 1);
}

void fill_random_signed(int *a, int n, int maxabs) {
    for (int i = 0; i < n; i++) {
        int v = rand() % (maxabs + 1);
        if (rand() & 1) v = -v;
        a[i] = v;
    }
}

void generate_dataset(int **outArr, int n, OrderType order, int want_signed) {
    int *a = (int*)malloc(n * sizeof(int));
    if (!a) { *outArr = NULL; return; }

    if (want_signed) fill_random_signed(a, n, 1000000);
    else fill_random_nonneg(a, n, 1000000);

    if (order == ORDER_ASC) {
        quickSort(a, 0, n - 1);
    } else if (order == ORDER_DESC) {
        quickSort(a, 0, n - 1);
        reverse_array(a, n);
    }

    *outArr = a;
}

int read_keyboard_array(int **outArr, int n) {
    int *a = (int*)malloc(n * sizeof(int));
    if (!a) return 0;

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1) {
            free(a);
            flush_line();
            return 0;
        }
    }
    *outArr = a;
    return 1;
}

void print_algorithm_menu() {
    printf("\n=== Algorithm Menu ===\n");
    printf("1) Bubble\n");
    printf("2) Insertion\n");
    printf("3) Quick\n");
    printf("4) Heap\n");
    printf("5) Radix (nonnegative only) [radix_sort2]\n");
    printf("6) Radix (signed)           [radix_sort]\n");
    printf("7) ALL (runs each one)\n");
    printf("0) Exit\n");
    printf("Choice: ");
}

void print_size_menu() {
    printf("\nChoose test size:\n");
    printf("1) 100\n");
    printf("2) 1,000\n");
    printf("3) 10,000\n");
    printf("4) 100,000\n");
    printf("5) 1,000,000\n");
    printf("Choice: ");
}

void print_order_prompt(void) {
    printf("Order on same line (asc/desc/rand): ");
}

OrderType order_convertor(char *s) {
    if (strcmp(s, "asc") == 0) return ORDER_ASC;
    if (strcmp(s, "desc") == 0) return ORDER_DESC;
    return ORDER_RAND;
}

void print_input_menu(void) {
    printf("\nArray input method:\n");
    printf("1) Generate automatically (default)\n");
    printf("2) Read from file\n");
    printf("3) Enter from keyboard\n");
    printf("Choice: ");
}

int load_array(InputMethod m, int n, OrderType order, int want_signed, int **arr, int *outN) {
    *arr = NULL;
    *outN = 0;

    if (m == INP_FILE) {
        char path[101];
        printf("File path: ");
        scanf("%100s", path);

        if (!readInputFile(path, arr, outN)) {
            printf("ERROR: Could not read file.\n");
            return 0;
        }
        return 1;
    }

    if (m == INP_KEYBOARD) {
        if (!read_keyboard_array(arr, n)) {
            printf("ERROR: Bad keyboard input.\n");
            return 0;
        }
        *outN = n;
        return 1;
    }

    // default: generate
    generate_dataset(arr, n, order, want_signed);
    if (!*arr) {
        printf("ERROR: Out of memory.\n");
        return 0;
    }
    *outN = n;
    return 1;
}

int ask_output_destination(char *dest, int cap) {
    // default - don't output
    printf("\nOutput? (none / stdout / filename): ");
    if (scanf("%100s", dest) != 1) return 0;
    (void)cap;
    return 1;
}

void run_one_algorithm(Algorithm alg, int *arr, int n) {
    switch (alg) {
        case ALG_BUBBLE:     bubbleSort(arr, n); break;
        case ALG_INSERTION:  insertionSort(arr, n); break;
        case ALG_QUICK:      quickSort(arr, 0, n - 1); break;
        case ALG_HEAP:       heapSort(arr, n); break;
        case ALG_RADIX_NONNEG: radix_sort2(arr, n); break;
        case ALG_RADIX_SIGNED: radix_sort(arr, n); break;
        default: break;
    }
}

const char* alg_name(Algorithm alg) {
    switch (alg) {
        case ALG_BUBBLE: return "Bubble";
        case ALG_INSERTION: return "Insertion";
        case ALG_QUICK: return "Quick";
        case ALG_HEAP: return "Heap";
        case ALG_RADIX_NONNEG: return "Radix(nonneg)";
        case ALG_RADIX_SIGNED: return "Radix(signed)";
        default: return "Unknown";
    }
}

void benchmark_and_optionally_output(Algorithm alg, int *src, int n, const char *dest) {
    int *work = (int*)malloc((size_t)n * sizeof(int));
    if (!work) { printf("ERROR: Out of memory.\n"); return; }
    memcpy(work, src, (size_t)n * sizeof(int));

    clock_t t0 = clock();
    run_one_algorithm(alg, work, n);
    clock_t t1 = clock();

    double seconds = (double)(t1 - t0) / (double)CLOCKS_PER_SEC;
    printf("%s: %.6f s\n", alg_name(alg), seconds);

    if (strcmp(dest, "none") != 0) {
        if (!writeOutput(dest, work, n)) {
            printf("ERROR: Could not write output.\n");
        }
    }

    free(work);
}

/* ============================================================
   MAIN (recurring menu workflow you requested)
   ============================================================ */

int main(void) {
    srand((unsigned)time(NULL));

    for (;;) {
        int alg_choice;
        print_algorithm_menu();
        if (!read_int_safe(&alg_choice)) continue;

        if (alg_choice == 0) break;
        if (alg_choice < 0 || alg_choice > 7) {
            printf("Invalid algorithm choice.\n");
            continue;
        }

        int size_choice;
        print_size_menu();
        if (!read_int_safe(&size_choice)) continue;

        int n = get_size_from_choice(size_choice);
        if (n == 0) {
            printf("Invalid size choice.\n");
            continue;
        }

        char order_tok[16];
        print_order_prompt();
        if (scanf("%15s", order_tok) != 1) continue;
        OrderType order = order_convertor(order_tok);

        int input_choice;
        print_input_menu();
        if (!read_int_safe(&input_choice)) continue;
        if (input_choice < 1 || input_choice > 3) input_choice = 1; // default generate

        // if user picked radix_sort2 (nonnegative), generate nonnegative by default
        int want_signed = (alg_choice == ALG_RADIX_SIGNED) ? 1 : 0;

        int *arr = NULL;
        int realN = 0;
        if (!load_array((InputMethod)input_choice, n, order, want_signed, &arr, &realN)) {
            free(arr);
            continue;
        }

        char dest[512];
        if (!ask_output_destination(dest, (int)sizeof(dest))) {
            free(arr);
            continue;
        }

        // Run selected algorithm(s)
        if ((Algorithm)alg_choice == ALG_ALL) {
            // run all on the SAME original data (we copy inside benchmark func)
            benchmark_and_optionally_output(ALG_BUBBLE, arr, realN, dest);
            benchmark_and_optionally_output(ALG_INSERTION, arr, realN, dest);
            benchmark_and_optionally_output(ALG_QUICK, arr, realN, dest);
            benchmark_and_optionally_output(ALG_HEAP, arr, realN, dest);
            benchmark_and_optionally_output(ALG_RADIX_NONNEG, arr, realN, dest);
            benchmark_and_optionally_output(ALG_RADIX_SIGNED, arr, realN, dest);
        } else {
            benchmark_and_optionally_output((Algorithm)alg_choice, arr, realN, dest);
        }

        free(arr);
    }

    printf("Bye.\n");
    return 0;
}

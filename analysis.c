#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

//            basic helpers 

void swap(int* a, int* b) {
    int temp = *a; *a = *b; *b = temp;
}

void flush_line(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int read_int_safe(int *out) {
    int x;
    if (scanf("%d", &x) != 1) {
        flush_line();
        return 0;
    }
    *out = x;
    return 1;
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

void fill_random_signed(int *a, int n, int maxabs) {
    for (int i = 0; i < n; i++) {
        int v = rand() % (maxabs + 1);
        if (rand() & 1) v = -v;
        a[i] = v;
    }
}

//              sorting algorithms

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
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

void quickSort(int arr[], int n, size_t *extra_mem)
{
    int *stack = malloc(n * sizeof(int));
    int top = -1;
    int peak = 0;  // will be used in extra_mem calc

    *extra_mem = n * sizeof(int);

    stack[++top] = 0;
    stack[++top] = n - 1;

    while (top >= 0)
    {
        int high = stack[top--];
        int low  = stack[top--];

        int pi = partition(arr, low, high);

        if (pi - 1 > low) {
            stack[++top] = low;
            stack[++top] = pi - 1;
            if (top + 1 > peak) peak = top + 1;
        }

        if (pi + 1 < high) {
            stack[++top] = pi + 1;
            stack[++top] = high;
            if (top + 1 > peak) peak = top + 1;
        }
    }

    *extra_mem = (size_t)peak * sizeof(int);

    free(stack);
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

int countingSort(int arr[], int n, size_t *extra_mem)
{
    int min = arr[0];
    int max = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    int range = max - min + 1;
        
    *extra_mem = (range + n) * sizeof(int);

    int *count = malloc(range * sizeof(int));
    int *output = malloc(n * sizeof(int));

    for (int i = 0; i < range; i++)
        count[i] = 0;

    for (int i = 0; i < n; i++)
        count[arr[i] - min]++;

    for (int i = 1; i < range; i++)
        count[i] += count[i - 1];


    for (int i = n - 1; i >= 0; i--) {
        int value = arr[i];
        output[count[value - min] - 1] = value;
        count[value - min]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];

    free(count);
    free(output);

    return 1;
}
//              datasets & order

typedef enum {
    ORDER_ASC = 1,
    ORDER_DESC,
    ORDER_RAND
} OrderType;

OrderType order_convertor(const char *s) {
    if (strcmp(s, "asc") == 0) return ORDER_ASC;
    if (strcmp(s, "desc") == 0) return ORDER_DESC;
    return ORDER_RAND;
}

int* generate_dataset(int n, OrderType order)
{   
    size_t dummy = 0;
    int *a = malloc(n * sizeof(int));

    fill_random_signed(a, n, 1000000);

    if (order != ORDER_RAND) {
        quickSort(a, n, &dummy );
        if (order == ORDER_DESC)
            reverse_array(a, n);
    }

    return a;
}

//          za menus

typedef enum {
    ALG_BUBBLE = 1,
    ALG_INSERTION,
    ALG_HEAP,
    ALG_QUICK,
    ALG_COUNTING,
    ALG_ALL
} Algorithm;

const char* alg_name(Algorithm alg) {
    switch (alg) {
        case ALG_BUBBLE: return "Bubble";
        case ALG_INSERTION: return "Insertion";
        case ALG_HEAP: return "Heap";
        case ALG_QUICK: return "Quick";
        case ALG_COUNTING: return "Counting";
    }
}

void print_algorithm_menu() {
    printf("\n=== Algorithm Menu ===\n");
    printf("1) Bubble\n");
    printf("2) Insertion\n");
    printf("3) Heap\n");
    printf("4) Quick\n");
    printf("5) Counting\n");
    printf("6) ALL (runs each one)\n");
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
    printf("Order (asc/desc/rand): ");
}

//              benchmarking 

int is_sorted(int *a, int n) {
    for (int i = 1; i < n; i++) if (a[i-1] > a[i]) return 0;
    return 1;
}

void run_one_algorithm(Algorithm alg, int *arr, int n, size_t *extra_mem)  // for counting sort (extra mem)
{
    if (alg == ALG_BUBBLE)
        bubbleSort(arr, n);
    else if (alg == ALG_INSERTION)
        insertionSort(arr, n);
    else if (alg == ALG_HEAP)
        heapSort(arr, n);
    else if (alg == ALG_QUICK)
        quickSort(arr, n, extra_mem);
    else if (alg == ALG_COUNTING)
        countingSort(arr, n, extra_mem );
}

void benchmark(Algorithm alg, int *source, int n)
{
    int *temp = malloc(n * sizeof(int));
    memcpy(temp, source, n * sizeof(int));

    size_t extra_mem = 0;

    clock_t t0 = clock();
    run_one_algorithm(alg, temp, n, &extra_mem); 
    clock_t t1 = clock();

    double seconds = (double)(t1 - t0) / CLOCKS_PER_SEC;
    size_t total_mem = 2 * n * sizeof(int) + extra_mem;

    printf("%s: %.10f s | total mem: %zu bytes | %s\n",alg_name(alg), seconds, total_mem, is_sorted(temp, n) ? "OK" : "NOT SORTED");

    free(temp);
}

//              main

int main() {
    srand(time(NULL));

    for (;;) {
        int alg_choice;
        print_algorithm_menu();
        if (!read_int_safe(&alg_choice)) continue;

        if (alg_choice == 0) break;
        if (alg_choice < 0 || alg_choice > 6) {
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

        char order_choice[16];
        print_order_prompt();
        scanf("%15s", order_choice);
        OrderType order = order_convertor(order_choice);

        int *arr = generate_dataset(n, order);

        if ((Algorithm)alg_choice == ALG_ALL) {
            benchmark(ALG_BUBBLE, arr, n);
            benchmark(ALG_INSERTION, arr, n);
            benchmark(ALG_HEAP, arr, n);
            benchmark(ALG_QUICK, arr, n);
            benchmark(ALG_COUNTING, arr, n);
        } else {
            benchmark((Algorithm)alg_choice, arr, n);
        }

        free(arr);
    }

    printf("Bye.\n");
    return 0;
}

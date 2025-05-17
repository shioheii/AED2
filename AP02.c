/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>

void selection_sort(int *arr, int n, int *comparisons, int *swaps) {
    int i, j, min_j, temp;
    
    for (i = 0; i < n - 1; i++) {
        min_j = i;
        for (j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (arr[min_j] > arr[j]) {
                min_j = j;
            }
        }
        if (i != min_j) {
            int temp = arr[i];
            arr[i] = arr[min_j];
            arr[min_j] = temp;
            (*swaps)++;
        }
    }
}

void bubble_sort(int *arr, int n, int *comparisons, int *swaps) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                (*swaps)++;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void insertion_sort(int *arr, int n, int *comparisons, int *movements) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        int swapped = 0;
        while (j >= 0) {
            (*comparisons)++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                (*movements)++;
                j--;
                swapped = 1;
            } else {
                break;
            }
        }
        if (swapped) {
            arr[j + 1] = key;
            if (j + 1 != i) (*movements)++;
        }
    }
}

int main() {
    int n, v;
    scanf("%d", &n);

    // Arrays para cada algoritmo
    int *array_selection_sort = malloc(n * sizeof(int));
    int *array_bubble_sort = malloc(n * sizeof(int));
    int *array_insertion_sort = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        scanf("%d", &v);
        array_selection_sort[i] = v;
        array_bubble_sort[i] = v;
        array_insertion_sort[i] = v;
    }

    int comp_selection_sort = 0, swap_selection_sort = 0;
    int comp_bubble_sort = 0, swap_bubble_sort = 0;
    int comp_insertion_sort = 0, mov_insertion_sort = 0;

    selection_sort(array_selection_sort, n, &comp_selection_sort, &swap_selection_sort);
    bubble_sort(array_bubble_sort, n, &comp_bubble_sort, &swap_bubble_sort);
    insertion_sort(array_insertion_sort, n, &comp_insertion_sort, &mov_insertion_sort);

    printf("%d %d\n", comp_selection_sort, swap_selection_sort);
    printf("%d %d\n", comp_bubble_sort, swap_bubble_sort);
    printf("%d %d\n", comp_insertion_sort, mov_insertion_sort);

    free(array_selection_sort);
    free(array_bubble_sort);
    free(array_insertion_sort);

    return 0;
}

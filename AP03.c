/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>

void printItems(int *arr, int n, int c) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n%d\n", c);
}

// Merge Sort
void merge(int *arr, int p, int q, int r, int *comp) {
    int i, j, k;
    int n1 = q - p + 1;
    int n2 = r - q;
    int arr1[n1 + 1], arr2[n2 + 1];
    for (i = 0; i < n1; i++) arr1[i] = arr[p + i];
    for (j = 0; j < n2; j++) arr2[j] = arr[q + j + 1];
    arr1[n1] = 100000;
    arr2[n2] = 100000;
    i = j = 0;
    for (k = p; k <= r; k++) {
        (*comp)++;
        if (arr1[i] <= arr2[j]) arr[k] = arr1[i++];
        else                    arr[k] = arr2[j++];
    }
}

void merge_sort(int *arr, int p, int r, int *comp) {
    if (p < r) {
        int q = p + (r - p) / 2;
        merge_sort(arr, p, q, comp);
        merge_sort(arr, q + 1, r, comp);
        merge(arr, p, q, r, comp);
    }
}

// Quick Sort
void swap(int *arr, int a, int b) {
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

int median_of_three(int *arr, int p, int r) {
    int mid = p + (r - p) / 2;
    int a = arr[p], b = arr[mid], c = arr[r];

    // Alterando a lógica para encontrar a mediana
    if (a > b) swap(arr, p, mid);
    if (a > c) swap(arr, p, r);
    if (b > c) swap(arr, mid, r);
    return mid;

    // Comparações para encontrar o valor intermediário
    // if ((a <= b && b <= c) || (c <= b && b <= a)) return mid;
    // if ((b <= a && a <= c) || (c <= a && a <= b)) return p;
    // return r;
}

int partition(int *arr, int p, int r, int *comp) {
    (*comp) += 3;
    int pivotIndex = median_of_three(arr, p, r);

    swap(arr, pivotIndex, r); // move o pivô para o fim
    int pivotValue = arr[r];
    int i = p - 1;
    for (int j = p; j < r; j++) {
        (*comp)++; 
        if (arr[j] <= pivotValue) swap(arr, ++i, j);
    }
    swap(arr, i + 1, r);
    return i + 1;
}

void quicksort(int *arr, int p, int r, int *comp) {
    if (p < r) {
        int q = partition(arr, p, r, comp);
        quicksort(arr, p, q - 1, comp);
        quicksort(arr, q + 1, r, comp);
    }
}

// Hybrid Quick Sort
void insertion_sort(int *arr, int n, int *comp) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        int swapped = 0;
        while (j >= 0) {
            (*comp)++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
                swapped = 1;
            } else {
                break;
            }
        }
        if (swapped) arr[j + 1] = key;
    }
}

void hybrid_quicksort(int *arr, int p, int r, int *comp) {
    int n = r - p + 1;
    if (n > 5) {
        int q = partition(arr, p, r, comp);
        hybrid_quicksort(arr, p, q - 1, comp);
        hybrid_quicksort(arr, q + 1, r, comp);
    } else {
        insertion_sort(arr + p, n, comp);
    }
}

int main() {
    int n, v;
    scanf("%d", &n);

    // Arrays para cada algoritmo
    int *array_merge_sort = malloc(n * sizeof(int));
    int *array_quick_sort = malloc(n * sizeof(int));
    int *array_hybrid_quicksort = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        scanf("%d", &v);
        array_merge_sort[i] = v;
        array_quick_sort[i] = v;
        array_hybrid_quicksort[i] = v;
    }

    int comp_merge_sort = 0;
    int comp_quick_sort = 0;
    int comp_hybrid_quicksort = 0;

    merge_sort(array_merge_sort, 0, n-1, &comp_merge_sort);
    quicksort(array_quick_sort, 0, n-1, &comp_quick_sort);
    hybrid_quicksort(array_hybrid_quicksort, 0, n-1, &comp_hybrid_quicksort);

    printItems(array_merge_sort, n, comp_merge_sort);
    printItems(array_quick_sort, n, comp_quick_sort);
    printItems(array_hybrid_quicksort, n, comp_hybrid_quicksort);

    free(array_merge_sort);
    free(array_quick_sort);
    free(array_hybrid_quicksort);

    return 0;
}

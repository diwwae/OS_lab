#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Function to merge two sorted arrays
void merge(int arr[], int left[], int right[], int leftSize, int rightSize) {
    int i = 0, j = 0, k = 0;

    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < leftSize) {
        arr[k++] = left[i++];
    }

    while (j < rightSize) {
        arr[k++] = right[j++];
    }
}

// Function to perform merge sort on an array
void mergeSort(int arr[], int size) {
    if (size <= 1) {
        return;
    }

    int mid = size / 2;
    int left[mid];
    int right[size - mid];

    for (int i = 0; i < mid; i++) {
        left[i] = arr[i];
    }

    for (int i = mid; i < size; i++) {
        right[i - mid] = arr[i];
    }

    mergeSort(left, mid);
    mergeSort(right, size - mid);
    merge(arr, left, right, mid, size - mid);
}

// Thread function to perform merge sort on a portion of the array
void* mergeSortThread(void* arg) {
    int* data = (int*)arg;
    int size = data[0];
    int* arr = &data[1];

    mergeSort(arr, size);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_threads> <array_size>\n", argv[0]);
        return 1;
    }

    int numThreads = atoi(argv[1]);
    int arraySize = atoi(argv[2]);

    if (numThreads <= 0 || arraySize <= 0) {
        printf("Invalid arguments\n");
        return 1;
    }

    int* arr = malloc(arraySize * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Print array...\n");
    for (int i = 0; i < arraySize; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Sorting array using %d threads...\n", numThreads);

    // Create an array of thread IDs
    pthread_t* threads = malloc(numThreads * sizeof(pthread_t));
    if (threads == NULL) {
        printf("Memory allocation failed\n");
        free(arr);
        return 1;
    }

    // Create threads and divide the array among them
    int portionSize = arraySize / numThreads;
    for (int i = 0; i < numThreads; i++) {
        int* data = malloc((portionSize + 1) * sizeof(int));
        if (data == NULL) {
            printf("Memory allocation failed\n");
            free(arr);
            free(threads);
            return 1;
        }

        data[0] = portionSize;
        data[1] = i * portionSize;
        data[2] = (i == numThreads - 1) ? arraySize - i * portionSize : portionSize;

        pthread_create(&threads[i], NULL, mergeSortThread, data);
    }

    // Wait for all threads to finish
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Merge the sorted portions
    for (int i = 1; i < numThreads; i++) {
        merge(arr, arr, arr + i * portionSize, i * portionSize, portionSize);
    }

    printf("Array sorted successfully\n");

    // Print the sorted array
    printf("Sorted array: ");
    for (int i = 0; i < arraySize; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    free(threads);

    return 0;
}
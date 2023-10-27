#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Structure for passing arguments to the merge sort function
typedef struct {
    int* arr;
    int left;
    int right;
} MergeSortArgs;

// Function to merge two sorted subarrays
void merge(int* arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Free temporary arrays
    free(L);
    free(R);
}

// Function to perform merge sort on a subarray
void* mergeSort(void* arg) {
    MergeSortArgs* args = (MergeSortArgs*)arg;
    int* arr = args->arr;
    int left = args->left;
    int right = args->right;

    if (left < right) {
        int mid = left + (right - left) / 2;

        // Create arguments for left and right subarrays
        MergeSortArgs leftArgs = { arr, left, mid };
        MergeSortArgs rightArgs = { arr, mid + 1, right };

        // Create threads for left and right subarrays
        pthread_t leftThread, rightThread;
        pthread_create(&leftThread, NULL, mergeSort, &leftArgs);
        pthread_create(&rightThread, NULL, mergeSort, &rightArgs);

        // Wait for the threads to finish
        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);

        // Merge the sorted subarrays
        merge(arr, left, mid, right);
    }

    pthread_exit(NULL);
}

// Function to perform parallel merge sort
void parallelMergeSort(int* arr, int size, int numThreads) {
    // Create arguments for the initial merge sort call
    MergeSortArgs args = { arr, 0, size - 1 };

    // Create threads for the initial merge sort call
    pthread_t threads[numThreads];
    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threads[i], NULL, mergeSort, &args);
    }

    // Wait for the threads to finish
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    // int arr[] = { 9, 4, 2, 7, 5, 1, 8, 3, 6 };
    int numThreads;
    printf("Print count of threads:");
    scanf("%d", &numThreads);
    printf("Print count of elements:");
    
    int count_of_elements;
    scanf("%d", &count_of_elements);

    int arr[count_of_elements];
    for (int i=0; i<count_of_elements; i++){
        // scanf("%d", &arr[i]);
        arr[i] = rand();
    }

    int size = sizeof(arr) / sizeof(arr[0]);

    struct timeval start, end;
    gettimeofday(&start, NULL);   

    parallelMergeSort(arr, size, numThreads);

    gettimeofday(&end, NULL);
    double execution_time = 
            (end.tv_sec - start.tv_sec)*1000.0;
    execution_time+=
            (end.tv_usec - start.tv_usec)/1000.0;

    printf("Execution_time: %.20f ms\n ", execution_time);

    // printf("Sorted array: ");
    // for (int i = 0; i < size; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    return 0;
}
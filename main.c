#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 4

typedef struct {
    int* array;
    int left;
    int right;
} QuickSortArgs;

typedef struct {
    pthread_t thread;
    QuickSortArgs* args;
} ThreadPoolTask;

typedef struct {
    ThreadPoolTask* tasks;
    int numTasks;
    int nextTaskIndex;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ThreadPool;

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void* quickSort(void* args) {
    QuickSortArgs* qsArgs = (QuickSortArgs*)args;
    int low = qsArgs->left;
    int high = qsArgs->right;

    if (low < high) {
        int pi = partition(qsArgs->array, low, high);

        QuickSortArgs qsArgsLeft = {qsArgs->array, low, pi - 1};
        QuickSortArgs qsArgsRight = {qsArgs->array, pi + 1, high};

        ThreadPool* threadPool = (ThreadPool*)qsArgs->array;
        pthread_mutex_lock(&threadPool->mutex);
        while (threadPool->nextTaskIndex == threadPool->numTasks) {
            pthread_cond_wait(&threadPool->cond, &threadPool->mutex);
        }
        int taskIndex = threadPool->nextTaskIndex++;
        pthread_mutex_unlock(&threadPool->mutex);

        ThreadPoolTask* task = &threadPool->tasks[taskIndex];
        task->args = &qsArgsLeft;
        pthread_create(&task->thread, NULL, quickSort, task->args);

        quickSort(&qsArgsRight);

        pthread_join(task->thread, NULL);

        pthread_mutex_lock(&threadPool->mutex);
        pthread_cond_signal(&threadPool->cond);
        pthread_mutex_unlock(&threadPool->mutex);
    }
    return NULL;
}

int main() {
    int arr[256];

    int n;
    printf("Введите количество элементов массива:");
    scanf("%d", &n);
    printf("Введите элементы массива:");
    for (int i=0; i<n; i++){
        scanf("%d", &arr[i]);
    }

    QuickSortArgs qsArgs = {arr, 0, n - 1};

    ThreadPoolTask tasks[MAX_THREADS];
    ThreadPool threadPool = {tasks, MAX_THREADS, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

    for (int i = 0; i < MAX_THREADS; i++) {
        tasks[i].args = &qsArgs;
        pthread_create(&tasks[i].thread, NULL, quickSort, tasks[i].args);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(tasks[i].thread, NULL);
    }

    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);

    return 0;
}
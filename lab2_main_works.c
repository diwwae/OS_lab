#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

struct ThreadArgs {
    int* array;
    int start;
    int end;
};

void Quicksort(int* InputArr, int StartIndex, int EndIndex) {
    if (StartIndex < EndIndex) {
        int pivot = InputArr[EndIndex];
        int i = StartIndex - 1;

        for (int j = StartIndex; j <= EndIndex - 1; j++) {
            if (InputArr[j] <= pivot) {
                i++;
                int temp = InputArr[i];
                InputArr[i] = InputArr[j];
                InputArr[j] = temp;
            }
        }

        int temp = InputArr[i + 1];
        InputArr[i + 1] = InputArr[EndIndex];
        InputArr[EndIndex] = temp;

        int partition_index = i + 1;

        Quicksort(InputArr, StartIndex, partition_index - 1);
        Quicksort(InputArr, partition_index + 1, EndIndex);
    }
}

void* thread_function(void* arg) {
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    Quicksort(args->array, args->start, args->end);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Invalid starting parameters, program was not launched\n");
        return 1;
    }
    int threadsAmount = atoi(argv[1]);
    printf("Amount_of_threads is %d\n", threadsAmount);
    int ArrayForSort[50];
    int arraySize = sizeof(ArrayForSort) / sizeof(ArrayForSort[0]);
    
    printf("Array for sort:");

    for (int i = 0; i < arraySize; i++){
        ArrayForSort[i] = 50000 - i;
        printf("%d ",ArrayForSort[i]); 
    }
    printf("\n");

    struct timeval start, end;
    gettimeofday(&start, NULL);    

    pthread_t threads[threadsAmount];
    struct ThreadArgs thread_args[threadsAmount];

    int chunkSize = arraySize / threadsAmount;
    int remainder = arraySize % threadsAmount;

    int startIndex = 0;
    int endIndex = chunkSize - 1;

    for (int iteration = 0; iteration < threadsAmount; iteration++){
        if (remainder > 0){
            endIndex++;
            remainder--;
        }

        thread_args[iteration].array = ArrayForSort;
        thread_args[iteration].start = startIndex;
        thread_args[iteration].end = endIndex;

        pthread_create(&threads[iteration], NULL, thread_function, (void*)&thread_args[iteration]);

        startIndex = endIndex + 1;
        endIndex = startIndex + chunkSize - 1;
    }

    for (int i = 0; i < threadsAmount; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    double execution_time = 
            (end.tv_sec - start.tv_sec)*1000.0;
    execution_time+=
            (end.tv_usec - start.tv_usec)/1000.0;

    printf("Execution_time: %.20f ms\n ", execution_time);


    Quicksort(ArrayForSort,0, arraySize -1);

    printf("Sorted array: ");

    for (int iteration = 0; iteration < arraySize; iteration++){
        printf("%d, ", ArrayForSort[iteration]);
    }
    printf("\n");




    return 0;
}
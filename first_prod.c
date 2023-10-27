#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* array;
    int left;
    int right;
} QuickSortArgs;

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
        pthread_t pthreadLeft;
        pthread_create(&pthreadLeft, NULL, quickSort, &qsArgsLeft);

        QuickSortArgs qsArgsRight = {qsArgs->array, pi + 1, high};
        pthread_t pthreadRight;
        pthread_create(&pthreadRight, NULL, quickSort, &qsArgsRight);

        pthread_join(pthreadLeft, NULL);
        pthread_join(pthreadRight, NULL);
    }
    return NULL;
}

int main() {
    // int arr[] = {10, 7, 8, 9, 1, 5};
    int arr[256];

    int n;
    printf("Введите количество элементов массива:");
    scanf("%d", &n);
    printf("Введите элементы массива:");
    for (int i=0; i<n; i++){
        scanf("%d", &arr[i]);
    }

    // int n = sizeof(arr) / sizeof(arr[0]);

    QuickSortArgs qsArgs = {arr, 0, n - 1};
    pthread_t pthread;
    pthread_create(&pthread, NULL, quickSort, &qsArgs);
    pthread_join(pthread, NULL);

    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);

    return 0;
}


/* 
#include <pthread.h> // Подключение библиотеки для работы с потоками
#include <stdio.h> // Подключение библиотеки для работы с вводом/выводом
#include <stdlib.h> // Подключение библиотеки для работы с памятью и преобразованием типов

typedef struct { // Определение структуры для передачи аргументов в функцию сортировки
    int* array; // Указатель на массив
    int left; // Индекс начала сортировки
    int right; // Индекс конца сортировки
} QuickSortArgs;

void swap(int* a, int* b) { // Функция для обмена значениями двух переменных
    int t = *a; // Сохранение значения a
    *a = *b; // Присваивание a значения b
    *b = t; // Присваивание b сохраненного значения a
}

int partition(int arr[], int low, int high) { // Функция для разделения массива на две части
    int pivot = arr[high]; // Выбор опорного элемента
    int i = (low - 1); // Индекс меньшего элемента

    for (int j = low; j <= high - 1; j++) { // Проход по массиву
        if (arr[j] < pivot) { // Если текущий элемент меньше опорного
            i++; // Увеличение индекса меньшего элемента
            swap(&arr[i], &arr[j]); // Обмен местами элементов
        }
    }
    swap(&arr[i + 1], &arr[high]); // Перемещение опорного элемента на правильное место
    return (i + 1); // Возвращение индекса опорного элемента
}

void* quickSort(void* args) { // Функция быстрой сортировки
    QuickSortArgs* qsArgs = (QuickSortArgs*)args; // Приведение типа аргументов к нужному
    int low = qsArgs->left; // Получение начала сортировки
    int high = qsArgs->right; // Получение конца сортировки

    if (low < high) { // Если есть что сортировать
        int pi = partition(qsArgs->array, low, high); // Разделение массива

        QuickSortArgs qsArgsLeft = {qsArgs->array, low, pi - 1}; // Аргументы для левой части
        pthread_t pthreadLeft; // Создание идентификатора потока
        pthread_create(&pthreadLeft, NULL, quickSort, &qsArgsLeft); // Создание потока для сортировки левой части

        QuickSortArgs qsArgsRight = {qsArgs->array, pi + 1, high}; // Аргументы для правой части
        pthread_t pthreadRight; // Создание идентификатора потока
        pthread_create(&pthreadRight, NULL, quickSort, &qsArgsRight); // Создание потока для сортировки правой части

        pthread_join(pthreadLeft, NULL); // Ожидание завершения потока для левой части
        pthread_join(pthreadRight, NULL); // Ожидание завершения потока для правой части
    }
    return NULL; // Возвращение NULL, так как функция должна что-то вернуть
}

int main() { // Главная функция
    int arr[] = {10, 7, 8, 9, 1, 5}; // Исходный массив
    int n = sizeof(arr) / sizeof(arr[0]); // Размер массива .... ЧУШЬ КАКАЯТО ИЗМЕНИТЬ

    QuickSortArgs qsArgs = {arr, 0, n - 1}; // Аргументы для сортировки
    pthread_t pthread; // Создание идентификатора потока
    pthread_create(&pthread, NULL, quickSort, &qsArgs); // Создание потока для сортировки
    pthread_join(pthread, NULL); // Ожидание завершения потока

    for (int i = 0; i < n; i++) // Вывод отсортированного массива
        printf("%d ", arr[i]);

    return 0; // Возвращение 0, что означает успешное завершение программы
} 
*/
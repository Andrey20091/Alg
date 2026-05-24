#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


// Печатает массив целых чисел в одну строку через пробел
void printSimple(int arr[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
}

// Печатает массив с выделением отсортированной зоны [leftBound..rightBound] без скобок,
// а неотсортированные части — в квадратных скобках
void printSortedZones(int arr[], int n, int leftBound, int rightBound) {
    if (leftBound > rightBound) {
        printf("[ ");
        for (int i = 0; i < n; i++) printf("%d ", arr[i]);
        printf("]\n");
        return;
    }

    if (leftBound > 0) {
        printf("[ ");
        for (int i = 0; i < leftBound; i++) printf("%d ", arr[i]);
        printf("] ");
    }

    for (int i = leftBound; i <= rightBound; i++) {
        printf("%d ", arr[i]);
    }

    if (rightBound < n - 1) {
        printf("[ ");
        for (int i = rightBound + 1; i < n; i++) printf("%d ", arr[i]);
        printf("] ");
    }
    printf("\n");
}

// Печатает массив, выделяя скобками два сравниваемых индекса, и статус шага (ок / ОБМЕН и т.д.)
void printStep(int arr[], int n, int idx1, int idx2, const char* status) {
    for (int i = 0; i < n; i++) {
        if (i == idx1 || i == idx2) printf("(%d) ", arr[i]);
        else printf("%d ", arr[i]);
    }
    printf(" | %s\n", status);
}

// Печатает массив при сортировке вставками, выделяя индекс сравниваемого элемента
void printInsertionStep(int arr[], int n, int compare_idx) {
    for (int i = 0; i < n; i++) {
        if (i == compare_idx) printf("(%d) ", arr[i]);
        else printf("%d ", arr[i]);
    }
}

// Меняет местами значения двух целых переменных по указателям
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


// Сортирует массив пузырьковым методом с уменьшением активной зоны и пошаговым выводом
void bubbleSort(int arr[], int n) {
    printf("\n=== ПУЗЫРЬКОВАЯ СОРТИРОВКА ===\nИзначально: ");
    printSimple(arr, n); printf("\n");

    int currentRightBound = n - 1;
    int pass = 1;
    int wasSwapped = 1;


    while (wasSwapped) {
        int lastExchangeIndex = 0;
        wasSwapped = 0;

        printf("\nПроход %d (активная зона до индекса %d):\n", pass++, currentRightBound);


        for (int j = 0; j < currentRightBound; j++) {
            if (arr[j] > arr[j + 1]) {
                printStep(arr, n, j, j + 1, "ОБМЕН");
                swap(&arr[j], &arr[j + 1]);

                wasSwapped = 1;
                lastExchangeIndex = j + 1;
            }
            else {
                printStep(arr, n, j, j + 1, "ок");
            }
        }

        printf("Итог прохода: ");


        if (!wasSwapped) {
            printf("Обменов нет, всё отсортировано. ");
            printSortedZones(arr, n, n, 0);
            break;
        }

        currentRightBound = lastExchangeIndex - 1;
        printSortedZones(arr, n, 0, currentRightBound);
    }

    printf("ФИНАЛ: "); printSortedZones(arr, n, n, 0);
}

// Сортирует массив методом выбора минимума с пошаговой визуализацией
void selectionSort(int arr[], int n) {
    printf("\n=== СОРТИРОВКА ВЫБОРОМ ===\nИзначально: ");
    printSimple(arr, n); printf("\n");
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        printf("\nШаг %d:\n", i + 1);
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                printStep(arr, n, min_idx, j, "новый минимум");
                min_idx = j;
            }
            else printStep(arr, n, min_idx, j, "ок");
        }
        if (min_idx != i) {
            printStep(arr, n, i, min_idx, "ОБМЕН");
            swap(&arr[i], &arr[min_idx]);
        }
        printf("Итог шага: "); printSortedZones(arr, n, i + 1, n - 1);
    }
    printf("ФИНАЛ: "); printSortedZones(arr, n, n, 0);
}

// Сортирует массив прямой сортировкой вставками с пошаговым выводом сдвигов
void insertionSort(int arr[], int n) {
    printf("\n=== СОРТИРОВКА ВСТАВКАМИ ===\nИзначально: ");
    printSimple(arr, n); printf("\n");
    for (int i = 1; i < n; i++) {
        int val = arr[i];
        int j = i - 1;
        printf("\n--- ШАГ %d: Берем элемент %d ---\n", i, val);
        while (j >= 0) {
            printf("  "); printInsertionStep(arr, n, j);
            if (arr[j] > val) {
                printf(" | %d > %d -> Сдвиг\n", arr[j], val);
                arr[j + 1] = arr[j];
                j--;
            }
            else {
                printf(" | %d <= %d -> Остановка\n", arr[j], val);
                break;
            }
        }
        arr[j + 1] = val;
        printf("Итог шага: "); printSortedZones(arr, n, i + 1, n - 1);
    }
    printf("ФИНАЛ: "); printSortedZones(arr, n, n, 0);
}

// Копирует n элементов из source в dest
void copyArray(int source[], int dest[], int n) {
    for (int i = 0; i < n; i++) dest[i] = source[i];
}

// Запрашивает массив и последовательно демонстрирует три метода сортировки на копиях
int main() {
    setlocale(LC_ALL, "Rus");
    int n;
    printf("Введите количество элементов: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 1;
    int* originalArr = (int*)malloc(n * sizeof(int));
    int* tempArr = (int*)malloc(n * sizeof(int));

    printf("Введите элементы: ");
    for (int i = 0; i < n; i++) scanf("%d", &originalArr[i]);

    copyArray(originalArr, tempArr, n); bubbleSort(tempArr, n);
    printf("\n**************************************\n");
    copyArray(originalArr, tempArr, n); selectionSort(tempArr, n);
    printf("\n**************************************\n");
    copyArray(originalArr, tempArr, n); insertionSort(tempArr, n);

    free(originalArr); free(tempArr);
    return 0;
}

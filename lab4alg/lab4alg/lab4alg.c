#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

// Читает из файла все целые числа и печатает их в одну строку (или «пусто»)
void printFileSimple(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\nОшибка: Не удалось открыть файл %s для чтения\n", filename);
        return;
    }
    int val;
    bool hasData = false;
    while (fscanf(file, "%d", &val) == 1) {
        printf("%d ", val);
        hasData = true;
    }
    if (!hasData) printf("пусто");
    fclose(file);
}

// Печатает содержимое файла, группируя числа в серии фиксированной длины seriesLength
void printFileSeries(const char* filename, int seriesLength) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\nОшибка: Не удалось открыть файл %s для вывода серий\n", filename);
        return;
    }
    int val;
    int count = 0;
    bool hasData = false;
    while (fscanf(file, "%d", &val) == 1) {
        if (count % seriesLength == 0) printf("[ ");
        printf("%d ", val);
        count++;
        if (count % seriesLength == 0) printf("] ");
        hasData = true;
    }
    if (count % seriesLength != 0) printf("] ");
    if (!hasData) printf("пусто");
    fclose(file);
}

// Печатает числа из файла, разбивая их на естественные неубывающие серии (границы в скобках)
void printFileNaturalSeries(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\nОшибка: Не удалось открыть файл %s для вывода естественных серий\n", filename);
        return;
    }
    int val, prev;
    if (fscanf(file, "%d", &prev) != 1) {
        printf("пусто");
        fclose(file);
        return;
    }
    printf("[ %d ", prev);
    while (fscanf(file, "%d", &val) == 1) {
        if (val < prev) printf("] [ ");
        printf("%d ", val);
        prev = val;
    }
    printf("] ");
    fclose(file);
}

// Сортирует файл f.txt прямым слиянием: на каждой итерации делит серии длины seriesLength
// между g.txt и t.txt, затем сливает обратно в f.txt, удваивая длину серии
void directMergeSort(const char* f_name) {
    const char* g_name = "g.txt";
    const char* t_name = "t.txt";
    int seriesLength = 1;
    int iteration = 1;

    while (true) {
        printf("\n==========================================\n");
        printf("ИТЕРАЦИЯ №%d (Длина серии: %d)\n", iteration, seriesLength);
        printf("==========================================\n");

        printf("Файл f.txt перед проходом: ");
        printFileSeries(f_name, seriesLength);
        printf("\n");

        FILE* f = fopen(f_name, "r");
        if (!f) { printf("\nОшибка открытия f\n"); return; }
        FILE* g = fopen(g_name, "w");
        if (!g) { fclose(f); return; }
        FILE* t = fopen(t_name, "w");
        if (!t) { fclose(f); fclose(g); return; }

        int val, totalElements = 0;
        int currentInSeries = 0;
        bool writeToG = true;

        while (fscanf(f, "%d", &val) == 1) {
            if (writeToG) fprintf(g, "%d ", val);
            else fprintf(t, "%d ", val);
            totalElements++;
            currentInSeries++;
            if (currentInSeries == seriesLength) {
                currentInSeries = 0;
                writeToG = !writeToG;
            }
        }
        fclose(f); fclose(g); fclose(t);

        printf("\n>>> ФАЗА РАЗДЕЛЕНИЯ (разливаем по сериям):\n");
        printf("Файл g.txt: "); printFileSeries(g_name, seriesLength); printf("\n");
        printf("Файл t.txt: "); printFileSeries(t_name, seriesLength); printf("\n");

        if (totalElements <= seriesLength) {
            printf("\n--- СОРТИРОВКА ЗАВЕРШЕНА ---\n");
            break;
        }

        f = fopen(f_name, "w");
        g = fopen(g_name, "r");
        t = fopen(t_name, "r");
        if (!f || !g || !t) { printf("\nОшибка при слиянии\n"); return; }

        int valG, valT;
        bool hasG = (fscanf(g, "%d", &valG) == 1);
        bool hasT = (fscanf(t, "%d", &valT) == 1);

        printf("\n<<< ФАЗА СЛИЯНИЯ (результат в f.txt):\n");
        while (hasG || hasT) {
            int leftG = seriesLength, leftT = seriesLength;
            while (leftG > 0 && hasG && leftT > 0 && hasT) {
                if (valG <= valT) {
                    fprintf(f, "%d ", valG);
                    hasG = (fscanf(g, "%d", &valG) == 1);
                    leftG--;
                }
                else {
                    fprintf(f, "%d ", valT);
                    hasT = (fscanf(t, "%d", &valT) == 1);
                    leftT--;
                }
            }
            while (leftG > 0 && hasG) {
                fprintf(f, "%d ", valG);
                hasG = (fscanf(g, "%d", &valG) == 1);
                leftG--;
            }
            while (leftT > 0 && hasT) {
                fprintf(f, "%d ", valT);
                hasT = (fscanf(t, "%d", &valT) == 1);
                leftT--;
            }
        }
        fclose(f); fclose(g); fclose(t);

        printf("Текущее состояние f.txt: ");
        printFileSimple(f_name);
        printf("\n");
        seriesLength *= 2;
        iteration++;
    }
}

// Добавляет длину серии в динамический массив
static void pushSeriesLen(int** lens, int* cap, int* count, int len) {
    if (*count >= *cap) {
        int newCap = (*cap == 0) ? 16 : *cap * 2;
        int* p = (int*)realloc(*lens, (size_t)newCap * sizeof(int));
        if (!p) {
            printf("\nОшибка выделения памяти\n");
            exit(1);
        }
        *lens = p;
        *cap = newCap;
    }
    (*lens)[(*count)++] = len;
}

// Сортирует файл f.txt естественным слиянием: серии — неубывающие отрезки во входном файле,
// чередует их между g.txt и t.txt, затем сливает попарно обратно в f.txt
void naturalMergeSort(const char* f_name) {
    const char* g_name = "g.txt";
    const char* t_name = "t.txt";
    int iteration = 1;

    while (true) {
        printf("\n==========================================\n");
        printf("ИТЕРАЦИЯ №%d (Естественное слияние)\n", iteration);
        printf("==========================================\n");

        printf("Файл f.txt перед проходом: ");
        printFileNaturalSeries(f_name);
        printf("\n");

        FILE* f = fopen(f_name, "r");
        if (!f) { printf("\nОшибка открытия f\n"); return; }
        FILE* g = fopen(g_name, "w");
        if (!g) { fclose(f); return; }
        FILE* t = fopen(t_name, "w");
        if (!t) { fclose(f); fclose(g); return; }

        int val, prev;
        int seriesCount = 0;
        bool writeToG = true;
        int curLen = 0;
        int* gLens = NULL;
        int* tLens = NULL;
        int gCap = 0, tCap = 0, gCount = 0, tCount = 0;

        if (fscanf(f, "%d", &prev) == 1) {
            seriesCount = 1;
            curLen = 1;
            fprintf(g, "%d ", prev);
            while (fscanf(f, "%d", &val) == 1) {
                if (val < prev) {
                    if (writeToG) pushSeriesLen(&gLens, &gCap, &gCount, curLen);
                    else pushSeriesLen(&tLens, &tCap, &tCount, curLen);
                    writeToG = !writeToG;
                    seriesCount++;
                    curLen = 1;
                }
                else {
                    curLen++;
                }
                if (writeToG) fprintf(g, "%d ", val);
                else fprintf(t, "%d ", val);
                prev = val;
            }
            if (writeToG) pushSeriesLen(&gLens, &gCap, &gCount, curLen);
            else pushSeriesLen(&tLens, &tCap, &tCount, curLen);
        }
        fclose(f); fclose(g); fclose(t);

        printf("\n>>> ФАЗА РАЗДЕЛЕНИЯ (по естественным сериям):\n");
        printf("Файл g.txt: "); printFileNaturalSeries(g_name); printf("\n");
        printf("Файл t.txt: "); printFileNaturalSeries(t_name); printf("\n");

        if (seriesCount <= 1) {
            free(gLens);
            free(tLens);
            printf("\n--- СОРТИРОВКА ЗАВЕРШЕНА (одна серия) ---\n");
            break;
        }

        f = fopen(f_name, "w");
        g = fopen(g_name, "r");
        t = fopen(t_name, "r");
        if (!f || !g || !t) {
            free(gLens);
            free(tLens);
            printf("\nОшибка при слиянии\n");
            return;
        }

        int valG, valT;
        bool hasG = (fscanf(g, "%d", &valG) == 1);
        bool hasT = (fscanf(t, "%d", &valT) == 1);
        int gIdx = 0, tIdx = 0;

        printf("\n<<< ФАЗА СЛИЯНИЯ (результат в f.txt):\n");
        while (hasG || hasT) {
            if (gIdx < gCount && tIdx < tCount) {
                int leftG = gLens[gIdx++];
                int leftT = tLens[tIdx++];
                while (leftG > 0 && hasG && leftT > 0 && hasT) {
                    if (valG <= valT) {
                        fprintf(f, "%d ", valG);
                        hasG = (fscanf(g, "%d", &valG) == 1);
                        leftG--;
                    }
                    else {
                        fprintf(f, "%d ", valT);
                        hasT = (fscanf(t, "%d", &valT) == 1);
                        leftT--;
                    }
                }
                while (leftG > 0 && hasG) {
                    fprintf(f, "%d ", valG);
                    hasG = (fscanf(g, "%d", &valG) == 1);
                    leftG--;
                }
                while (leftT > 0 && hasT) {
                    fprintf(f, "%d ", valT);
                    hasT = (fscanf(t, "%d", &valT) == 1);
                    leftT--;
                }
            }
            else if (gIdx < gCount) {
                int leftG = gLens[gIdx++];
                while (leftG > 0 && hasG) {
                    fprintf(f, "%d ", valG);
                    hasG = (fscanf(g, "%d", &valG) == 1);
                    leftG--;
                }
            }
            else if (tIdx < tCount) {
                int leftT = tLens[tIdx++];
                while (leftT > 0 && hasT) {
                    fprintf(f, "%d ", valT);
                    hasT = (fscanf(t, "%d", &valT) == 1);
                    leftT--;
                }
            }
            else {
                break;
            }
        }
        fclose(f); fclose(g); fclose(t);
        free(gLens);
        free(tLens);

        printf("Текущее состояние f.txt: ");
        printFileSimple(f_name);
        printf("\n");
        iteration++;
    }
}

// Запрашивает метод сортировки и запускает прямое или естественное слияние для f.txt
int main() {
    setlocale(LC_ALL, "Rus");
    const char* filename = "f.txt";

    FILE* check = fopen(filename, "r");
    if (!check) {
        printf("\nОшибка: Файл %s не найден\n", filename);
        return 1;
    }
    fclose(check);

    int choice;
    printf("Выберите метод сортировки:\n1 - Прямое слияние\n2 - Естественное слияние\nВаш выбор: ");
    if (scanf("%d", &choice) != 1) return 0;

    printf("\nИСХОДНЫЕ ЭЛЕМЕНТЫ: ");
    printFileSimple(filename);
    printf("\n");

    if (choice == 1) directMergeSort(filename);
    else if (choice == 2) naturalMergeSort(filename);
    else printf("Неверный выбор\n");

    printf("\nИТОГОВЫЙ РЕЗУЛЬТАТ: ");
    printFileSimple(filename);
    printf("\n");

    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Настраивает консоль Windows на кодировку CP1251 и русскую локаль
void setConsole() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
}

// Создаёт узел односвязного списка с заданным целым значением
Node* createNode(int value) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    n->data = value;
    n->next = NULL;
    return n;
}

// Выводит все элементы списка; текущий узел (указатель) выделяется в квадратных скобках
void printList(Node* head, Node* current) {
    if (!head) return;
    printf("Текущий список: ");
    Node* t = head;
    while (t) {
        if (t == current) printf("[%d] ", t->data);
        else printf("%d ", t->data);
        t = t->next;
    }
    printf("\n");
}

// Инициализирует список одним узлом со значением 0 и устанавливает на него рабочий указатель
void opInit(Node** head, Node** current) {
    if (*head != NULL) {
        printf("Список уже инициализирован.\n");
        return;
    }
    *head = createNode(0);
    *current = *head;
    printf("Работа начата. Список инициализирован значением 0.\n");
}

// Удаляет все узлы кроме головного и сбрасывает голову в 0; указатель — на начало
void opClear(Node** head, Node** current) {
    Node* t = (*head)->next;
    while (t) {
        Node* tmp = t;
        t = t->next;
        free(tmp);
    }
    (*head)->data = 0;
    (*head)->next = NULL;
    *current = *head;
    printf("Список очищен до начального состояния (0).\n");
}

// Сообщает, пуст ли список (только начальный 0 без хвоста)
void opCheckEmpty(Node* head) {
    if (head->next == NULL && head->data == 0) printf("Список пуст (содержит только начальный 0)\n");
    else printf("Список не пуст\n");
}

// Переносит рабочий указатель на первый (головной) узел списка
void opSetBegin(Node* head, Node** current) {
    *current = head;
    printf("Указатель переведен в начало.\n");
}

// Проверяет, находится ли рабочий указатель на последнем узле списка
void opCheckEnd(Node* current) {
    if (current->next == NULL) printf("Указатель в конце списка.\n");
    else printf("Указатель НЕ в конце.\n");
}

// Сдвигает рабочий указатель на один узел вперёд, если это возможно
void opMoveNext(Node** current) {
    if ((*current)->next) {
        *current = (*current)->next;
        printf("Указатель сдвинут.\n");
    }
    else printf("Сдвиг невозможен (конец списка).\n");
}

// Показывает значение узла, следующего за рабочим указателем
void opShowNext(Node* current) {
    if (current->next) printf("За указателем: %d\n", current->next->data);
    else printf("За указателем нет элемента.\n");
}

// Удаляет узел, следующий за рабочим указателем, и освобождает память
void opDeleteNext(Node* current) {
    if (current->next) {
        Node* tmp = current->next;
        current->next = tmp->next;
        free(tmp);
        printf("Элемент удален.\n");
    }
    else printf("Удаление невозможно.\n");
}

// Извлекает значение узла за указателем, удаляет этот узел; возвращает 1 при успехе, 0 если узла нет
int opTakeNext(Node* current, int* result) {
    if (current->next) {
        Node* tmp = current->next;
        *result = tmp->data;
        current->next = tmp->next;
        free(tmp);
        return 1;
    }
    return 0;
}

// Запрашивает у пользователя новое значение и записывает его в узел за указателем
void opChangeNext(Node* current) {
    if (current->next) {
        int value;
        printf("Введите новое значение: ");
        scanf("%d", &value);
        current->next->data = value;
        printf("Значение изменено.\n");
    }
    else printf("Изменение невозможно.\n");
}

// Добавляет новый узел за рабочим указателем (или заменяет начальный 0, если список «пуст»)
void opAddNext(Node* head, Node** current) {
    int value;
    printf("Введите значение: ");
    scanf("%d", &value);
    if (head->next == NULL && head->data == 0) {
        head->data = value;
        printf("Начальный 0 заменен на %d\n", value);
    }
    else {
        Node* n = createNode(value);
        n->next = (*current)->next;
        (*current)->next = n;
        printf("Элемент добавлен.\n");
    }
}

// Полностью уничтожает список, освобождает память и обнуляет указатели head и current
void opDestroy(Node** head, Node** current) {
    if (!*head) return;
    Node* t = *head;
    while (t) {
        Node* tmp = t;
        t = t->next;
        free(tmp);
    }
    *head = NULL;
    *current = NULL;
    printf("Работа со списком завершена. Память освобождена.\n");
}

// Главный цикл: меню операций над односвязным списком с рабочим указателем
int main() {
    setConsole();
    Node* head = NULL;
    Node* current = NULL;
    int choice;
    int takenValue;

    while (1) {
        printf("\n--- МЕНЮ ---\n");
        printf("1. Начать работу\n");
        printf("2. Сделать список пустым\n");
        printf("3. Проверить (пуст/не пуст)\n");
        printf("4. Установить рабочий указатель в начало\n");
        printf("5. Проверка: рабочий указатель в конце\n");
        printf("6. Передвинуть рабочий указатель на 1 вперёд\n");
        printf("7. Показать значение элемента за указателем\n");
        printf("8. Удалить элемент за указателем\n");
        printf("9. Взять элемент за указателем\n");
        printf("10. Изменить значение элемента за указателем\n");
        printf("11. Добавить элемент за указателем\n");
        printf("12. Закончить работу со списком\n");
        printf("13. Закончить работу программы\n");
        printf("Выбор: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (head == NULL && choice != 1 && choice != 13) {
            printf("Ошибка: Список не инициализирован! Нажмите 1.\n");
            continue;
        }

        switch (choice) {
        case 1: opInit(&head, &current); break;
        case 2: opClear(&head, &current); break;
        case 3: opCheckEmpty(head); break;
        case 4: opSetBegin(head, &current); break;
        case 5: opCheckEnd(current); break;
        case 6: opMoveNext(&current); break;
        case 7: opShowNext(current); break;
        case 8: opDeleteNext(current); break;
        case 9:
            if (opTakeNext(current, &takenValue)) {
                printf("Значение %d успешно извлечено!\n", takenValue);
            }
            else {
                printf("Нечего извлекать.\n");
            }
            break;
        case 10: opChangeNext(current); break;
        case 11: opAddNext(head, &current); break;
        case 12: opDestroy(&head, &current); break;
        case 13: opDestroy(&head, &current); printf("Программа завершена.\n"); return 0;
        default: printf("Неверный пункт меню.\n");
        }
        printList(head, current);
    }
    return 0;
}

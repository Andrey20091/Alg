#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Структура узла двусвязного списка
struct Node {
    char data;
    struct Node* next;
    struct Node* prev;
};

// Выделяет память и создаёт новый узел списка с заданным символом
struct Node* CreateNode(char ch) {
    struct Node* NewNode = (struct Node*)malloc(sizeof(struct Node));
    if (NewNode == NULL) {
        printf("ERROR with memory");
        exit(EXIT_FAILURE);
    }
    NewNode->data = ch;
    NewNode->next = NULL;
    NewNode->prev = NULL;
    return NewNode;
}

// Добавляет символ в конец двусвязного списка (создаёт узел через CreateNode)
void AppendToList(struct Node** head, char ch) {
    struct Node* NewNode = CreateNode(ch);
    if (*head == NULL) {
        *head = NewNode;
    }
    else {
        struct Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        NewNode->prev = current;
        current->next = NewNode;
    }
}

// Обрабатывает исходный список: сжимает подряд идущие точки в одну,
// после букв 'a'/'A' вставляет запятую; результат записывает в res
void Process(struct Node* origin, struct Node** res) {
    struct Node* current = origin;
    while (current) {
        char ch = current->data;

        if (ch == '.') {
            AppendToList(res, ch);
            while (current->next && current->next->data == '.') {
                current = current->next;
            }
        }
        else if (ch == 'a' || ch == 'A') {
            AppendToList(res, ch);
            AppendToList(res, ',');
        }
        else {
            AppendToList(res, ch);
        }
        current = current->next;
    }
}

// Печатает все символы списка подряд и перевод строки
void Output(struct Node* head) {
    while (head) {
        printf("%c", head->data);
        head = head->next;
    }
    printf("\n");
}

// Освобождает память всех узлов списка
void Clear(struct Node* head) {
    struct Node* tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

// Читает строку с клавиатуры, строит список, обрабатывает его и выводит результат
int main() {
    struct Node* start = NULL;
    struct Node* end = NULL;
    char ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        AppendToList(&start, ch);
    }

    Process(start, &end);
    Output(end);
    Clear(start);
    Clear(end);

    printf("Press Enter to escape:\n");
    if (ch != '\n') {
        while ((ch = getchar()) != '\n' && ch != EOF);
    }
    getchar();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define MAX_STACK 1000
#define MAX_INPUT 10001

typedef struct {
    int data[MAX_STACK];
    int top;
} Stack;

void push(Stack *s, int value) {
    if (s->top >= MAX_STACK - 1) {
        fprintf(stderr, "Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    s->data[++s->top] = value;
}

int pop(Stack *s) {
    if (s->top == -1) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return s->data[s->top--];
}

int main() {
    char input[MAX_INPUT];
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }

    // Удаляем перевод строки и проверяем точку в конце
    input[strcspn(input, "\n")] = '\0';
    size_t len = strlen(input);
    if (len == 0 || input[len-1] != '.') {
        fprintf(stderr, "Expression must end with a dot\n");
        return EXIT_FAILURE;
    }
    input[len-1] = '\0'; // Удаляем точку

    Stack stack = { .top = -1 };

    char *token = strtok(input, " ");
    while (token != NULL) {
        char *endptr;
        errno = 0;
        long num = strtol(token, &endptr, 10);

        // Обработка чисел
        if (endptr != token && *endptr == '\0') {
            if (errno == ERANGE || num > INT_MAX || num < INT_MIN) {
                fprintf(stderr, "Number out of range: %s\n", token);
                return EXIT_FAILURE;
            }
            push(&stack, (int)num);
        }
        // Обработка операторов
        else if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            if (stack.top < 1) {
                fprintf(stderr, "Not enough operands for operator %s\n", token);
                return EXIT_FAILURE;
            }
            int b = pop(&stack);
            int a = pop(&stack);
            int res;
            switch (token[0]) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if (b == 0) {
                        fprintf(stderr, "Division by zero\n");
                        return EXIT_FAILURE;
                    }
                    res = a / b;
                    break;
            }
            push(&stack, res);
        }
        // Некорректный токен
        else {
            fprintf(stderr, "Invalid token: %s\n", token);
            return EXIT_FAILURE;
        }

        token = strtok(NULL, " ");
    }

    // Проверка финального состояния стека
    if (stack.top != 0) {
        fprintf(stderr, "Invalid expression format\n");
        return EXIT_FAILURE;
    }

    printf("%d\n", pop(&stack));
    return EXIT_SUCCESS;
}

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Определение структуры списка
struct list {
  char word[20];
  struct list *next;
};

// Функция для добавления элемента в список
struct list *add_to_list(struct list *head, const char *word) {
  struct list *new_node = (struct list *)malloc(sizeof(struct list));
  if (!new_node) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }
  strcpy(new_node->word, word);
  new_node->next = head;
  return new_node;
}

// Функция для обмена местами двух элементов списка
void swap_elements(struct list *a, struct list *b) {
  char temp[20];
  strcpy(temp, a->word);
  strcpy(a->word, b->word);
  strcpy(b->word, temp);
}

// Функция для сортировки списка по алфавиту
void sort_list(struct list *head) {
  if (!head)
    return;
  struct list *i, *j;
  for (i = head; i->next != NULL; i = i->next) {
    for (j = i->next; j != NULL; j = j->next) {
      if (strcmp(i->word, j->word) > 0) {
        swap_elements(i, j);
      }
    }
  }
}

// Функция для печати списка
void print_list(struct list *head) {
  struct list *current = head;
  while (current) {
    printf("%s", current->word);
    if (current->next) {
      printf(" ");
    }
    current = current->next;
  }
  printf("\n");
}

// Функция для удаления списка
void delete_list(struct list *head) {
  struct list *current = head;
  while (current) {
    struct list *temp = current;
    current = current->next;
    free(temp);
  }
}

int main() {
  char input[1000];
  //    printf("Enter the string: ");
  fgets(input, sizeof(input), stdin);

  // Удаляем символ новой строки, если он есть
  size_t len = strlen(input);
  if (len > 0 && input[len - 1] == '\n') {
    input[len - 1] = '\0';
  }

  if (input[len - 2] != '.') {
    fprintf(stderr, "Error: the string should end with a dot.\n");
    return EXIT_FAILURE;
  }

  input[len - 2] = '\0'; // Убираем точку

  // Разбиваем строку на слова
  struct list *head = NULL;
  char *token = strtok(input, " ");
  while (token) {
    head = add_to_list(head, token);
    token = strtok(NULL, " ");
  }

  // Сортируем список
  sort_list(head);

  // Печатаем отсортированный список
  print_list(head);

  // Удаляем список
  delete_list(head);

  return 0;
}

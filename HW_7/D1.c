#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
  int key;
  struct tree *left, *right;
} tree;

void btUpView(tree *root) {
  // Определение структуры для хранения узлов с их горизонтальными расстояниями
  typedef struct {
    int hd;  // Горизонтальное расстояние
    int key; // Значение узла
  } NodeInfo;

  if (root == NULL) {
    return;
  }

  // Определяем диапазон горизонтальных расстояний
  int minHD = 0, maxHD = 0;
  tree *queue[1000]; // Очередь для обхода дерева
  int hdQueue[1000]; // Очередь для хранения горизонтальных расстояний
  int front = 0, rear = 0;

  // Добавляем корень в очередь
  queue[rear] = root;
  hdQueue[rear] = 0;
  rear++;

  // Массив для хранения первого узла на каждом горизонтальном расстоянии
  NodeInfo hdMap[1000];
  for (int i = 0; i < 1000; i++) {
    hdMap[i].hd = 2147483647; // INT_MAX
  }

  // Обход дерева
  while (front < rear) {
    tree *current = queue[front];
    int hd = hdQueue[front];
    front++;

    // Обновляем минимальное и максимальное горизонтальное расстояние
    if (hd < minHD) {
      minHD = hd;
    }
    if (hd > maxHD) {
      maxHD = hd;
    }

    // Если это первый узел на данном горизонтальном расстоянии, сохраняем его
    if (hdMap[hd + 500].hd ==
        2147483647) { // Смещение на 500 для работы с отрицательными индексами
      hdMap[hd + 500].hd = hd;
      hdMap[hd + 500].key = current->key;
    }

    // Добавляем левого потомка в очередь
    if (current->left != NULL) {
      queue[rear] = current->left;
      hdQueue[rear] = hd - 1;
      rear++;
    }

    // Добавляем правого потомка в очередь
    if (current->right != NULL) {
      queue[rear] = current->right;
      hdQueue[rear] = hd + 1;
      rear++;
    }
  }

  // Печатаем вид сверху
  for (int i = minHD + 500; i <= maxHD + 500; i++) {
    if (hdMap[i].hd != 2147483647) {
      printf("%d ", hdMap[i].key);
    }
  }
  printf("\n");
}

int main() {
  tree n1 = {1, NULL, NULL};
  tree n2 = {2, NULL, NULL};
  tree n3 = {3, NULL, NULL};
  tree n4 = {4, NULL, NULL};
  tree n5 = {5, NULL, NULL};

  n1.left = &n2;
  n1.right = &n3;
  n2.left = &n4;
  n2.right = &n5;

  btUpView(&n1); // Ожидаемый вывод: 4 2 1 3
  return 0;
}

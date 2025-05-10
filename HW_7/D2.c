#include <stdio.h>

// Определение структуры дерева
typedef struct tree {
  int key;
  struct tree *left, *right;
  struct tree *parent;
} tree;

// Функция для поиска брата
tree *findBrother(tree *root, int key) {
  if (root == NULL) {
    return 0;
  }
  // Ищем узел с заданным ключом
  tree *current = root;
  while (current != NULL) {
    if (current->key == key) {
      if (current->parent == NULL) {
        return 0; // У корня нет родителя, значит, нет брата
      }
      if (current->parent->left == current) {
        return current->parent->right; // Возвращаем правого ребенка
      } else {
        return current->parent->left; // Возвращаем левого ребенка
      }
    }
    // Переходим к следующему узлу
    if (key < current->key) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  return 0; // Узел с ключом не найден
}
int main() {

  tree node1 = {10, NULL, NULL, NULL};
  tree node2 = {5, NULL, NULL, &node1};
  tree node3 = {15, NULL, NULL, &node1};
  tree node4 = {3, NULL, NULL, &node2};
  tree node5 = {7, NULL, NULL, &node2};
  tree node6 = {12, NULL, NULL, &node3};
  tree node7 = {18, NULL, NULL, &node3};

  // Устанавливаем связи
  node1.left = &node2;
  node1.right = &node3;
  node2.left = &node4;
  node2.right = &node5;
  node3.left = &node6;
  node3.right = &node7;

  // Тестируем
  int testKeys[] = {5, 15, 3, 7, 12, 18, 10};
  for (int i = 0; i < 7; i++) {
    int key = testKeys[i];
    tree *brother = findBrother(&node1, key);
    if (brother) {
      printf("Key: %d, Brother: %d\n", key, brother->key);
    } else {
      printf("Key: %d, Brother not found\n", key);
    }
  }

  return 0;
}

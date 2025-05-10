#include "food.h"
#include <ncurses.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Places food on the game board.
 * 
 * @param food Array of food structures.
 * @param seed_number Number of food items to place.
 */
void putFood(food_t food[], size_t seed_number) {
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  srand(time(NULL));

  for (size_t i = 0; i < seed_number; i++) {
    food[i].x = rand() % max_x;
    food[i].y = MIN_Y + rand() % (max_y - MIN_Y);
    food[i].timestamp = time(NULL);
    food[i].enable = 1;
    attron(COLOR_PAIR(4));
    mvprintw(food[i].y, food[i].x, "O");
    attroff(COLOR_PAIR(4));
  }
}

/**
 * @brief Refreshes the food on the game board.
 * 
 * Removes expired food and generates new food if necessary.
 * 
 * @param food Array of food structures.
 * @param seed_number Number of food items to manage.
 */
void refreshFood(food_t food[], size_t seed_number) {
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);

  size_t active_food_count = 0;

  // Подсчет активной еды
  for (size_t i = 0; i < seed_number; i++) {
    if (food[i].enable) {
      active_food_count++;
    }
  }

  // Генерация новой еды, если активной еды меньше половины от seed_number
  for (size_t i = 0; i < seed_number; i++) {
    if ((!food[i].enable ||
         (time(NULL) - food[i].timestamp >= FOOD_EXPIRE_SECONDS)) &&
        active_food_count < seed_number / 2) {
      food[i].x = rand() % max_x;
      food[i].y = MIN_Y + rand() % (max_y - MIN_Y);
      food[i].timestamp = time(NULL);
      food[i].enable = 1;
      attron(COLOR_PAIR(4));
      mvprintw(food[i].y, food[i].x, "O");
      attroff(COLOR_PAIR(4));
      active_food_count++;
    }
  }
}

/**
 * @brief Checks if the snake eats any food.
 * 
 * @param head Pointer to the snake structure.
 * @param food Array of food structures.
 * @param seed_number Number of food items to check.
 * @return int 1 if food is eaten, 0 otherwise.
 */
int haveEat(snake_t *head, food_t food[], size_t seed_number) {
  for (size_t i = 0; i < seed_number; i++) {
    if (food[i].enable && head->x == food[i].x && head->y == food[i].y) {
      food[i].enable = 0;
      return 1; // Еда съедена
    }
  }
  return 0;
}

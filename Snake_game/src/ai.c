#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "ai.h"

/**
 * @brief Moves the AI-controlled snake towards the nearest food.
 * 
 * @param ai_snake Pointer to the AI snake structure.
 * @param food Array of food structures.
 * @param food_count Number of food items.
 */
void moveAI(snake_t *ai_snake, food_t food[], size_t food_count) {
  // Найти ближайшую еду
  int min_distance = INT_MAX;
  int target_x = ai_snake->x;
  int target_y = ai_snake->y;

  for (size_t i = 0; i < food_count; i++) {
    if (food[i].enable) {
      int distance =
          abs(ai_snake->x - food[i].x) + abs(ai_snake->y - food[i].y);
      if (distance < min_distance) {
        min_distance = distance;
        target_x = food[i].x;
        target_y = food[i].y;
      }
    }
  }

  // Двигаться к цели
  if (ai_snake->x < target_x)
    ai_snake->direction = RIGHT;
  else if (ai_snake->x > target_x)
    ai_snake->direction = LEFT;
  else if (ai_snake->y < target_y)
    ai_snake->direction = DOWN;
  else if (ai_snake->y > target_y)
    ai_snake->direction = UP;
}

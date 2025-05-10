#ifndef AI_H
#define AI_H

#include "food.h"
#include "snake.h"

// Function to move the AI-controlled snake towards the nearest food
void moveAI(snake_t *ai_snake, food_t food[], size_t food_count);

#endif // AI_H
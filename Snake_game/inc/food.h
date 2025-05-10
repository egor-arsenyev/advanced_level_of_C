#ifndef FOOD_H
#define FOOD_H

#define MIN_Y 2

#include "snake.h"
#include <time.h>

enum {MAX_FOOD_SIZE=20, FOOD_EXPIRE_SECONDS=10};

// Structure for food
typedef struct
{
    int x;
    int y;
    time_t timestamp;
    int enable; // 1 - active, 0 - eaten
} food_t;

void putFood(food_t food[], size_t seed_number);
void refreshFood(food_t food[], size_t seed_number);
int haveEat(snake_t *head, food_t food[], size_t seed_number);



#endif // FOOD_H
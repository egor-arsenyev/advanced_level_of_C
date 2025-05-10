// File snake.h contains structure definitions and function prototypes related to snake control.

#ifndef SNAKE_H
#define SNAKE_H

#include <stddef.h>
#include <ncurses.h>

#define CONTROLS 2
#define MIN_Y 2

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};
enum {MAX_TAIL_SIZE=100, START_TAIL_SIZE=3};

// Structure for snake controls
typedef struct 
{
    int down;
    int up;
    int left;
    int right;
} control_buttons_t;

// Structure for the snake's tail
typedef struct
{
    int x;
    int y;
} tail_t;

// Structure for the snake's head
typedef struct
{
    int x;
    int y;
    int direction;
    size_t tsize;
    tail_t *tail;
    control_buttons_t controls;
    int score; // Snake's score
} snake_t;

void initTail(tail_t t[], size_t size);
void initSnake(snake_t *head, size_t size, int x, int y);
int checkCollision(const snake_t *snake);
void addTail(snake_t *head);
void go(snake_t *head, int color_pair);
int checkDirection(snake_t *snake, int32_t key);
void changeDirection(snake_t *snake, const int32_t key);
void goTail(snake_t *head, int color_pair);

#endif // SNAKE_H
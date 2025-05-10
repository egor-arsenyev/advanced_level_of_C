#include "snake.h"
#include <stdio.h>
#include <stdlib.h>

// Array of control schemes
control_buttons_t default_controls[CONTROLS] = {
    {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT}, // Arrow keys
    {'s', 'w', 'a', 'd'}                    // WSAD keys
};

/**
 * @brief Initializes the tail of the snake.
 * 
 * @param t Array of tail segments.
 * @param size Size of the tail array.
 */
void initTail(tail_t t[], size_t size) {
  tail_t init_t = {0, 0};
  for (size_t i = 0; i < size; i++) {
    t[i] = init_t;
  }
}

/**
 * @brief Initializes the head of the snake.
 * 
 * @param head Pointer to the snake structure.
 * @param x Initial x-coordinate of the snake's head.
 * @param y Initial y-coordinate of the snake's head.
 */
void initHead(snake_t *head, int x, int y) {
  head->x = x;
  head->y = y;
  head->direction = RIGHT;
}

/**
 * @brief Initializes the snake with a head and tail.
 * 
 * @param head Pointer to the snake structure.
 * @param size Initial size of the snake's tail.
 * @param x Initial x-coordinate of the snake's head.
 * @param y Initial y-coordinate of the snake's head.
 */
void initSnake(snake_t *head, size_t size, int x, int y) {
  tail_t *tail = (tail_t *)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
  if (!tail) {
    perror("Failed to allocate memory for snake tail");
    exit(EXIT_FAILURE);
  }
  initTail(tail, MAX_TAIL_SIZE);
  initHead(head, x, y);
  head->tail = tail;
  head->tsize = size + 1;
  head->controls = default_controls[0]; // Default control scheme: arrow keys
  head->score = 0;                      // Initialize score
}

/**
 * @brief Checks if the snake's head collides with its tail.
 * 
 * @param snake Pointer to the snake structure.
 * @return int 1 if a collision occurs, 0 otherwise.
 */
int checkCollision(const snake_t *snake) {
  for (size_t i = 0; i < snake->tsize; i++) {
    if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y) {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief Adds a segment to the snake's tail.
 * 
 * @param head Pointer to the snake structure.
 */
void addTail(snake_t *head) {
  if (head->tsize < MAX_TAIL_SIZE) {
    head->tsize++;
  }
}

/**
 * @brief Moves the snake's head in the current direction.
 * 
 * @param head Pointer to the snake structure.
 * @param color_pair Color pair to use for the snake's head.
 */
void go(snake_t *head, int color_pair) {
  attron(COLOR_PAIR(color_pair));
  mvprintw(head->y, head->x, " ");
  attroff(COLOR_PAIR(color_pair));
  char ch = '@';
  int max_x = 0, max_y = 0;
  getmaxyx(stdscr, max_y, max_x);
  mvprintw(head->y, head->x, " ");

  switch (head->direction) {
  case LEFT:
    head->x--;
    if (head->x < 0)
      head->x = max_x - 1;
    break;
  case RIGHT:
    head->x++;
    if (head->x >= max_x)
      head->x = 0;
    break;
  case UP:
    head->y--;
    if (head->y < MIN_Y)
      head->y = max_y - 1;
    break;
  case DOWN:
    head->y++;
    if (head->y >= max_y)
      head->y = MIN_Y;
    break;
  default:
    break;
  }
  mvprintw(head->y, head->x, "%c", ch);
  refresh();
  attron(COLOR_PAIR(color_pair));
  mvprintw(head->y, head->x, "@");
  attroff(COLOR_PAIR(color_pair));
  refresh();
}

/**
 * @brief Checks if the new direction is valid for the snake.
 * 
 * Prevents the snake from moving in the opposite direction.
 * 
 * @param snake Pointer to the snake structure.
 * @param key Key pressed by the user.
 * @return int 1 if the direction is valid, 0 otherwise.
 */
int checkDirection(snake_t *snake, int32_t key) {
  for (int i = 0; i < CONTROLS; i++) {
    if ((snake->direction == RIGHT && key == default_controls[i].left) ||
        (snake->direction == LEFT && key == default_controls[i].right) ||
        (snake->direction == UP && key == default_controls[i].down) ||
        (snake->direction == DOWN && key == default_controls[i].up)) {
      return 0; // Invalid direction
    }
  }
  return 1; // Valid direction
}

/**
 * @brief Changes the direction of the snake based on user input.
 * 
 * @param snake Pointer to the snake structure.
 * @param key Key pressed by the user.
 */
void changeDirection(snake_t *snake, const int32_t key) {
  switch (key) {
  case 's':
  case 'S':
  case KEY_DOWN: // Down
    if (checkDirection(snake, KEY_DOWN))
      snake->direction = DOWN;
    break;
  case 'w':
  case 'W':
  case KEY_UP: // Up
    if (checkDirection(snake, KEY_UP))
      snake->direction = UP;
    break;
  case 'd':
  case 'D':
  case KEY_RIGHT: // Right
    if (checkDirection(snake, KEY_RIGHT))
      snake->direction = RIGHT;
    break;
  case 'a':
  case 'A':
  case KEY_LEFT: // Left
    if (checkDirection(snake, KEY_LEFT))
      snake->direction = LEFT;
    break;
  default:
    break;
  }
}

/**
 * @brief Moves the snake's tail to follow the head.
 * 
 * @param head Pointer to the snake structure.
 * @param color_pair Color pair to use for the snake's tail.
 */
void goTail(snake_t *head, int color_pair) {
  attron(COLOR_PAIR(color_pair));
  mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");
  for (size_t i = head->tsize - 1; i > 0; i--) {
    head->tail[i] = head->tail[i - 1];
    if (head->tail[i].y || head->tail[i].x)
      mvprintw(head->tail[i].y, head->tail[i].x, "*");
  }
  head->tail[0].x = head->x;
  head->tail[0].y = head->y;
  attroff(COLOR_PAIR(color_pair));
}

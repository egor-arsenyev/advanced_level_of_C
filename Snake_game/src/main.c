#include "main.h"

/**
 * @brief Main function of the Snake game.
 *
 * Initializes the game, handles game modes, and manages the game loop.
 *
 * @return int Exit status of the program.
 */
int main() {
  int game_mode = 0, player_color = 1, ai_color = 2;

  /*Displays the start menu and allows the user to select game mode and
   * colors.*/
  startMenu(&game_mode, &player_color, &ai_color);

  clear();
  refresh();
  initscr();
  start_color();
  initColors();
  bkgd(COLOR_PAIR(100));
  keypad(stdscr, TRUE);
  raw();
  noecho();
  curs_set(FALSE);
  mvprintw(1, 0, "Use arrows or WSAD for control. Press 'F10' for EXIT");
  timeout(0);

  /*Allocates memory for the player's snake, AI snake, and food array.*/
  snake_t *snake = (snake_t *)malloc(sizeof(snake_t));
  snake_t *ai_snake = (snake_t *)malloc(sizeof(snake_t));
  food_t food[MAX_FOOD_SIZE];

  clock_t last_time = clock();
  const double frame_delay = 100;
  int key_pressed = -1;
  int game_over = 0;

  if (game_mode == 1) {
    /*Initializes the player's snake and places food on the board.*/
    initSnake(snake, START_TAIL_SIZE, 10, 10);
    putFood(food, MAX_FOOD_SIZE);

    while (key_pressed != STOP_GAME && snake->score < 100) {
      clock_t current_time = clock();
      double elapsed =
          (double)(current_time - last_time) * 50000 / CLOCKS_PER_SEC;

      if (elapsed >= frame_delay) {
        last_time = current_time;
        key_pressed = getch();
        changeDirection(snake, key_pressed);

        go(snake, player_color);

        /* @brief Checks for collisions and ends the game if the snake collides
         * with itself or the wall.*/
        if (checkCollision(snake)) {
          int max_y, max_x;
          getmaxyx(stdscr, max_y, max_x);
          mvprintw(max_y / 2, max_x / 2 - 5, "GAME OVER!");
          refresh();
          sleep(2);
          game_over = 1;
          break;
        }

        goTail(snake, player_color);

        /*Checks if the snake eats food and increases its tail size and score.*/
        if (haveEat(snake, food, MAX_FOOD_SIZE)) {
          addTail(snake);
          snake->score++;
        }
        if (snake->score >= 10) {
          game_over = 1;
          int max_y, max_x;
          getmaxyx(stdscr, max_y, max_x);
          mvprintw(max_y / 2, max_x / 2 - 5, "YOU WIN!");
          refresh();
          sleep(2);
          break;
        }
        refreshFood(food, MAX_FOOD_SIZE);
      }
      usleep(1000);
    }

  } else if (game_mode == 2) {
    /*Initializes both player's and AI's snakes and places food on the board.*/
    initSnake(snake, START_TAIL_SIZE, 10, 10);
    initSnake(ai_snake, START_TAIL_SIZE, 20, 20);
    putFood(food, MAX_FOOD_SIZE);
    while (key_pressed != STOP_GAME && !game_over) {
      clock_t current_time = clock();
      double elapsed =
          (double)(current_time - last_time) * 50000 / CLOCKS_PER_SEC;

      if (elapsed >= frame_delay) {
        last_time = current_time;
        key_pressed = getch();
        changeDirection(snake, key_pressed);

        /*Moves the AI snake based on the food's position.*/
        moveAI(ai_snake, food, MAX_FOOD_SIZE);

        go(snake, player_color);
        go(ai_snake, ai_color);

        goTail(snake, player_color);
        goTail(ai_snake, ai_color);
        /*
                if (checkCollision(snake)) {
                  int max_y, max_x;
                  getmaxyx(stdscr, max_y, max_x);
                  mvprintw(max_y / 2, max_x / 2 - 5, "GAME OVER!");
                  refresh();
                  sleep(2);
                  game_over = 1;
                  break;
                }
        */
        /*Checks if the player's or AI's snake eats food and updates their
         * scores.*/
        if (haveEat(snake, food, MAX_FOOD_SIZE)) {
          addTail(snake);
          snake->score++;
        }
        if (haveEat(ai_snake, food, MAX_FOOD_SIZE)) {
          addTail(ai_snake);
          ai_snake->score++;
        }

        int active_food_count = 0;
        for (size_t i = 0; i < MAX_FOOD_SIZE; i++) {
          if (food[i].enable) {
            active_food_count++;
          }
        }

        if (active_food_count == 0) {
          game_over = 1;
          break;
        }
      }
      usleep(1000);
    }

    /*Displays the result of the game (Player Wins, AI Wins, or Draw).*/
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    if (snake->score > ai_snake->score)
      mvprintw(max_y / 2, max_x / 2 - 5, "PLAYER WINS!");
    else if (snake->score < ai_snake->score)
      mvprintw(max_y / 2, max_x / 2 - 5, "AI WINS!");
    else
      mvprintw(max_y / 2, max_x / 2 - 5, "DRAW!");

    refresh();
    sleep(2);
  }

  /*Frees allocated memory for snakes and ends the ncurses session.*/
  free(snake->tail);
  free(snake);
  free(ai_snake->tail);
  free(ai_snake);
  endwin();
  return 0;
}

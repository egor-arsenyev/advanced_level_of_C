#include "utils.h"
#include <ncurses.h>

/**
 * @brief Displays the start menu for the Snake game.
 * 
 * Allows the user to select the game mode and snake colors.
 * 
 * @param game_mode Pointer to the selected game mode.
 * @param player_color Pointer to the player's snake color.
 * @param ai_color Pointer to the AI snake's color.
 */
void startMenu(int *game_mode, int *player_color, int *ai_color) {
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(FALSE);

  int choice = 0;
  int highlight = 0;
  const char *menu[] = {"Single Player Mode", "Multiplayer Mode (vs AI)",
                        "Exit"};
  const int menu_size = sizeof(menu) / sizeof(menu[0]);

  while (1) {
    clear();
    mvprintw(1, 0, "Welcome to Snake Game!");
    mvprintw(2, 0, "Use arrow keys to navigate and Enter to select.");
    for (int i = 0; i < menu_size; i++) {
      if (i == highlight)
        attron(A_REVERSE);
      mvprintw(4 + i, 0, "%s", menu[i]);
      attroff(A_REVERSE);
    }
    choice = getch();
    switch (choice) {
    case KEY_UP:
      highlight = (highlight - 1 + menu_size) % menu_size;
      break;
    case KEY_DOWN:
      highlight = (highlight + 1) % menu_size;
      break;
    case '\n':
      if (highlight == 2) // Exit
      {
        endwin();
        exit(0);
      }
      *game_mode = highlight + 1;
      goto color_selection;
    }
  }

color_selection:
  clear();
  mvprintw(1, 0, "Choose Player Snake Color:");
  mvprintw(2, 0, "1. Green");
  mvprintw(3, 0, "2. Red");
  mvprintw(4, 0, "3. Blue");
  mvprintw(5, 0, "Enter your choice (1-3): ");
  echo();
  scanw("%d", player_color);
  noecho();

  if (*game_mode == 2) // Multiplayer mode
  {
    mvprintw(7, 0, "Choose AI Snake Color:");
    mvprintw(8, 0, "1. Green");
    mvprintw(9, 0, "2. Red");
    mvprintw(10, 0, "3. Blue");
    mvprintw(11, 0, "Enter your choice (1-3): ");
    echo();
    scanw("%d", ai_color);
    noecho();
  }

  endwin();
}

/**
 * @brief Initializes color pairs for the game.
 * 
 * Sets up color pairs for the player's snake, AI snake, and food.
 */
void initColors() {
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);  // Green
  init_pair(2, COLOR_RED, COLOR_BLACK);    // Red
  init_pair(3, COLOR_BLUE, COLOR_BLACK);   // Blue
  init_pair(4, COLOR_YELLOW, COLOR_BLACK); // Food
}

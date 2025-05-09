#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 

#define MIN_Y  2
#define CONTROLS 2

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};
enum {MAX_TAIL_SIZE=100, START_TAIL_SIZE=3, MAX_FOOD_SIZE=20, FOOD_EXPIRE_SECONDS=10};

// Структура для управления змейкой
typedef struct 
{
    int down;
    int up;
    int left;
    int right;
} control_buttons_t;

// Массив схем управления
control_buttons_t default_controls[CONTROLS] = {
    {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT}, // Стрелки
    {'s', 'w', 'a', 'd'}                    // WSAD
};

// Структура для хвоста змейки
typedef struct
{
    int x;
    int y;
} tail_t;

// Структура для головы змейки
typedef struct
{
    int x;
    int y;
    int direction;
    size_t tsize;
    tail_t *tail;
    control_buttons_t controls;
} snake_t;

// Структура для еды
typedef struct
{
    int x;
    int y;
    time_t timestamp;
    int enable; // 1 - активна, 0 - съедена
} food_t;

// Инициализация хвоста
void initTail(tail_t t[], size_t size)
{
    tail_t init_t = {0, 0};
    for (size_t i = 0; i < size; i++)
    {
        t[i] = init_t;
    }
}

// Инициализация головы змейки
void initHead(snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

// Инициализация змейки
void initSnake(snake_t *head, size_t size, int x, int y)
{
    tail_t *tail = (tail_t *)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    if (!tail)
    {
        perror("Failed to allocate memory for snake tail");
        exit(EXIT_FAILURE);
    }
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail;
    head->tsize = size + 1;
    head->controls = default_controls[0]; // По умолчанию управление стрелками
}

// Проверка на столкновение головы с хвостом
int checkCollision(const snake_t *snake)
{
    for (size_t i = 0; i < snake->tsize; i++)
    {
        if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y)
        {
            return 1;
        }
    }
    return 0;
}

// Увеличение хвоста
void addTail(snake_t *head)
{
    if (head->tsize < MAX_TAIL_SIZE)
    {
        head->tsize++;
    }
}

// Движение головы
void go(snake_t *head)
{
    char ch = '@';
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(head->y, head->x, " ");

    switch (head->direction)
    {
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
}

// Проверка корректности направления
int checkDirection(snake_t *snake, int32_t key)
{
    for (int i = 0; i < CONTROLS; i++)
    {
        if ((snake->direction == RIGHT && key == default_controls[i].left) ||
            (snake->direction == LEFT && key == default_controls[i].right) ||
            (snake->direction == UP && key == default_controls[i].down) ||
            (snake->direction == DOWN && key == default_controls[i].up))
        {
            return 0; // Некорректное направление
        }
    }
    return 1; // Корректное направление
}

// Изменение направления движения
void changeDirection(snake_t *snake, const int32_t key)
{
    switch (key)
    {
        case 's': case 'S': case KEY_DOWN: // Вниз
            if (checkDirection(snake, KEY_DOWN))
                snake->direction = DOWN;
            break;
        case 'w': case 'W': case KEY_UP: // Вверх
            if (checkDirection(snake, KEY_UP))
                snake->direction = UP;
            break;
        case 'd': case 'D': case KEY_RIGHT: // Вправо
            if (checkDirection(snake, KEY_RIGHT))
                snake->direction = RIGHT;
            break;
        case 'a': case 'A': case KEY_LEFT: // Влево
            if (checkDirection(snake, KEY_LEFT))
                snake->direction = LEFT;
            break;
        default:
            break;
    }
}

// Движение хвоста
void goTail(snake_t *head)
{
    char ch = '*';
    mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");
    for (size_t i = head->tsize - 1; i > 0; i--)
    {
        head->tail[i] = head->tail[i - 1];
        if (head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

// Расстановка еды
void putFood(food_t food[], size_t seed_number)
{
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    srand(time(NULL));

    for (size_t i = 0; i < seed_number; i++)
    {
        food[i].x = rand() % max_x;
        food[i].y = MIN_Y + rand() % (max_y - MIN_Y);
        food[i].timestamp = time(NULL);
        food[i].enable = 1;
        mvprintw(food[i].y, food[i].x, "O");
    }
}

// Обновление еды
void refreshFood(food_t food[], size_t seed_number)
{
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    size_t active_food_count = 0;

    // Подсчет активной еды
    for (size_t i = 0; i < seed_number; i++)
    {
        if (food[i].enable)
        {
            active_food_count++;
        }
    }

    // Генерация новой еды, если активной еды меньше половины от seed_number
    for (size_t i = 0; i < seed_number; i++)
    {
        if ((!food[i].enable || (time(NULL) - food[i].timestamp >= FOOD_EXPIRE_SECONDS)) &&
            active_food_count < seed_number / 2)
        {
            food[i].x = rand() % max_x;
            food[i].y = MIN_Y + rand() % (max_y - MIN_Y);
            food[i].timestamp = time(NULL);
            food[i].enable = 1;
            mvprintw(food[i].y, food[i].x, "O");
            active_food_count++;
        }
    }
}

// Проверка, съедена ли еда
int haveEat(snake_t *head, food_t food[], size_t seed_number)
{
    for (size_t i = 0; i < seed_number; i++)
    {
        if (food[i].enable && head->x == food[i].x && head->y == food[i].y)
        {
            food[i].enable = 0;
            return 1; // Еда съедена
        }
    }
    return 0;
}

// Основная функция
int main()
{
    snake_t *snake = (snake_t *)malloc(sizeof(snake_t));
    food_t food[MAX_FOOD_SIZE];

    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);
    mvprintw(1, 0, "Use arrows or WSAD for control. Press 'F10' for EXIT");
    timeout(0);
    
    initSnake(snake, START_TAIL_SIZE, 10, 10);
    putFood(food, MAX_FOOD_SIZE);

    clock_t last_time = clock();
    const double frame_delay = 100; // 100 ms per frame
    int key_pressed = 0;
    int game_over = 0;

    while (key_pressed != STOP_GAME && !game_over)
    {
        clock_t current_time = clock();
        double elapsed = (double)(current_time - last_time) * 100000 / CLOCKS_PER_SEC;

        if (elapsed >= frame_delay)
        {
            last_time = current_time;
            key_pressed = getch();
            changeDirection(snake, key_pressed);
            go(snake);

            if (checkCollision(snake))
            {
                int max_y, max_x;
                getmaxyx(stdscr, max_y, max_x);
                mvprintw(max_y / 2, max_x / 2 - 5, "GAME OVER!");
                refresh();
                sleep(2);
                game_over = 1;
                break;
            }

            if (haveEat(snake, food, MAX_FOOD_SIZE))
            {
                addTail(snake);
            }

            goTail(snake);
            refreshFood(food, MAX_FOOD_SIZE);
        }
        usleep(1000); // Reduce CPU usage
    }

    free(snake->tail);
    free(snake);
    endwin();
    return 0;
}

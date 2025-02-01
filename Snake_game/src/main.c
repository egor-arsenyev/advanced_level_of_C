#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h> 
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y  2
enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};
enum {MAX_TAIL_SIZE=100, START_TAIL_SIZE=3, MAX_FOOD_SIZE=20, FOOD_EXPIRE_SECONDS=10};


// Здесь храним коды управления змейкой
typedef struct
{
    int down;
    int up;
    int left;
    int right;
}control_buttons_t;

control_buttons_t default_controls = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};

/*
 Хвост это массив состоящий из координат x,y
 */
typedef struct
{
    int x;
    int y;
} tail_t;

/*
 Голова змейки содержит в себе
 x,y - координаты текущей позиции
 direction - направление движения
 tsize - размер хвоста
 *tail -  ссылка на хвост
 */
typedef struct
{
    int x;
    int y;
    int direction;
    size_t tsize;
    tail_t *tail;
    control_buttons_t controls;
}snake_t;

void initTail(tail_t t[], size_t size)
{
    tail_t init_t={0,0};
    for(size_t i=0; i<size; i++)
    {
        t[i]=init_t;
    }
}
void initHead(snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t *head, size_t size, int x, int y)
{
tail_t*  tail  = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail; // прикрепляем к голове хвост
    head->tsize = size+1;
    head->controls = default_controls;
}

/*
Проверка на совпадение координат головы с любым сегментом хвоста
*/
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

/*
 Движение головы с учетом текущего направления движения
 При переходе через границу терминала, змейка появляется на противоположной стороне
 */
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

void changeDirection(snake_t* snake, const int32_t key)
{
    if (key == snake->controls.down)
        snake->direction = DOWN;
    else if (key == snake->controls.up)
        snake->direction = UP;
    else if (key == snake->controls.right)
        snake->direction = RIGHT;
    else if (key == snake->controls.left)
        snake->direction = LEFT;
}

/*
 Движение хвоста с учетом движения головы
 */
void goTail(snake_t *head)
{
    char ch = '*';
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " ");
    for(size_t i = head->tsize-1; i>0; i--)
    {
        head->tail[i] = head->tail[i-1];
        if( head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

int main()
{
    snake_t* snake = (snake_t*)malloc(sizeof(snake_t));
    initSnake(snake, START_TAIL_SIZE, 10, 10);
    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);
    mvprintw(1, 0, "Use arrows for control. Press 'F10' for EXIT");
    timeout(0);

    clock_t last_time = clock();
    const double frame_delay = 100; // 100 ms per frame
    int key_pressed = 0;
    int game_over = 0;

    while (key_pressed != STOP_GAME && !game_over)
    {
        clock_t current_time = clock();
        double elapsed = (double)(current_time - last_time) * 50000 / CLOCKS_PER_SEC;

        if (elapsed >= frame_delay)
        {
            last_time = current_time;
            key_pressed = getch();
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
            goTail(snake);
            changeDirection(snake, key_pressed);
        }
        usleep(1000); // Reduce CPU usage
    }

    free(snake->tail);
    free(snake);
    endwin();
    return 0;
}

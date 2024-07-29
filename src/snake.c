#include "raylib.h"
#include "raymath.h"
#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static void draw_background(void);
static void init_board(void);
static void draw_board(void);
static void init_snake(void);
static void update_snake(void);
static void update_direction(void);
static void game_over(void);
static void draw_apple(void);
static void increase_snake(void);


/* The game board which controls the grid, indexes are in (x, y) format */
Cell board[COLS][ROWS];

Snake *snake;

int apple_exists = 0;

int main(void) {
    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");
    SetTargetFPS(FPS);
    srand(time(NULL));

    init_board();
    init_snake();

    double last_time = GetTime();
    while (!WindowShouldClose()) {
        if (!apple_exists) {
            draw_apple();
        }

        BeginDrawing();
        draw_background();
        draw_board();
        EndDrawing();

        update_direction();

        /* Update snake's position at 10 FPS */
        if((GetTime() - last_time) > (1.0 / POSITION_FPS)) {
            update_snake();
            last_time = GetTime();
        }  
    }

    CloseWindow();

    return 0;
}

static void draw_background(void) {
    ClearBackground(WHITE);
}

static void init_board(void) {
    for (int x = 0; x < COLS; x++) {
        for (int y = 0; y < ROWS; y++) {
            board[x][y] = (Cell) {.x = x, .y = y, .type = EMPTY};
        }
    }
}

static void draw_board() {
    for (int x = 0; x < COLS; x++) {
        for (int y = 0; y < ROWS; y++) {
            Cell c = board[x][y];
            Color color;
            
            if (c.type == SNAKE_BODY || c.type == SNAKE_HEAD) {
                color = GREEN;
            }
            else if (c.type == APPLE) {
                color = RED;
            }
            else {
                color = BLACK;
            }

            DrawRectangle(c.x * CELL_WIDTH, c.y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, BLACK);
            DrawRectangle(c.x * CELL_WIDTH + 1, c.y * CELL_WIDTH - 1, CELL_WIDTH - 2, CELL_WIDTH - 2, color);
        }
    }
}

static void init_snake(void) {
    snake = malloc(sizeof(Snake));
    snake->head = malloc(sizeof(SnakeCell));
    snake->tail = snake->head;
    snake->head->x = 0;
    snake->head->y = 0;
    snake->head->direction = RIGHT;
    board[0][0].type = SNAKE_HEAD;
}

static void update_direction(void) {
    SnakeCell *head = snake->head;

    /* Update direction */
    if (IsKeyDown(KEY_RIGHT) && head->direction != LEFT) {
        head->direction = RIGHT;
    }
    if (IsKeyDown(KEY_LEFT) && head->direction != RIGHT) {
        head->direction = LEFT;
    }
    if (IsKeyDown(KEY_UP) && head->direction != DOWN) {
        head->direction = UP;
    }
    if (IsKeyDown(KEY_DOWN) && head->direction != UP) {
        head->direction = DOWN;
    }
}

static void update_snake(void) {
    SnakeCell *cell = snake->head;

    int x = cell->x, y = cell->y;
    Direction parent_direction = cell->direction;

    if (cell->direction == RIGHT) {
        cell->x += 1;
    } else if (cell->direction == LEFT) {
        cell->x -= 1;
    } else if (cell->direction == DOWN) {
        cell->y += 1;
    } else {
        cell->y -= 1;
    }

    /* Check if snake hit the edge of the board */
    if (cell->x < 0 || cell->x >= COLS || cell->y < 0 || cell->y >= ROWS) {
        game_over();
    }

    /* Check if snake collided with itself */
    if (board[cell->x][cell->y].type == SNAKE_BODY) {
        game_over();
    }

    /* Check if snake ate an apple */
    if (board[cell->x][cell->y].type == APPLE) {
        increase_snake();
        apple_exists = 0;
    }

    board[cell->x][cell->y].type = SNAKE_HEAD; /* Update board of snake's location */
    cell = cell->next;
    while (cell) {
        x = cell->x;
        y = cell->y;
        if (cell->direction == RIGHT) {
            cell->x += 1;
        } else if (cell->direction == LEFT) {
            cell->x -= 1;
        } else if (cell->direction == DOWN) {
            cell->y += 1;
        } else {
            cell->y -= 1;
        }

        /* Change direction of snake body */
        Direction curr_direction = cell->direction;
        cell->direction = parent_direction;
        parent_direction = curr_direction;
        

        board[cell->x][cell->y].type = SNAKE_BODY; /* Update board of snake's location*/
        cell = cell->next;
    }

    board[x][y].type = EMPTY;
}

static void game_over(void) {
    CloseWindow();
    exit(EXIT_SUCCESS);
}

static void draw_apple(void) {
    /* Get a random cell in board that is not occupied by the snake */
    int rand_x, rand_y;
    do {
        rand_x = rand() % COLS;
        rand_y = rand() % ROWS;
    } while (board[rand_x][rand_y].type != EMPTY);

    board[rand_x][rand_y].type = APPLE;
    apple_exists = 1;
}

static void increase_snake(void) {
    SnakeCell *new_tail = malloc(sizeof(SnakeCell));
    new_tail->x = snake->tail->x;
    new_tail->y = snake->tail->y;

    int offset = 1;
    if (snake->head == snake->tail) {
        offset = 2;
    }

    if (snake->tail->direction == RIGHT) {
        new_tail->direction = RIGHT;
        new_tail->x -= offset;
    } else if (snake->tail->direction == LEFT) {
        new_tail->direction = LEFT;
        new_tail->x += offset;
    } else if (snake->tail->direction == DOWN) {
        new_tail->direction = DOWN;
        new_tail->y -= offset;
    } else {
        new_tail->direction = UP;
        new_tail->y += offset;
    }

    snake->tail->next = new_tail;
    snake->tail = new_tail;    
}





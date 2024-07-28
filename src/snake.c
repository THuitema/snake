#include "raylib.h"
#include "raymath.h"
#include "snake.h"
#include <stdlib.h>
#include <stdio.h>

static void draw_background(void);
static void init_board(void);
static void draw_board(void);
static void init_snake(void);
static void update_snake(void);
static void update_direction(void);
static void game_over(void);
static void draw_apple(void);

/* The game board which controls the grid, indexes are in (x, y) format */
Cell board[COLS][ROWS];

/* Snake */
Snake *snake;

int apple_exists = 0;

/*
TODO: draw apples on board and grow snake
*/
int main(void) {
    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");
    SetTargetFPS(FPS);

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
        update_direction();
        
        
        EndDrawing();

        /* Update snake's position at 10 FPS */
        if((GetTime() - last_time) > 0.1) {
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
            if (c.type == SNAKE_HEAD) {
                DrawRectangle(c.x * CELL_WIDTH, c.y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, DARKGREEN);
            } 
            else if (c.type == SNAKE_BODY) {
                DrawRectangle(c.x * CELL_WIDTH, c.y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, GREEN);
            }
            else if (c.type == APPLE) {
                DrawRectangle(c.x * CELL_WIDTH, c.y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, RED);
            }
            else {
                DrawRectangleLines(c.x * CELL_WIDTH, c.y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, LIGHTGRAY);
            }
        }
    }
}

static void init_snake(void) {
    snake = malloc(sizeof(Snake));
    snake->head = malloc(sizeof(SnakeCell));
    snake->head->x = 0;
    snake->head->y = 0;
    snake->head->direction = RIGHT;
    board[0][0].type = SNAKE_HEAD;
}

static void update_direction(void) {
    SnakeCell *head = snake->head;

    /* Update direction */
    if (IsKeyDown(KEY_RIGHT)) head->direction = RIGHT;
    if (IsKeyDown(KEY_LEFT)) head->direction = LEFT;
    if (IsKeyDown(KEY_UP)) head->direction = UP;
    if (IsKeyDown(KEY_DOWN)) head->direction = DOWN;
}

static void update_snake(void) {
    SnakeCell *cell = snake->head;

    int x = cell->x, y = cell->y;

    if (cell->direction == RIGHT) {
        cell->x += 1;
    } else if (cell->direction == LEFT) {
        cell->x -= 1;
    } else if (cell->direction == DOWN) {
        cell->y += 1;
    } else {
        cell->y -= 1;
    }

    /* Check that snake didn't hit the edge of the board */
    if (cell->x < 0 || cell->x >= COLS || cell->y < 0 || cell->y >= ROWS) {
        game_over();
    }
    board[cell->x][cell->y].type = SNAKE_HEAD; /* Update board of snake's location*/
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





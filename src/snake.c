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

/* The game board which controls the grid, indexes are in (x, y) format */
Cell board[COLS][ROWS];

/* Snake */
Snake *snake;

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");
    SetTargetFPS(FPS);

    init_board();
    init_snake();
    printf("Snake %d, %d", snake->head->x, snake->head->y);

    while (!WindowShouldClose()) {
        BeginDrawing();
            draw_background();
            draw_board();
        EndDrawing();
        update_snake();
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

static void update_snake(void) {
    SnakeCell *cell = snake->head;

    int x, y;

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
        board[cell->x][cell->y].type = SNAKE_HEAD; /* Update board of snake's location*/
        cell = cell->next;
    }
    board[x][y].type = EMPTY; /* */
}






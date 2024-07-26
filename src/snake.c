#include "raylib.h"
#include "raymath.h"
#include "snake.h"
#include <stdlib.h>
#include <stdio.h>

static void draw_background(void);
static void init_board(void);
static void draw_board(void);

/* Our game board, indexes are in (x, y) format */
Cell board[COLS][ROWS];

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");
    SetTargetFPS(FPS);

    init_board();

    while (!WindowShouldClose()) {
        BeginDrawing();
            draw_background();
            draw_board();
        EndDrawing();
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
            DrawRectangleLines(board[x][y].x * CELL_WIDTH, board[x][y].y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, BLUE);
        }
    }
}







#include "raylib.h"
#include "raymath.h"
#include "snake.h"
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define FPS 30
#define SNAKE_WIDTH 50

static Snake *init_snake(void);
static void update_snake(Snake *snake);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(FPS);

    Snake *snake = init_snake();

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw_snake(snake);
        EndDrawing();
        
        update_snake(snake);
    }

    CloseWindow();

    return 0;
}

static Snake *init_snake() {
    Snake *snake = malloc(sizeof(Snake));
    snake->size = 1;
    Vector2 shape = {SNAKE_WIDTH, SNAKE_WIDTH};
    snake->shape = shape;

    /* Initialize head of snake */
    snake->head = malloc(sizeof(SnakeCell));
    snake->head->direction = RIGHT;

    Vector2 position = {0, 0};
    snake->head->position = position;

    return snake;
}

static void update_snake(Snake *snake) {
    if (IsKeyDown(KEY_LEFT)) snake->head->direction = LEFT;
    if (IsKeyDown(KEY_RIGHT)) snake->head->direction = RIGHT;
    if (IsKeyDown(KEY_UP)) snake->head->direction = UP;
    if (IsKeyDown(KEY_DOWN)) snake->head->direction = DOWN; 

    /* See if we need to update direction */
    if (snake->head->direction == LEFT) snake->head->position.x -= SNAKE_WIDTH;
    if (snake->head->direction == RIGHT) snake->head->position.x += SNAKE_WIDTH;
    if (snake->head->direction == UP) snake->head->position.y -= SNAKE_WIDTH;
    if (snake->head->direction == DOWN) snake->head->position.y += SNAKE_WIDTH;
}

void draw_snake(Snake *snake) {
    /* Will have to implement some kind of loop to draw each SnakeCell and update positions */
    DrawRectangleV(snake->head->position, snake->shape, GREEN);
}



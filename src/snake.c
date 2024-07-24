#include "raylib.h"
#include "raymath.h"
#include "snake.h"
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define FPS 30

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);

    Snake *c = malloc(sizeof(Snake));

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Start Snake!", 190, 200, 20, DARKGRAY);
        EndDrawing();

        if (IsKeyDown(KEY_LEFT)) printf("Left\n");
        if (IsKeyDown(KEY_RIGHT)) printf("Right\n");
        if (IsKeyDown(KEY_UP)) printf("Up\n");
        if (IsKeyDown(KEY_DOWN)) printf("Down\n");        
    }

    CloseWindow();

    return 0;
}

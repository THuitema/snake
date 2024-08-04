#include "raylib.h"
#include "raymath.h"
#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static void draw_background(void);
static void draw_header(void);
static void init_board(void);
static void draw_board(void);
static void init_snake(void);
static void update_snake(void);
static void update_direction(void);
static void game_over(void);
static void draw_apple(void);
static void increase_snake(void);
static void draw_start_screen(void);
static void draw_game_over_screen(void);
static void reset_game(void);

Cell board[COLS][ROWS];

Snake *snake;

int apple_exists = 0;
int high_score = 0;

GameScreen screen = START;

int main(void) {
    /* Initial game settings and setup */
    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + HEADER_HEIGHT, "Snake");
    SetTargetFPS(FPS);
    srand(time(NULL));

    init_board();
    init_snake();

    double last_time = GetTime();

    /* Frame loop */
    while (!WindowShouldClose()) {
        BeginDrawing();
        draw_background();
        
        /* Start screen */
        if (screen == START) {
            draw_start_screen();
            if (IsKeyPressed(KEY_ENTER)) {
                screen = GAME;
            }
        }

        /* Game over screen */
        if (screen == GAMEOVER) {
            draw_game_over_screen();
            if (IsKeyPressed(KEY_ENTER)) {
                screen = GAME;
                reset_game();
            }
        }

        /* Game screen */
        if (screen == GAME) {
            if (!apple_exists) {
                draw_apple();
            }
            draw_board();
            update_direction();

            /* Update snake's position at 10 FPS */
            if((GetTime() - last_time) > (1.0 / POSITION_FPS)) {
                update_snake();
                last_time = GetTime();
            }  
        }
        draw_header();
        EndDrawing();  
    }

    CloseWindow();

    return 0;
}


static void draw_background(void) {
    ClearBackground(BLACK);
}

/* Draw panel at top of screen showing score and high score */
static void draw_header(void) {
    DrawLine(0, HEADER_HEIGHT, WINDOW_WIDTH, HEADER_HEIGHT, RAYWHITE);

    /* Only draw the score if the game is being played */
    char score[20];
    if (screen == GAME) {
        sprintf(score, "Score: %d", snake->length);
    } else {
        sprintf(score, "Score:");
    }
    DrawText(score, 15, 15, SCORE_FONT_SIZE, RAYWHITE);

    char high_score_str[20];
    sprintf(high_score_str, "High Score: %d", high_score);
    DrawText(high_score_str, WINDOW_WIDTH - 175, 15, SCORE_FONT_SIZE, RAYWHITE);
}

/* Initialize the board to all EMPTY cells */
static void init_board(void) {
    for (int x = 0; x < COLS; x++) {
        for (int y = 0; y < ROWS; y++) {
            board[x][y] = (Cell) {.x = x, .y = y, .type = EMPTY};
        }
    }
}

/* Initialize the snake by allocating memory and starting location */
static void init_snake(void) {
    snake = malloc(sizeof(Snake));
    snake->head = malloc(sizeof(SnakeCell));
    snake->tail = snake->head;
    snake->head->x = 0;
    snake->head->y = 0;
    snake->head->direction = RIGHT;
    snake->length = 1;
    board[0][0].type = SNAKE_HEAD;
}

/* Draw snake and apple (if applicable) on board */
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

            DrawRectangle(c.x * CELL_WIDTH, c.y * CELL_WIDTH + HEADER_HEIGHT, CELL_WIDTH, CELL_WIDTH, BLACK);
            DrawRectangle(c.x * CELL_WIDTH + 1, c.y * CELL_WIDTH - 1 + HEADER_HEIGHT, CELL_WIDTH - 2, CELL_WIDTH - 2, color);
        }
    }
}

/* Check for key down to change direction */
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

/* Move the snake according to the current direction */
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
    while (cell) { /* Update the rest of the snake */
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

/* Update high score if necessary and change from game screen to game over screen */
static void game_over(void) {
    if (snake->length > high_score) {
        high_score = snake->length;
    }
    screen = GAMEOVER;
}

/* Draw apple in random cell not occupied by the snake */
static void draw_apple(void) {
    int rand_x, rand_y;
    do {
        rand_x = rand() % COLS;
        rand_y = rand() % ROWS;
    } while (board[rand_x][rand_y].type != EMPTY);

    board[rand_x][rand_y].type = APPLE;
    apple_exists = 1;
}

/* Increase length of snake after it eats an apple */
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
    snake->length++;
}

/* Start screen for game. Draw title and instructions for how to start game */
static void draw_start_screen(void) {
    DrawText("SNAKE", (WINDOW_WIDTH / 2) - 115, (WINDOW_HEIGHT / 2) - 150, 70, GREEN);
    DrawText("Press [Enter] to play", (WINDOW_WIDTH / 2) - 165, (WINDOW_HEIGHT / 2) - 50, 30, RAYWHITE);
}

/* Game over screen. Draw "GAME OVER" and instructions for how to restart game */
static void draw_game_over_screen(void) {
    DrawText("GAME OVER", (WINDOW_WIDTH / 2) - 215, (WINDOW_HEIGHT / 2) - 150, 70, GREEN);
    DrawText("Press [Enter] to play", (WINDOW_WIDTH / 2) - 165, (WINDOW_HEIGHT / 2) - 50, 30, RAYWHITE);
}

/* Set all board cells to EMPTY and free old allocate memory from snake*/
static void reset_game(void) {
    /* Clear all board cells */
    for (int x = 0; x < COLS; x++) {
        for (int y = 0; y < ROWS; y++) {
            board[x][y].type = EMPTY;
        }
    }

    /* Free snake memory */
    SnakeCell *curr = snake->head;
    SnakeCell *to_delete;

    while (curr) {
        to_delete = curr;
        curr = curr->next;
        free(to_delete);
    }
    free(snake);
    init_snake();

    apple_exists = 0;
}

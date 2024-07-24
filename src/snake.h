typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct SnakeCell {
    Vector2 position;
    Direction direction;
    struct SnakeCell *next;
} SnakeCell;

typedef struct {
    int size;
    Vector2 shape;
    SnakeCell *head;
} Snake;

void draw_snake(Snake *snake);

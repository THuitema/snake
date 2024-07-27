#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define ROWS 15 // 25
#define COLS 25 // 15
#define CELL_WIDTH WINDOW_WIDTH / COLS
#define FPS 60

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction; 

typedef enum {
    SNAKE_HEAD,
    SNAKE_BODY,
    APPLE,
    EMPTY
} CellState;

typedef struct {
    int x;
    int y;
    CellState type;
} Cell;

typedef struct SnakeCell {
    int x;
    int y;
    Direction direction;
    struct SnakeCell *next;
} SnakeCell;

typedef struct {
    SnakeCell *head;
} Snake;
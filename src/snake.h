typedef struct SnakeCell {
    struct SnakeCell *next;
} SnakeCell;

typedef struct {
    SnakeCell *head;
} Snake;

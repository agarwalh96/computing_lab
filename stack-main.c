#include "common.h"

#define INITIAL_STACK_SIZE 10

typedef int DATA;
/* Some other possibilities:
 * typedef char *DATA;
 * typedef char[BUF_LEN] DATA;
 */

typedef struct {
    unsigned int capacity, top;
    DATA *contents;
} STACK;

/* Prototypes: please provide definitions in stack.c, #include-d below */
STACK init_stack();
void push(STACK *s, DATA element);
DATA pop(STACK *s);
void print_stack(STACK *s);

#include "stack.c"

#define MAX_OPS 100

int main(int ac, char *av[])
{
    int i, x;
    STACK s;

    s = init_stack();

    // srand((int) time(NULL));
    for (i = 0; i < MAX_OPS; i++) {
        if (rand() % 2) {
            x = rand() % MAX_OPS + 1;
            printf("Pushing %d\n", x);
            push(&s, x);
            print_stack(&s);
        }
        else {
            x = pop(&s);
            if (x > 0) {
                printf("Popped %d\n", x);
                print_stack(&s);
            }
        }
    }

    /* write a delete_stack in stack.c and call it here to prevent a
     * memory leak */
    return 0;
}

#include "common.h"

#define INITIAL_Q_SIZE 10

typedef int DATA;
/* Some other possibilities:
 * typedef char *DATA;
 * typedef char[BUF_LEN] DATA;
 */

typedef struct {
    unsigned int capacity, head, tail;
    DATA *contents;
} QUEUE;

/* Prototypes: please provide definitions in queue.c, #include-d below */
QUEUE init_queue();
void enqueue(QUEUE *q, DATA element);
DATA dequeue(QUEUE *q);
void print_queue(QUEUE *q);

#include "queue.c"

#define MAX_OPS 100

int main(int ac, char *av[])
{
    int i, x;
    QUEUE q;

    q = init_queue();

    // srand((int) time(NULL));
    for (i = 0; i < MAX_OPS; i++) {
        if (rand() % 2) {
            x = rand() % MAX_OPS + 1;
            printf("Enqueue %d\n", x);
            enqueue(&q, x);
            print_queue(&q);
        }
        else {
            x = dequeue(&q);
            if (x > 0) {
                printf("Dequeue %d\n", x);
                print_queue(&q);
            }
        }
    }

    /* write a delete_queue in queue.c and call it here to prevent a
     * memory leak */
    return 0;
}

#ifndef QUE_H
#define QUE_H
#include <common.h>

struct node
{
    char *msg;
    struct node *next;
};

struct queue
{
    struct node *front;
    struct node *rear;
    pthread_mutex_t lock;
    int msg_cnt;
};

struct queue *queue_init(void);
struct node *node_init(char *data);
void enqueue(struct queue *q, char *data);
char *dequeue(struct queue *q);
void queue_print(struct queue *q);
#endif

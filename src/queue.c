
#include "queue.h"
#include "common.h"

struct queue *queue_init(void)
{
    struct queue *temp = malloc(sizeof(struct queue));

    temp->front = NULL;
    temp->rear = NULL;
    pthread_mutex_init(&(temp->lock), NULL);
    temp->msg_cnt = 0;

    return temp;
}

struct node *node_init(char *data)
{
    struct node *temp = malloc(sizeof(struct node));

    temp->msg = strndup(data, strlen(data));
    temp->next = NULL;
    return temp;
}

void enqueue(struct queue *q, char *data)
{
    pthread_mutex_lock(&(q->lock));
    struct node *a_node = node_init(data);

   
    if (q->front == NULL)
    {
        q->front = q->rear = a_node;
    }
    else
    {
        q->rear->next = a_node;
        q->rear = a_node;
    }
    q->msg_cnt += 1; 
    pthread_mutex_unlock(&(q->lock));

}
char *dequeue(struct queue *q)
{
    if (q->msg_cnt == 0)
    {
        return NULL;
    }

    char *data;
    pthread_mutex_lock(&(q->lock));

    if (q->front)
    {
        data = strndup(q->front->msg, strlen(q->front->msg));
        q->front = q->front->next;
        q->msg_cnt -= 1;
    }
    else
    {
        pthread_mutex_unlock(&(q->lock));
        return NULL;
    }
    pthread_mutex_unlock(&(q->lock));
    return data;
}


#ifndef CONN_H
#define CONN_H

#include "common.h"
#include "queue.h"

struct client_info                                                     
{
    int connection;
    pthread_t thread_id;
    char *ip;
    struct client_info *next;
    struct queue *msg_queue;
};

struct con_list
{
    struct client_info *head;
    struct client_info *tail;
    struct queue *msg_queue;
    pthread_mutex_t list_mutex;

};

void list_add(struct con_list *list, struct client_info *client);
struct client_info *client_init(void);
struct con_list *list_init(void);
void list_destroy(struct con_list *list);
void list_print(struct con_list *list);
#endif


#include "common.h"
#include "connection.h"
#include "queue.h"

struct client_info *client_init(void)
{
    struct client_info *client = malloc(sizeof(struct client_info));
    client->msg_queue = NULL;

    return client;
}

struct con_list *list_init(void)
{
    struct con_list *list = malloc(sizeof(struct con_list));
    list->head = NULL;
    list->tail = NULL;
    list->msg_queue = queue_init(); 
    pthread_mutex_init(&(list->list_mutex), NULL);
    return list;

}

void list_destroy(struct con_list *list)
{
    struct client_info *tmp;

    while (list->head)
    {
        tmp = list->head->next;
        free(list->head);
        list->head = tmp;
    }
    free(list);
}

void list_add(struct con_list *list, struct client_info *client)
{
    pthread_mutex_lock(&(list->list_mutex));

    if (!list->head)
    {
        list->head = client;
        list->tail = client;
    }
    else
    {
        list->tail->next = client;
        list->tail = client;
    }
    pthread_mutex_unlock(&(list->list_mutex));

}

void list_print(struct con_list *list)
{
    int x = 1;
    struct client_info *head = list->head;

    while (head)
    {
        printf("CLIENT # %d: %s ", x, head->ip);        
        printf("THREAD ID: %ld\n", head->thread_id);
        head = head->next;
        x += 1;
    }
}

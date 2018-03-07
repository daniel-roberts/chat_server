

#include "networking.h"
#include "server.h"
#include "connection.h"

int main(void)
{

    int sck;

    sck = socket_start("1337");

    if (sck < 0)
    {
        exit(0);
    }
    struct sockaddr_storage storage;
    char addr[INET_ADDRSTRLEN];
    socklen_t storage_sz = sizeof(storage);
    int remote, success;
    pthread_t recv_t, send_t;

    struct con_list *list = list_init();

    while (1)
    {
        remote = accept(sck, (struct sockaddr *)&storage, &storage_sz);

        if (remote < 0)
        {
            continue;
        }

        inet_ntop(storage.ss_family, &((struct sockaddr_in *)&storage)->sin_addr,
                  addr, sizeof(addr));

        printf("[!]: Got connection from %s!\n", addr);

        // store client info
        struct client_info *client = client_init();
        client->connection = remote;
        client->ip = addr; 
        client->msg_queue = list->msg_queue;

        list_add(list, client);
        printf("**********************************\n");
        list_print(list);
        printf("**********************************\n");

        if (!(success = banner_send(client)))
        {
            fprintf(stderr, "[!] Could not communicate with %s!\n", client->ip);
        }
        pthread_create(&recv_t, NULL, &connection_handler, client); 
        pthread_create(&send_t, NULL, &echo_messages, list);

    }
}

void *echo_messages(void *args)
{
    struct con_list *list = args;
    struct client_info *head = list->head;
    char *msg;

    while (1)
    {
        if ((msg = dequeue(list->msg_queue)))
        {
            while (head)
            {
                send(head->connection, msg, strlen(msg), 0);
                head = head->next; 
            }
            head = list->head;
        }
    }
}

void *connection_handler(void *client)
{
    struct client_info *my_client = client;
    char buffer[4096];

    memset(buffer, 0, sizeof(buffer));

    while (recv(my_client->connection, buffer, sizeof(buffer)-1, 0) > 0)
    {
        if (strncmp(buffer, "/exit", 5) == 0)
        {
            break;
        }
        enqueue(my_client->msg_queue, buffer); 
        memset(buffer, 0, sizeof(buffer));
    }
    close(my_client->connection);
    return NULL;
}

int banner_send(struct client_info *client)
{
    ssize_t success = 0;
    char *banner = "WELCOME TO THE ROBERTS CHAT SERVER!\n"
                   "Start chatting, or type /exit to exit!\n"
                   "--------------------------------------\n";

    success = send(client->connection, banner, strlen(banner), 0);

    if (!success)
    {
        return -1;
    }
    return success;

}

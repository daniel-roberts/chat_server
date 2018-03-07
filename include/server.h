

#ifndef SERV_H
#define SERV_H

#include "connection.h"


void *echo_messages(void *args);
void *connection_handler(void *client);
int banner_send(struct client_info *client);

#endif

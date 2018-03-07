
#include "networking.h"

int socket_start(char *port)
{
    /*
        Creates a socket and listens on specified port.
    */
    int err, socket_fd, success;

    struct addrinfo *srv = {0}, hints = {0};

    // IPV4, TCP
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    // get info for socket
    if ((err = getaddrinfo(NULL, port, &hints, &srv)))
    {
        fprintf(stderr, "Error: %s\n", gai_strerror(err));
        return -1;
    }


    while (srv)
    {
        socket_fd = socket(srv->ai_family, srv->ai_socktype, srv->ai_protocol);
        if ((success = bind(socket_fd, srv->ai_addr, srv->ai_addrlen)) > -1)
        {
            break;
        }
        srv = srv->ai_next;
    }

    freeaddrinfo(srv);
    printf("Starting socket on port: %s\n", port);

    if (success < 0)
    {
        fprintf(stderr, "Could not bind socket!\n");
        return -1;
    }

    int enable = 1;

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(int)) < 0)
    {
        fprintf(stderr, "Failed to set sock opt\n");
        return -1;
    }

    if (listen(socket_fd, 5) < 0)
    {
        fprintf(stderr, "Could not listen on socket!\n");
        return -1; 
    }

    return socket_fd;
}


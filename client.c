#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MSG_LENGTH 100

int main()
{
    char buf[MAX_MSG_LENGTH];
    int k;
    int sock_desc;
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1)
    {
        printf("Error in socket creation");
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(3002);
    k = connect(sock_desc, (struct sockaddr *)&server, sizeof(server));
    if (k == -1)
    {
        printf("Error in connecting to server");
        exit(1);
    }
    while (1)
    {
        // Send message to server
        printf("Enter message to send to server: ");
        fgets(buf, MAX_MSG_LENGTH, stdin);
        if (strncmp(buf, "end", 3) == 0)
            break;
        k = send(sock_desc, buf, strlen(buf), 0);
        if (k == -1)
        {
            printf("Error in sending");
            exit(1);
        }

        // Clear buffer
        memset(&buf, 0, sizeof(buf));

        // Receive message from server
        k = recv(sock_desc, buf, MAX_MSG_LENGTH, 0);
        if (k == -1)
        {
            printf("Error in receiving");
            exit(1);
        }
        buf[k] = '\0';
        printf("Message received from server: %s\n", buf);

        // Clear buffer
        memset(&buf, 0, sizeof(buf));
    }
    close(sock_desc);
    exit(0);
}

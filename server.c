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
    int sock_desc, conn_desc;
    struct sockaddr_in server, client;
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1)
    {
        printf("Error in socket creation");
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(3002);
    k = bind(sock_desc, (struct sockaddr *)&server, sizeof(server));
    if (k == -1)
    {
        printf("Error in binding");
        exit(1);
    }
    k = listen(sock_desc, 20);
    if (k == -1)
    {
        printf("Error in listening");
        exit(1);
    }
    printf("Server listening...\n");
    socklen_t len = sizeof(client);
    conn_desc = accept(sock_desc, (struct sockaddr *)&client, &len);
    if (conn_desc == -1)
    {
        printf("Error in accepting connection");
        exit(1);
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    while (1)
    {
        // Receive message from client
        k = recv(conn_desc, buf, MAX_MSG_LENGTH, 0);
        if (k == -1)
        {
            printf("Error in receiving");
            exit(1);
        }
        buf[k] = '\0';
        printf("Message received from client: %s\n", buf);

        // Send message to client
        printf("Enter message to send to client: ");
        fgets(buf, MAX_MSG_LENGTH, stdin);
        if (strncmp(buf, "end", 3) == 0)
            break;
        k = send(conn_desc, buf, strlen(buf), 0);
        if (k == -1)
        {
            printf("Error in sending");
            exit(1);
        }

        // Clear buffer
        memset(buf, 0, sizeof(buf));
    }
    close(conn_desc);
    close(sock_desc);
    exit(0);
}

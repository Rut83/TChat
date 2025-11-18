#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>

int client_fd;
int server_fd;
char buffer[1024];

void *recv_thread(void *arg) {
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            printf("Client disconnected.\n");
            exit(1);
        }
        printf("\nClient: %s", buffer);
        printf("You: ");  // keeps prompt clean
        fflush(stdout);
    }
    return NULL;
}
void *send_thread(void *arg) {
    while (1) {
        printf("You: ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }
    return NULL;
}



void run_server() {
    struct sockaddr_in server, client;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    listen(server_fd, 1);
    printf("Server started on port 8080\n");

    socklen_t c = sizeof(client);
    client_fd = accept(server_fd, (struct sockaddr*)&client, &c);
    printf("Client connected!\n");

    pthread_t t_recv, t_send;

    pthread_create(&t_recv, NULL, recv_thread, NULL);
    pthread_create(&t_send, NULL, send_thread, NULL);

    pthread_join(t_recv, NULL);
    pthread_join(t_send, NULL);

    close(server_fd);
}


int main() {
    run_server();
    return 0;
}

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sock;
char buffer[1024];

void* recv_thread(void* arg) {
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int b = recv(sock, buffer, sizeof(buffer), 0);
        if (b <= 0) {
            printf("\nDisconnected from server.\n");
            exit(1);
        }
        printf("\nServer: %s", buffer);
        printf("You: ");
        fflush(stdout);
    }
}

void* send_thread(void* arg) {
    while (1) {
        printf("You: ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);
        send(sock, buffer, strlen(buffer), 0);
    }
}

void run_client() {
    char ip[64];
    struct sockaddr_in server;

    printf("Enter server IP: ");
    scanf("%63s", ip);
    getchar();

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (inet_pton(AF_INET, ip, &server.sin_addr) <= 0) {
        printf("Invalid IP.\n");
        return;
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed.\n");
        return;
    }

    printf("Connected!\n");

    pthread_t t_recv, t_send;

    pthread_create(&t_recv, NULL, recv_thread, NULL);
    pthread_create(&t_send, NULL, send_thread, NULL);

    pthread_join(t_recv, NULL);
    pthread_join(t_send, NULL);

    close(sock);
}

int main() {
    run_client();
    return 0;
}

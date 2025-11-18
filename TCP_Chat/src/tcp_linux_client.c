#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void run_client() {
    int sock;
    char ip[64], buffer[1024];
    struct sockaddr_in server;

    printf("Enter server IP: ");
    scanf("%63s", ip);
    getchar();

    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (inet_pton(AF_INET, ip, &server.sin_addr) <= 0) {
        printf("Invalid IP address.\n");
        return;
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Failed to connect.\n");
        return;
    }

    printf("Connected!\n");

    while (1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);
        printf("Server: %s", buffer);
    }
    close(sock);
}
int main() {
    run_client();
    return 0;
}

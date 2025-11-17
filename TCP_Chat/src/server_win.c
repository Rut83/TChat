#define _WIN32_WINNT 0x0501
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

void run_server() {
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server, client;
    char buffer[1024];

    WSAStartup(MAKEWORD(2,2), &wsa);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;  // local IP
    server.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Bind failed.\n");
        return;
    }

    listen(server_fd, 1);

    printf("Server running on port 8080\nWaiting for client...\n");

    int c = sizeof(client);
    client_fd = accept(server_fd, (struct sockaddr*)&client, &c);

    printf("Client connected!\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        int recv_len = recv(client_fd, buffer, sizeof(buffer), 0);
        if (recv_len <= 0) break;

        printf("Client: %s", buffer);

        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }

    closesocket(server_fd);
    WSACleanup();
}

int main() {
    run_server();
    return 0;
}

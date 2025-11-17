#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

void run_win_client() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int sock;
    char ip[64], buffer[1024];
    struct sockaddr_in server;

    printf("Enter server IP: ");
    scanf("%63s", ip);
    getchar();

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    // Fallback for MinGW (inet_pton missing)
    server.sin_addr.s_addr = inet_addr(ip);
    if (server.sin_addr.s_addr == INADDR_NONE) {
        printf("❌ Invalid IP address.\n");
        return;
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("❌ Could not connect.\n");
        return;
    }

    printf("✅ Connected to server!\n");

    while (1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);
        printf("Server: %s", buffer);
    }

    closesocket(sock);
    WSACleanup();
}

int main() {
    run_win_client();
    return 0;
}
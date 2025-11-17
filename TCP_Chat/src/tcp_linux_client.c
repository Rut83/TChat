#ifdef _WIN32
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#endif

void run_client() {
    int sock;
    char ip[64], buffer[1024];
    struct sockaddr_in server;

    printf("Enter server IP: ");
    scanf("%63s", ip);
    getchar();

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    // FIXED FOR WINDOWS
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

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
}

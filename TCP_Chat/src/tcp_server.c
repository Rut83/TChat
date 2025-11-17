#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#endif
void print_local_ip() {
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        // IPv4 only
        if (ifa->ifa_addr->sa_family == AF_INET) {

            // Skip loopback (lo)
            if (strcmp(ifa->ifa_name, "lo") == 0) continue;

            struct sockaddr_in *addr = (struct sockaddr_in*)ifa->ifa_addr;

            // Convert and print
            inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));
            printf("Server IP: %s\n", ip);

            freeifaddrs(ifaddr);
            return;
        }
    }

    freeifaddrs(ifaddr);
    printf("Server IP: Unknown\n");
}

void run_server() {
    int server_fd, client_fd;
    char buffer[1024];
    struct sockaddr_in server, client;

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    listen(server_fd, 1);
    print_local_ip();
    printf("Server started on port 8080. Waiting for client...\n");

#ifdef _WIN32
    int c = sizeof(client);
#else
    socklen_t c = sizeof(client);
#endif

    client_fd = accept(server_fd, (struct sockaddr*)&client, &c);
    printf("Client connected!\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(client_fd, buffer, sizeof(buffer), 0);
        printf("Client: %s", buffer);

        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }

#ifdef _WIN32
    closesocket(server_fd);
    WSACleanup();
#else
    close(server_fd);
#endif
}

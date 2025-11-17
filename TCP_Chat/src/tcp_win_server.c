#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

void print_local_ip() {
    char hostname[256];
    char ipstr[INET_ADDRSTRLEN];
    struct addrinfo hints, *res, *p;

    gethostname(hostname, sizeof(hostname));

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;   // IPv4 only
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        printf("Server IP: Unknown\n");
        return;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        struct sockaddr_in *addr = (struct sockaddr_in*)p->ai_addr;
        inet_ntop(AF_INET, &addr->sin_addr, ipstr, sizeof(ipstr));
        printf("Server IP: %s\n", ipstr);
        break;
    }

    freeaddrinfo(res);
}

void run_win_server() {
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server, client;
    char buffer[1024];

    WSAStartup(MAKEWORD(2,2), &wsa);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    listen(server_fd, 1);

    print_local_ip();
    printf("Server started on port 8080. Waiting for client...\n");

    int c = sizeof(client);
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

    closesocket(server_fd);
    WSACleanup();
}

int main() {
    run_win_server();
    return 0;
}

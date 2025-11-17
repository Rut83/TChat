#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main() {
    struct sockaddr_rc addr = { 0 };
    int sock;
    char dest[18] = "D0:65:78:32:D4:5C";   // ← PUT SERVER MAC HERE
                                           // Example: "74:45:CE:12:AB:90"

    // Create RFCO  MM socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sock < 0) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Setup connection parameters
    addr.rc_family = AF_BLUETOOTH;
    str2ba(dest, &addr.rc_bdaddr);
    addr.rc_channel = (uint8_t)1;   // channel must match server
    std::cout << "Connecting to server " << dest << "...\n";
    
    // Connect to server
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Connection failed\n";
        close(sock);
        return 1;
    }
    // int s =accept(sock,NULL,NULL);
    std::cout << "Connected!\n";

    // Send a message
    const char msg[] = "Hello from client";
    write(sock, msg, strlen(msg));
    char *m;
    m=(char *)malloc(1024);
    while(1){
        std::cin>>m;
        if (sizeof(m)>0)
        {
            // send(s,m,sizeof(m),0);
            write(sock,m,sizeof(m));
        }
        char buf[1024] = {0};
        int bytes_read = read(sock, buf, sizeof(buf));
        if (bytes_read > 0) {
            std::cout << "Received: " << buf << std::endl;
        }
    }
    // Read the server reply

    // Close socket
    close(sock);
    return 0;
}

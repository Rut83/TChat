#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main() {
    int dev_id = hci_get_route(NULL);
    int sock = hci_open_dev(dev_id);

    if (dev_id < 0 || sock < 0) {
        perror("Opening socket");
        return 1;
    }

    // -------------------------------
    // Set scan parameters
    // -------------------------------
    uint8_t scan_param[] = {
        0x01,       // passive scan
        0x10, 0x00, // interval
        0x10, 0x00, // window
        0x00,       // filter: accept all
        0x00        // no duplicates
    };

    if (hci_send_cmd(sock, OGF_LE_CTL, OCF_LE_SET_SCAN_PARAMETERS,
                     sizeof(scan_param), scan_param) < 0)
        perror("Set scan parameters");

    // -------------------------------
    // Enable scanner
    // -------------------------------
    uint8_t scan_enable[] = {0x01, 0x00};  // enable, no dup filtering

    if (hci_send_cmd(sock, OGF_LE_CTL, OCF_LE_SET_SCAN_ENABLE,
                     sizeof(scan_enable), scan_enable) < 0)
        perror("Enable scan");

    printf("Scanning...\n");

    // -------------------------------
    // Read advertising packets
    // -------------------------------
    unsigned char buf[HCI_MAX_EVENT_SIZE];
    int len;

    while (1) {
        len = read(sock, buf, sizeof(buf));
        if (len < 0) continue;

        // LE Advertising Report event = 0x3E
        if (buf[1] == EVT_LE_META_EVENT && buf[3] == 0x02) {
            uint8_t addr_type = buf[7];
            uint8_t *addr = &buf[8];
            uint8_t data_len = buf[14];
            uint8_t *data = &buf[15];

            char mac[18];
            ba2str((bdaddr_t *)addr, mac);

            printf("Device: %s | Data: ", mac);
            for (int i = 0; i < data_len; i++)
                printf("%02X ", data[i]);
            printf("\n");
        }
    }

    return 0;
}

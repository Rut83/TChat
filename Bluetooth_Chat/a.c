#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
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
    uint8_t scan_params[7] = {
        0x01,       // Passive scan
        0x10, 0x00, // Interval
        0x10, 0x00, // Window
        0x00,       // Own address type
        0x00        // Filter: accept all
    };

    if (hci_send_cmd(sock, OGF_LE_CTL, OCF_LE_SET_SCAN_PARAMETERS,
                     sizeof(scan_params), scan_params) < 0) {
        perror("Set scan parameters");
    }

    // -------------------------------
    // Enable scanning
    // -------------------------------
    uint8_t scan_enable[2] = {
        0x01, // Enable
        0x00  // No duplicate filter
    };

    if (hci_send_cmd(sock, OGF_LE_CTL, OCF_LE_SET_SCAN_ENABLE,
                     sizeof(scan_enable), scan_enable) < 0) {
        perror("Enable scan");
    }

    printf("BLE scanner started...\n");

    // -------------------------------
    // Read events
    // -------------------------------
    unsigned char buf[HCI_MAX_EVENT_SIZE];
    int len;

    while (1) {
        len = read(sock, buf, sizeof(buf));
        if (len < 0) continue;

        // LE Meta Event (0x3E)
        if (buf[1] == EVT_LE_META_EVENT) {

            uint8_t subevent = buf[3];
            if (subevent != EVT_LE_ADVERTISING_REPORT)
                continue;

            uint8_t report_type = buf[4];
            uint8_t addr_type = buf[5];

            uint8_t *addr = &buf[6];
            char mac[18];
            ba2str((bdaddr_t *)addr, mac);

            uint8_t data_len = buf[12];
            uint8_t *data = &buf[13];

            printf("Device: %s | Data: ", mac);
            for (int i = 0; i < data_len; i++)
                printf("%02X ", data[i]);
            printf("\n");
        }
    }

    return 0;
}

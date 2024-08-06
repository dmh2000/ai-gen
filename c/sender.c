#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "../can/can.h"

#define CAN_INTERFACE "vcan0"
#define CAN_ID 0x100
#define LOOP_DELAY_US 100000  // 100,000 microseconds = 0.1 seconds (10 Hz)

int main() {
    int sock = open_can_socket(CAN_INTERFACE);
    if (sock < 0) {
        fprintf(stderr, "Failed to open CAN socket\n");
        return 1;
    }

    int32_t count = 0;

    while (1) {
        // Send count to CAN bus
        if (write_can_frame(sock, CAN_ID, (unsigned char*)&count, sizeof(count)) < 0) {
            fprintf(stderr, "Failed to send CAN frame\n");
            break;
        }

        printf("Sent: %d\n", count);

        // Increment count
        count++;

        // Sleep for 0.1 seconds (10 Hz)
        usleep(LOOP_DELAY_US);
    }

    close_can_socket(sock);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../can/can.h"

#define CAN_INTERFACE "vcan0"
#define SEND_INTERVAL_NS 100000000  // 100,000,000 nanoseconds = 0.1 seconds (10 Hz)

int main() {
    int socket_fd;
    unsigned int count = 0;
    can_frame_t frame;

    // Open CAN socket
    socket_fd = open_can_socket(CAN_INTERFACE);
    if (socket_fd < 0) {
        fprintf(stderr, "Failed to open CAN socket\n");
        return 1;
    }

    printf("Sending data on CAN bus...\n");

    while (1) {
        // Prepare CAN frame
        frame.id = 0x123;  // Example CAN ID
        frame.len = sizeof(unsigned int);
        memcpy(frame.data, &count, sizeof(unsigned int));

        // Send CAN frame
        if (write_can_frame(socket_fd, frame) < 0) {
            fprintf(stderr, "Failed to send CAN frame\n");
            break;
        }

        printf("Sent: %u\n", count);

        // Increment count
        count++;

        // Sleep for the specified interval
        struct timespec ts = {0, SEND_INTERVAL_NS};
        nanosleep(&ts, NULL);
    }

    // Close CAN socket
    close_can_socket(socket_fd);

    return 0;
}

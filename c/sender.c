#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "../can/can.h"

// Define constants for the CAN interface, message ID, and loop delay
#define CAN_INTERFACE "vcan0"
#define CAN_ID 0x100
#define LOOP_DELAY_NS 100000000  // 100,000,000 nanoseconds = 0.1 seconds (10 Hz)

int main() {
    // Open a CAN socket on the specified interface
    int sock = open_can_socket(CAN_INTERFACE);
    if (sock < 0) {
        fprintf(stderr, "Failed to open CAN socket\n");
        return 1;
    }

    // Initialize a 32-bit counter
    int32_t count = 0;

    // Infinite loop to continuously send CAN messages
    while (1) {
        // Send the current count value as a CAN frame
        if (write_can_frame(sock, CAN_ID, (unsigned char*)&count, sizeof(count)) < 0) {
            fprintf(stderr, "Failed to send CAN frame\n");
            break;
        }

        // Print the sent value to stdout
        printf("Sent: %d\n", count);

        // Increment the counter
        count++;

        // Sleep for 0.1 seconds (10 Hz) using nanosleep
        struct timespec ts = {0, LOOP_DELAY_NS};
        nanosleep(&ts, NULL);
    }

    // Close the CAN socket before exiting
    close_can_socket(sock);
    return 0;
}

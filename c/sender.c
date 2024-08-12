#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include "../can/can.h"
#include "config.h"

// Function to log errors
void log_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

int main(int argc, char *argv[]) {
    const char *interface = CAN_INTERFACE;
    int can_id = CAN_ID;
    int delay_ns = LOOP_DELAY_NS;

    // Parse command-line arguments for custom configuration
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            interface = argv[++i];
        } else if (strcmp(argv[i], "-id") == 0 && i + 1 < argc) {
            can_id = (int)strtol(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            delay_ns = atoi(argv[++i]);
        }
    }

    // Open a CAN socket on the specified interface
    int sock = open_can_socket(interface);
    if (sock < 0) {
        log_error("Failed to open CAN socket");
        return 1;
    }

    // Initialize a 32-bit counter
    int32_t count = 0;

    // Infinite loop to continuously send CAN messages
    while (1) {
        // Send the current count value as a CAN frame
        if (write_can_frame(sock, can_id, (unsigned char*)&count, sizeof(count)) < 0) {
            log_error("Failed to send CAN frame");
            break;
        }

        // Print the sent value to stdout
        printf("Sent: %d (ID: 0x%X)\n", count, can_id);

        // Increment the counter
        count++;

        // Sleep for the specified delay using nanosleep
        struct timespec ts = {0, delay_ns};
        nanosleep(&ts, NULL);
    }

    // Close the CAN socket before exiting
    close_can_socket(sock);
    return 0;
}

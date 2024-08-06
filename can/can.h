#ifndef CAN_H
#define CAN_H

#include <stdint.h>

typedef struct can_frame_t {
    int32_t id;
    int8_t len;
    uint8_t data[8];
} can_frame_t;

// Open a CAN socket
int open_can_socket(const char *ifname);

// Write a CAN frame to the socket
int write_can_frame(int sock, int can_id, const unsigned char *data, int data_length);

// Read a CAN frame from the socket
int read_can_frame(int sock, struct can_frame_t *frame);

// Close the CAN socket
void close_can_socket(int sock);

#endif // CAN_H

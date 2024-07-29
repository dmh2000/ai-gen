#ifndef CAN_H
#define CAN_H

#include <stdint.h>

// create a portable can frame struct
// don't use can_frame because its defined in system can.h
typedef struct can_frame_t {
    int32_t id;
    int8_t len;
    uint8_t data[8];
} can_frame_t;

// Open a CAN socket
int open_can_socket(const char *ifname);

// Write a CAN frame to the socket
int write_can_frame(int socket_fd, can_frame_t frame);

// Read a CAN frame from the socket
int read_can_frame(int socket_fd, struct can_frame_t *frame);

// Close the CAN socket
void close_can_socket(int socket_fd);

#endif // CAN_H

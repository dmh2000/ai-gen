#ifndef CAN_H
#define CAN_H

#include <linux/can.h>

// Open a CAN socket
int open_can_socket(const char *ifname);

// Write a CAN frame to the socket
int write_can_frame(int socket_fd, int can_id, const unsigned char *data, int data_length);

// Read a CAN frame from the socket
int read_can_frame(int socket_fd, struct can_frame *frame);

// Close the CAN socket
void close_can_socket(int socket_fd);

#endif // CAN_H

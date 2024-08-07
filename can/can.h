#ifndef CAN_H
#define CAN_H

#include <linux/can.h>

int open_can_socket(const char *interface);
int write_can_socket(int sock, struct can_frame *frame);
int read_can_socket(int sock, struct can_frame *frame);
int close_can_socket(int sock);

#endif  // CAN_H

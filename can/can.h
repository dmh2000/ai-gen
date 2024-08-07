#ifndef CAN_H
#define CAN_H

#include <stdint.h>

typedef struct can_frame_t {
    uint8_t id;
    uint8_t len;
    uint8_t data[8];
} can_frame_t;

int open_can_socket(const char *interface);
int write_can_socket(int sock, struct can_frame_t *frame);
int read_can_socket(int sock, struct can_frame_t *frame);
int close_can_socket(int sock);

#endif  // CAN_H

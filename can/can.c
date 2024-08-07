#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "can.h"

// Function to open a CAN socket
int open_can_socket(const char *interface) {
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, interface);
    ioctl(sock, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }

    return sock;
}

// Function to write to a CAN socket
int write_can_socket(int sock, struct can_frame_t *frame) {
    struct can_frame can_frame;
    can_frame.can_id = frame->id;
    can_frame.can_dlc = frame->len;
    memcpy(can_frame.data, frame->data, 8);
    int bytes_written = write(sock, &can_frame, sizeof(struct can_frame));
    if (bytes_written < 0) {
        perror("write");
        return -1;
    }
    return bytes_written;
}

// Function to read from a CAN socket
int read_can_socket(int sock, struct can_frame_t *frame) {
    struct can_frame can_frame;
    int bytes_read = read(sock, &can_frame, sizeof(struct can_frame));
    if (bytes_read < 0) {
        perror("read");
        return -1;
    }
    frame->id = can_frame.can_id;
    frame->len = can_frame.can_dlc;
    memcpy(frame->data, can_frame.data, 8);
    return bytes_read;
}

// Function to close a CAN socket
int close_can_socket(int sock) {
    return close(sock);
}

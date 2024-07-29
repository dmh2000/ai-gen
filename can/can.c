#include "can.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/if.h>
#include <stdint.h>

int open_can_socket(const char *ifname) {
    int socket_fd;
    struct sockaddr_can addr;
    struct ifreq ifr;

    // Create a socket
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Get the interface index
    strcpy(ifr.ifr_name, ifname);
    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("Error getting interface index");
        close(socket_fd);
        return -1;
    }

    // Bind the socket to the CAN interface
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error binding socket to CAN interface");
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

int write_can_frame(int socket_fd, can_frame_t frame) {
    struct can_frame linux_frame;
    
    // Prepare the CAN frame
    linux_frame.can_id = frame.id;
    linux_frame.can_dlc = frame.len;
    memcpy(linux_frame.data, frame.data, frame.len);

    // Write the frame to the socket
    if (write(socket_fd, &linux_frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        perror("Error writing CAN frame");
        return -1;
    }

    return 0;
}

int read_can_frame(int socket_fd, can_frame_t *frame) {
    struct can_frame linux_frame;
    int nbytes = read(socket_fd, &linux_frame, sizeof(struct can_frame));
    
    if (nbytes < 0) {
        perror("Error reading CAN frame");
        return -1;
    }

    if (nbytes < (int)sizeof(struct can_frame)) {
        fprintf(stderr, "Incomplete CAN frame\n");
        return -1;
    }

    // Convert linux_frame to can_frame_t
    frame->id = linux_frame.can_id;
    frame->len = linux_frame.can_dlc;
    memcpy(frame->data, linux_frame.data, linux_frame.can_dlc);

    return 0;
}

void close_can_socket(int socket_fd) {
    close(socket_fd);
}

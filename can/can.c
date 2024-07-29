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

int write_can_frame(int socket_fd, int can_id, const unsigned char *data, int data_length) {
    struct can_frame frame;
    
    // Prepare the CAN frame
    frame.can_id = can_id;
    frame.can_dlc = data_length;
    memcpy(frame.data, data, data_length);

    // Write the frame to the socket
    if (write(socket_fd, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        perror("Error writing CAN frame");
        return -1;
    }

    return 0;
}

int read_can_frame(int socket_fd, struct can_frame *frame) {
    int nbytes = read(socket_fd, frame, sizeof(struct can_frame));
    
    if (nbytes < 0) {
        perror("Error reading CAN frame");
        return -1;
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "Incomplete CAN frame\n");
        return -1;
    }

    return 0;
}

void close_can_socket(int socket_fd) {
    close(socket_fd);
}

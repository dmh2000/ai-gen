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
    int sock;
    struct sockaddr_can addr;
    struct ifreq ifr;

    // Create the socket
    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Get the interface index
    strcpy(ifr.ifr_name, ifname);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("Error getting interface index");
        close(sock);
        return -1;
    }

    // Bind the socket to the CAN interface
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error binding socket to CAN interface");
        close(sock);
        return -1;
    }

    return sock;
}

int write_can_frame(int sock, int can_id, const unsigned char *data, int data_length) {
    struct can_frame frame;
    
    // Prepare the CAN frame
    frame.can_id = can_id;
    frame.can_dlc = data_length;
    memcpy(frame.data, data, data_length);

    // Write the frame
    if (write(sock, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        perror("Error writing CAN frame");
        return -1;
    }

    return 0;
}

int read_can_frame(int sock, struct can_frame *frame) {
    int nbytes = read(sock, frame, sizeof(struct can_frame));
    
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

void close_can_socket(int sock) {
    close(sock);
}

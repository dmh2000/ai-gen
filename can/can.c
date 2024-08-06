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

/**
 * Opens a CAN socket on the specified interface.
 *
 * @param ifname The name of the CAN interface (e.g., "vcan0")
 * @return The socket file descriptor on success, -1 on failure
 */
int open_can_socket(const char *ifname) {
    int sock;
    struct sockaddr_can addr;
    struct ifreq ifr;

    // Create a raw CAN socket
    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Get the interface index for the specified CAN interface
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

/**
 * Writes a CAN frame to the specified socket.
 *
 * @param sock The CAN socket file descriptor
 * @param can_id The CAN ID for the frame
 * @param data Pointer to the data to be sent
 * @param data_length Length of the data (max 8 bytes)
 * @return 0 on success, -1 on failure
 */
int write_can_frame(int sock, int can_id, const unsigned char *data, int data_length) {
    struct can_frame frame;
    
    // Prepare the CAN frame
    frame.can_id = can_id;
    frame.can_dlc = data_length;
    memcpy(frame.data, data, data_length);

    // Write the frame to the socket
    if (write(sock, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        perror("Error writing CAN frame");
        return -1;
    }

    return 0;
}

/**
 * Reads a CAN frame from the specified socket.
 *
 * @param sock The CAN socket file descriptor
 * @param frame Pointer to a can_frame_t struct to store the received frame
 * @return 0 on success, -1 on failure
 */
int read_can_frame(int sock, struct can_frame_t *frame) {
    struct can_frame kernel_frame;
    int nbytes = read(sock, &kernel_frame, sizeof(struct can_frame));
    
    if (nbytes < 0) {
        perror("Error reading CAN frame");
        return -1;
    }

    if ((size_t)nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "Incomplete CAN frame\n");
        return -1;
    }

    // Convert kernel_frame to our custom can_frame_t
    frame->id = kernel_frame.can_id;
    frame->len = kernel_frame.can_dlc;
    memcpy(frame->data, kernel_frame.data, kernel_frame.can_dlc);

    return 0;
}

/**
 * Closes the specified CAN socket.
 *
 * @param sock The CAN socket file descriptor to close
 */
void close_can_socket(int sock) {
    close(sock);
}

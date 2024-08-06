#ifndef CAN_H
#define CAN_H

#include <stdint.h>

typedef struct can_frame_t {
    int32_t id;
    int8_t len;
    uint8_t data[8];
} can_frame_t;

/**
 * Opens a CAN socket on the specified interface.
 *
 * @param ifname The name of the CAN interface (e.g., "vcan0")
 * @return The socket file descriptor on success, -1 on failure
 */
int open_can_socket(const char *ifname);

/**
 * Writes a CAN frame to the specified socket.
 *
 * @param sock The CAN socket file descriptor
 * @param can_id The CAN ID for the frame
 * @param data Pointer to the data to be sent
 * @param data_length Length of the data (max 8 bytes)
 * @return 0 on success, -1 on failure
 */
int write_can_frame(int sock, int can_id, const unsigned char *data, int data_length);

/**
 * Reads a CAN frame from the specified socket.
 *
 * @param sock The CAN socket file descriptor
 * @param frame Pointer to a can_frame_t struct to store the received frame
 * @return 0 on success, -1 on failure
 */
int read_can_frame(int sock, struct can_frame_t *frame);

/**
 * Closes the specified CAN socket.
 *
 * @param sock The CAN socket file descriptor to close
 */
void close_can_socket(int sock);

#endif // CAN_H

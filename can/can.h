#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>

#define CAN_MAX_DLEN 8
#define CAN_MAX_ID 0x1FFFFFFF

typedef struct {
    uint32_t id;
    uint8_t len;
    uint8_t data[CAN_MAX_DLEN];
    bool is_extended;
    bool is_rtr;
    bool is_error;
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
 * @param frame Pointer to the can_frame_t struct containing the frame to be sent
 * @return 0 on success, -1 on failure
 */
int write_can_frame(int sock, const can_frame_t *frame);

/**
 * Reads a CAN frame from the specified socket.
 *
 * @param sock The CAN socket file descriptor
 * @param frame Pointer to a can_frame_t struct to store the received frame
 * @return 0 on success, -1 on failure
 */
int read_can_frame(int sock, can_frame_t *frame);

/**
 * Closes the specified CAN socket.
 *
 * @param sock The CAN socket file descriptor to close
 */
void close_can_socket(int sock);

/**
 * Sets CAN bus bitrate.
 *
 * @param sock The CAN socket file descriptor
 * @param bitrate The desired bitrate in bits per second
 * @return 0 on success, -1 on failure
 */
int set_can_bitrate(int sock, int bitrate);

/**
 * Enables or disables CAN FD mode.
 *
 * @param sock The CAN socket file descriptor
 * @param enable true to enable CAN FD, false to disable
 * @return 0 on success, -1 on failure
 */
int set_can_fd_mode(int sock, bool enable);

#endif // CAN_H

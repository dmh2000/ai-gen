#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "can.h"

int main() {
    int sock = open_can_socket("can0");
    if (sock < 0) {
        perror("open_can_socket");
        return 1;
    }

    int32_t count = 0;
    struct can_frame_t frame;
    frame.id = 0x100;
    frame.len = 4;

    while (1) {
        memcpy(frame.data, &count, 4);
        if (write_can_socket(sock, &frame) < 0) {
            perror("write_can_socket");
            break;
        }
        count++;
        usleep(100000); // 10 Hz
    }

    close_can_socket(sock);
    return 0;
}

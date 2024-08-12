#ifndef CONFIG_H
#define CONFIG_H

// Define constants for the CAN interface, message ID, and loop delay
#define CAN_INTERFACE "vcan0"
#define CAN_ID 0x100
#define LOOP_DELAY_NS 100000000  // 100,000,000 nanoseconds = 0.1 seconds (10 Hz)

#endif // CONFIG_H
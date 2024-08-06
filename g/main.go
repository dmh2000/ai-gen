package main

/*
#cgo CFLAGS: -I../can
#cgo LDFLAGS: -L../can -lcan
#include "can.h"
#include <stdio.h>
#include <stdlib.h>
*/
import "C"
import (
	"fmt"
	"unsafe"
)

func main() {
	// Create a C string for the CAN interface name
	canInterface := C.CString("vcan0")
	// Ensure the C string is freed when we're done
	defer C.free(unsafe.Pointer(canInterface))

	// Open a CAN socket using the C function from libcan
	sock := C.open_can_socket(canInterface)
	if sock < 0 {
		fmt.Println("Failed to open CAN socket")
		return
	}
	// Ensure the socket is closed when we're done
	defer C.close_can_socket(sock)

	fmt.Println("Listening for CAN messages...")

	// Infinite loop to continuously read CAN frames
	for {
		var frame C.struct_can_frame_t
		// Read a CAN frame using the C function from libcan
		result := C.read_can_frame(sock, &frame)
		if result < 0 {
			fmt.Println("Error reading CAN frame")
			continue
		}

		// Check if the frame ID matches our expected ID (0x100)
		if frame.id == 0x100 {
			// Decode the 32-bit integer from the frame data
			// Assuming little-endian format
			count := int32(frame.data[0]) |
				int32(frame.data[1])<<8 |
				int32(frame.data[2])<<16 |
				int32(frame.data[3])<<24
			fmt.Printf("Received: %d\n", count)
		}
	}
}

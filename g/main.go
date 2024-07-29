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

const (
	canInterface = "vcan0"
	canID        = 0x123
)

func main() {
	// Open CAN socket
	socketFd := C.open_can_socket(C.CString(canInterface))
	if socketFd < 0 {
		fmt.Println("Failed to open CAN socket")
		return
	}
	defer C.close_can_socket(C.int(socketFd))

	fmt.Printf("Listening for CAN messages with ID 0x%X on %s\n", canID, canInterface)

	for {
		var frame C.can_frame_t
		result := C.read_can_frame(C.int(socketFd), &frame)

		if result < 0 {
			fmt.Println("Error reading CAN frame")
			continue
		}

		if frame.id == C.int32_t(canID) {
			fmt.Printf("Received CAN frame: ID=0x%X, Length=%d, Data=", frame.id, frame.len)
			data := (*[8]byte)(unsafe.Pointer(&frame.data[0]))
			for i := 0; i < int(frame.len); i++ {
				fmt.Printf("%02X ", data[i])
			}
			fmt.Println()
		}
	}
}

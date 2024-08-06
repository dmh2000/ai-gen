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
	canInterface := C.CString("vcan0")
	defer C.free(unsafe.Pointer(canInterface))

	sock := C.open_can_socket(canInterface)
	if sock < 0 {
		fmt.Println("Failed to open CAN socket")
		return
	}
	defer C.close_can_socket(sock)

	fmt.Println("Listening for CAN messages...")

	for {
		var frame C.struct_can_frame_t
		result := C.read_can_frame(sock, &frame)
		if result < 0 {
			fmt.Println("Error reading CAN frame")
			continue
		}

		if frame.id == 0x100 {
			count := *(*int32)(unsafe.Pointer(&frame.data[0]))
			fmt.Printf("Received: %d\n", count)
		}
	}
}

# Code Generation with Anthropic and Aider-chat

## SETUP

### Clone starter repo from github

```sh
>git clone 
```

### CONDA
Conda is a command line utility that lets you set up 'virtual environments' that include specific software dependencies that you can for a particular application. Its mostly used with Python.
Its not the same as docker. Example: you want to test code with a particular version of Python. Use a conda virtual environment.

- https://docs.conda.io/projects/conda/en/latest/user-guide/install/linux.html
```sh
>conda create -n <venv-name> --prefix <path> python=3 
>conda activate <venv name>
```

### VCAN
Linux 'virtual CAN' creates a local CAN bus that can be used to test offline software using a CAN bus
- CAN is a bus architecture
- There are not addresses, only messages
- each message has an id
- various clients can send and/or receive messages
- CAN has a cool way of priotizing messages by message id
- the 'ip' command is used for creating virtual network interfaces

- https://linuxconfig.org/configuring-virtual-network-interfaces-in-linux
- https://www.pragmaticlinux.com/2021/10/how-to-create-a-virtual-can-interface-on-linux/

```sh
#!/bin/sh

# check that the virtual CAN interface is loaded and ready
# ifconfig vcan0
# Load the kernel module.
sudo modprobe vcan
# Create the virtual CAN interface.
sudo ip link add dev vcan0 type vcan
# Bring the virtual CAN interface online.
sudo ip link set up vcan0
# Check its ok
sudo ip link show vcan0
ifconfig vcan0
```

### Start of Aider conversation

>aider

#### create a library source for interfacing to VCAN

`using the C language, implement functions to access a CAN bus using network sockets. The file should include functions that open, write,read and close a CAN socket. Write the file to can/can.c`
 

#### ERROR fix : ifreq ifr not defined

`add the file 'linux/if.h to can/can.c`


#### create the include file for can

`create a file can/can.h that exports the functions from can/can.c`

#### MANUAL FIX

I didn't like having to include the system file can.h in client code so manually modify it to have its own definiton of can_frame.

`using the updated file canlib/canlib.h as a model, modify canlib/canlib.c to match the function prototypes defined there`

#### build the library

`create a Makefile in can that compiles can/can.c and builds a static library`

#### check the Makefile works

- cd can
- Make

#### WARNING type mismatch

```text 
canlib.c:71:16: warning: comparison of integer expressions of different signedness: ‘int’ and ‘long unsigned int’ [-Wsign-compare]
   71 |     if (nbytes < sizeof(struct can_frame)) {
      |                ^
```      

`in can/can.c change line 71 to add a cast (int)sizeof(struct can_frame)`

Rerun the Makefile

- make (should build without warnigns)

#### create a can bus client in directory 'c'. 

`in directory 'c' create a file named sender.c. create a main function that opens a can socket. add a variable named 'count' initialized to 0. add a loop that sends the contents of the variable count to the can bus socket at 10 Hz. increment count by 1 on each loop iteration`

#### build the client

`add a makefile to directory c that compiles c/sender.c and includes the library libcan.a from directory can`

Teset the Makefile

#### WARNING 'usleep undefined'

`in file sender.c change the call to usleep to use nanosleep. add an argument to c/Makefile CFLAGS to define the macro \_POSIX_C_SOURCE=200000`


#### IN DIRECTORY 'g'

#### create go file
`create a main.go file in directory 'g' and add an empty main function`

#### add code to receive can messages
in the file g/main.go, add code to receive from a can socket using the library can/libcan.h, use cgo for this

#### ERROR 'frame.can_dlc not found'

linux/can.h uses a packaged union for can_frame. cgo doesn't properly handle packed structs/unions. looking at the cgo definition of can_frame, the symbol 'anon0[1]' is in place of the packed union for the data length.

`replace the line "dlc := frame.can_dlc" to "dlc := int(frame.anon[0])"`

#### get rid of unsafe pointers

`in main.go, instead of using ifname as a discrete variable, make the interface name as a global constant with value 'vcan0', and use C.Cstring directly in the call to open_can_socket to void using an unsafe pointer directly `

`in main.go instead of using an unsafe pointer and binary encoding, read the frame dta bytes and conver them to an int using shifts and add. assume the value received will be a 32 bit int  `

#### in file c/sender.c change the CAN_ID to 100

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
# ubuntu : sudo install can-utils

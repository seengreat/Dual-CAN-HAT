"""
Before running this demo program,another CAN 
device connected to this machine needs to run
the receive.py program in advance
The wiring method is H to H,L to L
"""
import os
import can

os.system('sudo ip link set can1 type can bitrate 100000')
os.system('sudo ifconfig can1 up') # up the can1

can1 = can.interface.Bus(channel = 'can1', bustyp = 'socketcan_ctypes')# socketcan_native

msg = can.Message(arbitration_id=0x123, data=[8, 1, 8, 3, 6, 5, 6, 7], is_extended_id=False)
can1.send(msg) # send message

os.system('sudo ifconfig can1 down') #down the can1

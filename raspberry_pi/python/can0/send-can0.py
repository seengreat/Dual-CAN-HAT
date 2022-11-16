"""
Before running this demo program,another CAN 
device connected to this machine needs to run
the receive.py program in advance
The wiring method is H to H,L to L
"""
import os
import can

os.system('sudo ip link set can0 type can bitrate 100000')
os.system('sudo ifconfig can0 up') # up the can0

can0 = can.interface.Bus(channel = 'can0', bustyp = 'socketcan_ctypes')# socketcan_native

msg = can.Message(arbitration_id=0x123, data=[6, 1, 8, 3, 6, 5, 6, 7], is_extended_id=False)
can0.send(msg) # send message

os.system('sudo ifconfig can0 down') #down the can0

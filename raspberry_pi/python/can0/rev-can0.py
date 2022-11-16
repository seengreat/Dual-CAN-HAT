"""
Before another device sends data to the machine,
it needs to run the program first, and then wait
for the data to be received
The wiring method is H to H,L to L
"""
import os
import can

os.system('sudo ip link set can0 type can bitrate 100000')
os.system('sudo ifconfig can0 up') # up the can0

can0 = can.interface.Bus(channel = 'can0', bustyp = 'socketcan_ctypes')# socketcan_native

msg = can0.recv(20.0) # receive timeout is set to 10 seconds
print(msg)
if msg is None:
    print('Timeout occurred, no message.')
os.system('sudo ifconfig can0 down') #down the can0
# Dual-CH Can HAT from seengreat:www.seengreat.com
## Overview
	Dual-CH Can HAT is a professional expansion board specially developed for Raspberry Pi for learning or testing the dual-channel CAN communication function with multiple protections. At the same time, it also leads to the SPI jumper wire pin interface for STM32, Arduino and other host control boards.
## Features
Based on Raspberry Pi design, suitable for Raspberry Pi Zero/Zero W/Zero WH/2B/3B/3B+/4B<br> 
Adopt MCP2515 and ATA6561 dual-chip combination solution, support dual-channel CAN interface communication<br> 
Onboard level conversion circuit, the working level of 3.3V/5V can be switched by slide switch<br> 
Onboard 120Ω terminal resistor, it can be set Enable by slide switch<br> 
Leads out the SPI control interface, for connecting with host control boards like STM32/Arduino<br> 
Provide complete supporting information manuals and demo codes<br> 
## Specifications
|Dimensions	|65mm(length)*56mm(width)|
|----------------------|-------------------------------|
|Input Voltage	|5V|
|Logic Voltage	|3.3V/5V|
|CAN Control Chip	|MCP2515|
|CAN Transceiver	|ATA6561-GAQW-N|

#Usage
##Instructions of Hard Interface Configuration
The level of Raspberry Pi and STM32 logic system is 3.3V, users need to pull the logic voltage control switch S3 of the board to 3.3V when use Dual CAN HAT.<br> 


###STM32 Hardware Connection<br> 
 * Hardware Connection :STM32F103 -> Dual-CH CAN HAT<br> 
 *       CAN0_H -> CAN1_H              CAN0_L -> CAN1_L<br> 
 *          5V  -> 5V                		PB15 -> MOSI<br> 
 *          GND -> GND               	PB14 -> MISO<br> 
 *          PB3 -> INT0             		PB13 -> SCK<br> 
 *          PB4 -> INT1            		PB12 -> CE0<br> 
 *                            			PB5  -> CE1<br> 
###Raspberry Pi Interface Definition<br>
The pin definitions for connecting Dual CAN HAT to Raspberry Pi are shown in Table 2-1 below.<br>
|Function Pin|Raspberry Pi Connector （BCM）|Raspberry Pi Connector（WPI）|Description|
|--------------|------------------------------------|----------------------------------|-------------|
|5V	|5V	|5V	|5V VCC|
|GND	|GND	|GND	|GND|
|MISO	|9(MISO)	|13(MISO)	|SPI Clock Input|
|MOSI	|10(MOSI)	|12(MOSI)	|SPI Data Input|
|SCK	|11(SCLK)	|14(SCLK)	|SPI Data Output|
|CS_0	|8(CE0)	|10(CE0)	|CAN_0 Chip Select|
|INT_0	|25	|6	|CAN_0 Interrupt Output|
|CS_1	|7(CE1)	|11(CE1)	|CAN_1 Chip Select|
|INT_1	|23	|4	|CAN_1 Interrupt Output|

###Hardware Setup
Turn the slide switch to 3v3，the terminal resistance slide switch also to the same setting, and then plug the module into Raspberry Pi.<br>
##Configuration Environment
###Install the Necessary Function Library
####Install wiringPi
sudo apt-get install wiringpi<br>
#Raspberry Pi 4B may need to be upgraded:<br>
wget https://project-downloads.drogon.net/wiringpi-latest.deb<br>
sudo dpkg -i wiringpi-latest.deb<br>
gpio -v<br>
#When gpio -v is running, the version 2.52 will appear. If it does not appear, the installation is wrong.
####Install Python Function Library
#python3
sudo apt-get update<br>
sudo apt-get install python3-pip<br>
sudo apt-get install python3-pil<br>
sudo apt-get install python3-numpy<br>
sudo pip3 install spidev <br>
sudo pip3 install python-can<br>
###Configure SPI for Raspberry Pi
####Enable SPI for Raspberry Pi
Enter the following command to enter the setting interface of the Raspberry Pi and enable the SPI interface.<br>
sudo raspi-config<br>
Select Interfacing Options -> SPI -> Yes to enable the SPI interface<br>
####Load the MCP2515CAN driver into the kernel
Enter the following command to edit the boot configuration file of the Raspberry Pi<br>
sudo nano /boot/config.txt<br>
Add the following three lines of code at the end of the document<br>
dtparam=spi=on<br>
dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25<br>
dtoverlay=mcp2515-can1,oscillator=16000000,interrupt=23<br>
Reboot the Raspberry Pi to apply the changed settings<br>
sudo reboot<br>
####Test whether the configuration is successful
After the Raspberry Pi reboot, check the SPI information<br>
dmesg | grep spi0<br>
The following information appears means the initialization is successful:<br>
"mcp251x spi0.1 can0:MCP2515 successfully initialized."<br>
"mcp251x spi0.0 can1:MCP2515 successfully initialized."<br>

##Test
###Command Line Test
If there is only one Raspberry Pi and Dual-CH Can HAT module, CAN0 and CAN1 can be connected（The H pins of the two can interfaces are connected, and the L pins are connected）.<br>
Firstly, install the can test tool can-utils on the command line:<br>
sudo apt-get install can-utils<br>
Then open the can interface with the following commands:<br>
sudo ifconfig can0 up<br>
sudo ifconfig can1 up<br>
Open two terminals first when carry out transceiver test, and the following test process is to send data from Can1 to can0:<br>
One of the terminal inputs receives CAN0 data command<br>
candump can0<br>
Another terminal input sends CAN1 data command<br>
cansend can1 000#11.22.33.44<br>
The demonstration effect is as follows:<br>
  "can0 123 [4] 01 23 45 67"<br>
  "can0 123 [4] 01 23 45 88"<br>
###Python demo codes
can1 sends data to can0:<br>
First, open two terminals. One terminal is used to run the data receiving code, and the other terminal is used to run the data sending code. Run the data receiving code first and then the data sending code.<br>
cd #Own file directory/dual-can/python/can0<br>
python3 rev-can0.py：<br>
"Timestamp:1657008680.304630   ID:0123 S Rx DL:8    08   01 08 03 06 05 06 07   Channel:can0"<br>
Another terminal input:<br>
cd #Own file directory/dual-can/python/can1<br>
python3 send-can1.py:<br>
#Notice:
If an error similar to the following occurs during the running of the python program:<br>
RTNETLINK answers:Device or resource busy<br>
Traceback (most recent call last):<br>
File “xxxxx.py”, line xx, in<br>
can0 = can.interface.Bus(channel = ‘can0’, bustyp = ‘socketcan_ctypes’)<br>
File “....xxxx.py”, line xxx, in new<br>
kwargs = load_config(config=kwargs, context=context)<br>
File “/usr/local/lib/python3.7/dist-packages/can/util.py”, line 192, in load_config<br>
...<br>
f'Unknown interface type "{config[interface]}"'<br>
can.exceptions.CanInterfaceNotImplementError:Unknown interface type "None"<br>
Solution:<br>
Add the .canrc file under ~/,<br>
sudo nano ~/.canrc<br>
Fill in the following contents:<br>
[default]<br>
interface = socketcan<br>
channel = can0<br>
Exit after saving, reboot and rerun the Python code.<br>
###C demo codes
Execute the following command to move to the program directory<br>
cd #Own file directory/dual-can/c<br>
Then execute the following commands in sequence<br>
sudo make clean<br>
sudo make<br>
sudo ./main<br>
The program runs as shown in the following information:<br>
"S:3<br>
can1 had received a can frame from can0<br>
    id=0x655 dlc=8 Data:0x61 0x00 0x22 0x00 0x00 0x00 0x00 0x00<br>
can0 had received a can frame from can1 <br>
    id=0x656 dlc=8 Data:0x61 0x10 0x11 0x00 0x00 0x00 0x00 0x00"<br>


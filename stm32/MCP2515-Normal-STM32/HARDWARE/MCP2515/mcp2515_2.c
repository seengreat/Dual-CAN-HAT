/***************************************************************************************
 * Project  :MCP2515-Normal-STM32
 * Describe :CAN0 communicates with CAN1
 * Experimental Platform :STM32F103C8T6 + Dual-CH CAN HAT
 * Hardware Connection :STM32F103 -> Dual-CH CAN HAT
 *		 CAN0_H -> CAN1_H			  CAN0_L -> CAN1_L
 *			5V	-> 5V					PB15 -> MOSI
 *			GND	-> GND					PB14 -> MISO
 *			PB3 -> INT0					PB13 -> SCK
 *			PB4 -> INT1					PB12 -> CE0
 *										PB5  -> CE1
 * Library Version :ST_V3.5
 * Author		   :Christian
 * Web Site		   :www.seengreat.com
***************************************************************************************/
#include "spi.h"
#include "gpio.h"
#include "delay.h"
#include "mcp2515_2.h"

//16M oscillator baud rate
#define CAN_50Kbps	0x13
#define CAN_100Kbps	0x09
#define CAN_125Kbps	0x07
#define CAN_250Kbps	0x03
#define	CAN_500Kbps	0x01

void MCP2515_2_WriteByte(unsigned char addr,unsigned char dat)
{
	CE1 = 0;							//Drive NSS Low
	SPI2_ReadWriteByte(CAN_WRITE);		//Send Write Command
	SPI2_ReadWriteByte(addr);			//Send Address
	SPI2_ReadWriteByte(dat);			//Write Data
	CE1 = 1;							//Drive NSS High
}

unsigned char MCP2515_2_ReadByte(unsigned char addr)
{
	unsigned char rByte;
	CE1 = 0;							//Drive NSS Low
	SPI2_ReadWriteByte(CAN_READ);		//Send Read Command
	SPI2_ReadWriteByte(addr);			//Send Address
	rByte=SPI2_ReadWriteByte(0x00);		//Read Data
	CE1 = 1;							//Drive NSS High
	return rByte;						//Returns one byte of data read
}

void MCP2515_2_Reset(void)
{
	CE1 = 0;                    		//Drive NSS Low
	SPI2_ReadWriteByte(CAN_RESET);		//Sends the register reset command
	CE1 = 1;                    		//Drive NSS High
}

void MCP2515_2_Init(void)
{
	unsigned char temp=0;

	MCP2515_2_Reset();
	delay_ms(1);

	//Set the baud rate to 500Kbps,Sampling Points��75.00%
	MCP2515_2_WriteByte(CNF1,CAN_500Kbps);
	MCP2515_2_WriteByte(CNF2,0x8A);
	MCP2515_2_WriteByte(CNF3,0X01);
	
	MCP2515_2_WriteByte(TXB0SIDH,0xFF);
	MCP2515_2_WriteByte(TXB0SIDL,0xE0);
	
	MCP2515_2_WriteByte(RXB0SIDH,0x00);
	MCP2515_2_WriteByte(RXB0SIDL,0x00);
	MCP2515_2_WriteByte(RXB0CTRL,0x20);
	MCP2515_2_WriteByte(RXB0DLC,DLC_8);
	
	MCP2515_2_WriteByte(RXF0SIDH,0xFF);
	MCP2515_2_WriteByte(RXF0SIDL,0xE0);
	MCP2515_2_WriteByte(RXM0SIDH,0xFF);
	MCP2515_2_WriteByte(RXM0SIDL,0xE0);
	
	MCP2515_2_WriteByte(CANINTF,0x00);
	MCP2515_2_WriteByte(CANINTE,0x01);
	
	MCP2515_2_WriteByte(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);
	
	temp=MCP2515_2_ReadByte(CANSTAT);
	if(OPMODE_NORMAL!=(temp&0xE0))
	{
		MCP2515_2_WriteByte(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);
	}
}

void CAN_2_Send_Buffer(unsigned char *CAN_TX_Buf,unsigned char len)
{
	unsigned char j,dly,count;

	count=0;
	while(count<len)
	{
		dly=0;
		while((MCP2515_2_ReadByte(TXB0CTRL)&0x08) && (dly<50))
		{
			delay_ms(1);
			dly++;
		}
				
		for(j=0;j<8;)
		{
			MCP2515_2_WriteByte(TXB0D0+j,CAN_TX_Buf[count++]);
			j++;
			if(count>=len) break;
		}
		MCP2515_2_WriteByte(TXB0DLC,j);
		MCP2515_2_WriteByte(TXB0CTRL,0x08);
	}
}

unsigned char CAN_2_Receive_Buffer(unsigned char *CAN_RX_Buf)
{
	unsigned char i=0,len=0,temp=0;

	temp = MCP2515_2_ReadByte(CANINTF);
	if(temp & 0x01)
	{
		len=MCP2515_2_ReadByte(RXB0DLC);
		while(i<len)
		{
			CAN_RX_Buf[i]=MCP2515_2_ReadByte(RXB0D0+i);
			i++;
		}
	}
	MCP2515_2_WriteByte(CANINTF,0);
	return len;
}


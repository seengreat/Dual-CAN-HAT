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
#include "usart.h"
#include "sys.h"
#include "delay.h"
#include "gpio.h"
#include "exti.h"
#include "mcp2515.h"
#include "mcp2515_2.h"

extern unsigned char int_flag0;
extern unsigned char int_flag1;
u8 CAN0_BUF_SEND[8]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77};
u8 CAN0_BUF_RECEIVE[8];
u8 CAN1_BUF_SEND[8]={0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x88,0x99};
u8 CAN1_BUF_RECEIVE[8];

int main(void)
{
	u8 len,i;
	SystemInit();
	Uart1Init(9600,0,0);
	printf("\r\n---------- Dual-CH CAN HAT ----------\r\n");
	printf("STM32F103C8T6 ");
	printf("V1.0.0 Build 2022/07/12 17:40\r\n");
	EXTIX_Init();
	delay_init();
	MCP2515_Init();
	MCP2515_2_Init();
	while(1)
	{
		printf("\r\nStart sending data\r\n");
		CAN_Send_Buffer(CAN0_BUF_SEND,8);
		CAN_2_Send_Buffer(CAN1_BUF_SEND,8);
		delay_ms(1);
		if(int_flag0)
		{
			int_flag0 = 0;
			len=CAN_Receive_Buffer(CAN0_BUF_RECEIVE);
			if(len)
			{
				printf("CAN0 received data is:");
				for(i=0;i<len;i++)
				{
					printf("%02X ",CAN0_BUF_RECEIVE[i]);
				}
				printf("\r\n");
			}
		}
		if(int_flag1)
		{
			int_flag1 = 0;
			len=CAN_2_Receive_Buffer(CAN1_BUF_RECEIVE);
			if(len)
			{
				printf("CAN1 received data is:");
				for(i=0;i<len;i++)
				{
					printf("%02X ",CAN1_BUF_RECEIVE[i]);
				}
				printf("\r\n");
			}
		}
		delay_ms(1000);
	}
}


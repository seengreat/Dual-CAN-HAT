/***************************************************************************************
 * Project  :MCP2515-LoopBack-STM32
 * Describe :CAN0 is automatically collected in loopback mode
 * Experimental Platform :STM32F103C8T6 + Dual-CH CAN HAT
 * Hardware Connection :STM32F103 -> Dual-CH CAN HAT
 *			5V	-> 5V					PB15 -> MOSI
 *			GND	-> GND					PB14 -> MISO
 *			PB3 -> INT0					PB13 -> SCK
 *										PB12 -> CE0
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

extern unsigned char int_flag;
u8 CAN_BUF_SEND[8]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77};
u8 CAN_BUF_RECEIVE[8];

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
	while(1)
	{
		printf("\r\nStart sending data\r\n");
		CAN_Send_Buffer(CAN_BUF_SEND,8);
		delay_ms(1);
		if(int_flag)
		{
			int_flag = 0;
			len=CAN_Receive_Buffer(CAN_BUF_RECEIVE);
			if(len)
			{
				printf("The received data is:");
				for(i=0;i<len;i++)
				{
					printf("%02X ",CAN_BUF_RECEIVE[i]);
				}
			}
		}
		delay_ms(500);
	}
}


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
#include "gpio.h"
#include "spi.h"

void IO_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI2_Init();
	SPI2_SetSpeed(SPI_BaudRatePrescaler_32);
} 


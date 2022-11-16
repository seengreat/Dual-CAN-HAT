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
#include "exti.h"
#include "gpio.h"
#include "delay.h"

unsigned char int_flag=0;

void EXTIX_Init(void)
{
   	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    IO_Init();

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI3_IRQHandler(void)
{
    if(int_flag == 0)
    {
       int_flag = 1;
    }	
	EXTI_ClearITPendingBit(EXTI_Line3);
}
 

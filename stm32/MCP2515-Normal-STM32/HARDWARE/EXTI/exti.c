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
#include "exti.h"
#include "gpio.h"
#include "delay.h"

unsigned char int_flag0=0;
unsigned char int_flag1=0;

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

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);
	
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure); 
	
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI3_IRQHandler(void)
{
    if(int_flag0 == 0)
    {
       int_flag0 = 1;
    }	
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
    if(int_flag1 == 0)
    {
       int_flag1 = 1;
    }	
	EXTI_ClearITPendingBit(EXTI_Line4);
}


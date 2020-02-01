#include "stm32f10x.h"
#include "led.h"


//初始化PA8和PD2为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				     //LED1-->PD.2 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOD, &GPIO_InitStructure);
 GPIO_SetBits(GPIOD,GPIO_Pin_2);						           //PD.2 输出高

}
 

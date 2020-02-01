#include "stm32f10x.h"
#include "exti.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "movement.h"
#include "usart.h"
#include "motor.h"

u8 exti_flag=0;
u8 movement_tag='Q';


/*外部中断2初始化*/
void Exti_Init(void)
{
	 	EXTI_InitTypeDef EXTI_InitStructure;
	  KEY_Init();   	                                            /*引脚GPIO初始化*/
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	        /*使能复用功能时钟*/
	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);  /*打开外部中断*/
	  EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     /*下降沿触发*/
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	

		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13); 
	  EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     /*下降沿触发*/
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);	

	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
	  EXTI_InitStructure.EXTI_Line=EXTI_Line0;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;     /*上升沿触发*/
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);	
	
    Ex_NVIC_Config();                                        /*外部中断优先级设置*/
	
}

/*外部中断优先级设置*/
void Ex_NVIC_Config(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	    /*抢占优先级2*/ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					  /*子优先级2*/
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								/*使能外部中断通道*/
  	NVIC_Init(&NVIC_InitStructure);	

		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	    /*抢占优先级2*/ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					  /*子优先级2*/
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								/*使能外部中断通道*/
  	NVIC_Init(&NVIC_InitStructure);	
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	    /*抢占优先级2*/ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					  /*子优先级2*/
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								/*使能外部中断通道*/
  	NVIC_Init(&NVIC_InitStructure);	
	
}

/*功能描述：操作的第一步，四个机械手全部松开，好让操作者把魔方放入机械手中
 *备    注：这个按键有两个地方可以用到：1.系统上电之后，舵机的角度很不规则 2.复原魔方结束之后，把魔方拿出来
 *          KEY0
 */
void EXTI1_IRQHandler(void)
{
	delay_ms(10);
	
	if(KEY0==0)
	{
		LED1=!LED1;		
		Init_MotorMovement();
		motor_speed=250;
		change();		
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断	
		TIM_Cmd(TIM3, ENABLE);
	}	
	EXTI_ClearITPendingBit(EXTI_Line1);  
	
}



/*功能描述：转到第一个需要拍照的面，把魔方放入机械手之后按下这个按键
 *备    注：开发板对应KEY1，注意不是EXTI13_IRQHandler
 */
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);
	
	if(KEY1==0)
	{
		LED1=!LED1;		
		PicArray_ToBufferArray(firpic_position,4);
		motor_speed=250;//速度设定
		change();		
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断	
		TIM_Cmd(TIM3, ENABLE);     
		movement_tag='Z';   //动作做完之后需要通知APP拍摄第一张图片
		
	}
	EXTI_ClearITPendingBit(EXTI_Line13); 
  }
	


/*功能描述：四个机械手闭合，抓紧魔方
 *备    注：开发板对应KEY2
 */
void EXTI0_IRQHandler(void)
{
	
	delay_ms(10);
	
	if(KEY2==1)
	{
		LED1=!LED1;
		PicArray_ToBufferArray(firpic_position,2);
		motor_speed=250;
		change();	
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断	
		TIM_Cmd(TIM3, ENABLE);  
		
	}

	EXTI_ClearITPendingBit(EXTI_Line0);  	
}


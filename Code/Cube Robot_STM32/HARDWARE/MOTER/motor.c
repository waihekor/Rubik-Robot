/*  文件名：motor.c
 *文件描述：主要存放舵机数据线引脚GPIO的初始化和舵机速度控制插补算法
 *备    注：change函数n为速度值，经过实际测试，发现当n为50或者更小时，动作会出现错乱
 *因为舵机转动有一定的机械滞后性，转动需要一定的时间，所以n值不能太小，测试
 *中发现当n值为500或者1000时，舵机转动良好
 */
#include "stm32f10x.h"
#include "motor.h"
#include "sys.h"
#include "instruction.h"
#include "movement.h"
#include "usart.h"
#include "exti.h"
#include "timer.h"
#include <math.h>
#include "led.h"

int point_now=-1;    /*正在执行的数组*/
u8 point_aim=0;	     /*正在执行数组的下一行数组*/
u16 n=300;		       /*设定的插补次数*/
u16 motor_speed=300;
//u16 m;		           /*当前的插补次数*/
double dp;
double dp0[8];	  /*插补增量*/
u16 pwm[8]={1510,1475,1550,1505,2065,1770,1945,1745};

u8 end_flag=0;//魔方复原完成标志位
u16 pos[500][8];



/*
 *函 数 名:change()  
 *功能描述:初位置末尾置更替
 *		   有效的数据是插补增量，和插补次数，知道这两个量，和当前初位置即可
 *输    入：无
 *输    出：无
 *调    用：被 vpwm()调用		 
 *备    注：要实现匀角速度，须在每一次进入此函数时对8个舵机赋予不同的插补量，
 */	
 void change(void)
{	
	 u8 j;  	
	
	 if(point_aim==lines_num)	        
   {
		 
				TIM_Cmd(TIM3, DISABLE);  				 
				point_aim=0;
				point_now=-1;
				if(send_start_flag==1)
				{
					end_flag=1;
				}
		    
				switch(movement_tag)
				 {
							 case 'Z':
							 {
								  USART_SendChar('1');
								  TIM_Cmd(TIM4, ENABLE);
								 
							 }break;
							 
							 case 'A':
							 {
								  USART_SendChar('2');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
						 
							 case 'B':
							 {
								  USART_SendChar('3');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
						 
							 case 'C':
							 {
								 USART_SendChar('4');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
						 
							 case 'D':
							 {
								  USART_SendChar('5');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
						 
							 case 'E':
							 {
								  USART_SendChar('6');
								  TIM_Cmd(TIM4, ENABLE);
							 }break;
	
							 default:
							 {
								 
							 }break;		 							 
				 }	
				 
   }
	 
   else
   {
			point_aim++;
			point_now++;	
			for(j=0;j<8;j++)
			{
				 pwm[j]=pos[point_aim][j];
			} 
		 
   }
	 
}


/*
 *函 数 名：pwm[]数组更新函数  
 *功能描述：数据插补，插补时间间隔为20ms/16，由Timer1控制，使舵机平滑实现速度控制
 *          另一个功能是执行完一行后去更新下一行数据，即调用change()
 *输    入：无 
 *输    出：无
 *调    用：被main.c调用
 *备    注：
 */	
void vpwm(void)		 
{				                                                                                                                                                                                                                                                                                                                                                                                                 
	 static u16 m=0;
	
	  m++;
		if(m==motor_speed)
		{	
			m=0;		
			change();			 	  
		}
		
		return;
}


/*功能描述：初始化和信号线连接的GPIO引脚
 *备    注：PA.1--------PA.8
 */
void MotorPin_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   /*使能PinA端口时钟*/
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	/*端口配置，PA0被按键WK_UP占用*/
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   /*推挽输出*/
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   /*IO口速度为50MHz*/
	
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					         /*根据设定参数初始化GPIOA*/
	 GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);						
	
}


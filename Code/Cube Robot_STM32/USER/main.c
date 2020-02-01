#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include "timer.h"
#include "motor.h"
#include "movement.h"
#include "instruction.h"

/*
 *备注：使用芯片型号为:STM32F103RCT6，使用KEIL4
 */
	
 int main(void)
 {
	 
 	SystemInit(); 			   //系统时钟初始化为72M	  SYSCLK_FREQ_72MHz
	delay_init(72);	    	 //延时函数初始化	  
	NVIC_Configuration();  //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	LED_Init();			       //LED端口初始化
	Exti_Init();
  USART1_Config();
	 
	MotorPin_Init();    
	Init_TotalArray();   
  TIM3_Int_Init(100,100);	   //不使能时间开关3，没有使能定时器3中断
	TIM4_Int_Init(29999,7199);  //不使能时间开关4，定时3S


		while(1)
		{                                               
						
				if(rece_flag==1)//接收到指令并且前一个动作已经完成
				{
							TIM_Cmd(TIM3, DISABLE);       
			   
						  if(rece_string[0]=='#')
							{
										motor_speed=250;  
										lines_num=Analy_UsartString();
										end_flag=0;
							}
	
							
							
							//调试，串口发送示例为：@1395%1440%1435%1500%1500%1500%1500%1500!
							else if(rece_string[0]=='@')
							{	
										motor_speed=250;  
										Analy_String(rece_string);
								    lines_num=1;									

					    }
							
							
							//串口发送示例：%1!,%2!,%3！，%4！，%5！，%6！
							else if(rece_string[0]=='%')//调试，调到魔方面朝上拍照时的状态，方便查看此时的舵机角度
							{
								
										motor_speed=250;  
								    Debug_Photo_Angle(rece_string);//已经修改全局变量lines_num

							}
					
				change();		
				TIM_Cmd(TIM3, ENABLE);   
				TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断	
				rece_flag=0;

        }	 
				
				if(flag_vpwm==1)	  
				{
						vpwm();				             
						flag_vpwm=0;
				}	
				
				if(send_start_flag==1&&end_flag==1)
				{
					 USART_SendChar('7');//发送7，让APP通过蓝牙传动解算指令	
					 send_start_flag=0;
					 end_flag=0;
					
				}


    }

}



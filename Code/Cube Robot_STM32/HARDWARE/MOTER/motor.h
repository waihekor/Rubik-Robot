#ifndef ___MOTOR___H____
#define ___MOTOR___H____


#include "stm32f10x.h"

#define PWM1 PAout(1)// PA1
#define PWM2 PAout(2)// PA2	
#define PWM3 PAout(3)// PA3
#define PWM4 PAout(4)// PA4	
#define PWM5 PAout(5)// PA5
#define PWM6 PAout(6)// PA6	
#define PWM7 PAout(7)// PA7
#define PWM8 PAout(8)// PA8	

extern u16 pwm[8];
extern u16 pos[500][8];
extern u16 motor_speed;
extern u8 point_aim;	
extern u8 end_flag;//魔方复原完成标志位

void change(void);
void vpwm(void);
void MotorPin_Init(void);


		 
#endif

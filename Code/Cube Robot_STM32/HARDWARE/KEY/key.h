#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 
#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) /*读取KEY0*/
#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)/*读取KEY1*/
#define KEY2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) /*读取KEY2，即为丝印标注：WK_UP*/

void KEY_Init(void);
#endif

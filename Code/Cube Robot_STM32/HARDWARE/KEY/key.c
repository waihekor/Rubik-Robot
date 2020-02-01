#include "key.h"
#include "sys.h" 
#include "delay.h"

								    
/*按键初始化函数*/
void KEY_Init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
}


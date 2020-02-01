#ifndef ___INSTRUCTION___H___
#define ___INSTRUCTION___H___

#include "stm32f10x.h"

extern u16 solvecube_data[500][8];/*执行最终解算的数组*/

u8 List_num_Diff(u16 *array1,u16 *array2);
u8 Analy_One_Instruction(u8 char1,u8 char2,u16(*char_buffer)[8]);
u8 Merge_Movement(u16(*array1)[8], u16(*array2)[8], u8 array1_list_num, u8 array2_list_num,u16 (*double_buffer)[8]);
void Initial_Data(u16 (*array)[8],u16 start_line,u16 end_line);
u8 Judge_Merge(u8 char1,u8 char2);
u16 Analy_UsartString(void);
void Analy_String(u8 *rece_string);//调试解析
void Debug_Photo_Angle(u8 *string);



#endif

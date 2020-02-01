/*文件名称: instruction.c
 *文件功能：字符串解析
 *备注：总共16种指令
 *第一组：LL RR FF BB（翻转90度）
 *第二组：L2 R2 F2 B2（旋转180度）
 *第三组：L' R' F' B'（逆时针90度）
 *第四组：L^ R^ F^ B^（顺时针90度）
 */

#include "stm32f10x.h"
#include "instruction.h"
#include "usart.h"
#include "movement.h"
#include "motor.h"
#include <math.h>

u16 solvecube_data[500][8];/*执行最终解算的数组*/

/*  函数名：List_num_Diff
 *函数功能：返回array1和array2元素不同的所在的列号
 *输    入：array为一维数组，8个元素
 *输    出：唯一不同元素所在的列号
 *调用情况：被 Merge_Movement调用
 *备    注：两个数组之间只有一个不同的元素
 */
u8 List_num_Diff(u16 *array1,u16 *array2)
{
	int list_num=0;
	
	for(list_num=0;list_num<8;list_num++)
	{
		if(array1[list_num]!=array2[list_num])
		{
			break;
		}
	}
	
	return list_num;
}


/* 函数名：u8 Analy_One_Instruction(char1,char2) 
*函数功能：根据单个指令的内容把相应的执行数组放在double_buffer数组中
*输    入：char1为单个指令的第一个字符，char2为单个指令的第二个字符
*输    出：执行数组的行数，有三种，4,6,8
*调用情况：被Analy_UsartString()调用
*备    注：放在char_buffer数组中
*/
u8 Analy_One_Instruction(u8 char1,u8 char2,u16(*char_buffer)[8])
{
	u8 i,j,line_num=0;
	if(char1=='L')
	{
			if(char2=='^')//L^
			{
				for(i=0;i<4;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(l_clockwise90+i)+j);
						}
				}
				line_num=4;
				
			}
			else if(char2==0x27)//L'
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(l_anticlockwise90+i)+j);
							}
					}
						line_num=4;
			}
			else if(char2=='2')//L2
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(l_clockwise90+i)+j);
							}
					}
					
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i+4)+j)=*(*(l_clockwise90+i)+j);
							}
					}
					
				line_num=8;

			}
			else //翻转，LL
			{
				
				for(i=0;i<6;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(overturnto_l+i)+j);
						}
				}
				line_num=6;
								
			}
	
	}
	
	else if(char1=='R')
	{
			if(char2=='^')//R^
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(r_clockwise90+i)+j);
							}
					}
				line_num=4;
			}
			else if(char2==0x27)//R'
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(r_anticlockwise90+i)+j);
							}
					}
					line_num=4;

			}
			else if(char2=='2')//R2
			{
				
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(r_clockwise90+i)+j);
							}
					}
					
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i+4)+j)=*(*(r_clockwise90+i)+j);
							}
					}
					
				line_num=8;
				
			}
			else //翻转，RR
			{
				
				for(i=0;i<6;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(overturnto_r+i)+j);
						}
				}
				line_num=6;
								
			}
					
	}
	
	else if(char1=='F')
	{
			if(char2=='^')//F^
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(f_clockwise90+i)+j);
							}
					}
					line_num=4;
			}
			else if(char2==0x27)//F'
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(f_anticlockwise90+i)+j);
							}
					}
					line_num=4;

			}
			else if(char2=='2')//F2
			{
				
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(f_clockwise90+i)+j);
							}
					}
					
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i+4)+j)=*(*(f_clockwise90+i)+j);
							}
					}
					
				line_num=8;				
			}
			else //翻转，FF
			{
				
				for(i=0;i<6;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(overturnto_f+i)+j);
						}
				}
				line_num=6;
				
			}
	}
	else
	{
			if(char2=='^')//B^
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(b_clockwise90+i)+j);
							}
					}
					
				line_num=4;
					
			}
			else if(char2==0x27)//B'
			{
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(b_anticlockwise90+i)+j);
							}
					}
					
					line_num=4;

			}
			else if(char2=='2')//B2
			{
				
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i)+j)=*(*(b_clockwise90+i)+j);
							}
					}
					
					for(i=0;i<4;i++)
					{
							for(j=0;j<8;j++)
							{
									*(*(char_buffer+i+4)+j)=*(*(b_clockwise90+i)+j);
							}
					}
					
				line_num=8;				
			}
			else //翻转,BB
			{
				
				for(i=0;i<6;i++)
				{
						for(j=0;j<8;j++)
						{
								*(*(char_buffer+i)+j)=*(*(overturnto_b+i)+j);
						}
				}
				
				line_num=6;
				
			}
	
	}
	return line_num;
	
}


/* 函数名：u8 Merge_Movement
 *函数功能：把两个可以连续执行的动作指令整合在一起，放在double_buffer数组中
 *输    入：array1_list_num是array1的行数，array2_list_num是array2的行数
 *输    出：两者较大的行数
 *调用情况：
 *备    注：array后一行的元素和前一行相比，只有一个元素不同,行数只有4,8两种
 */
u8 Merge_Movement(u16(*array1)[8], u16(*array2)[8], u8 array1_list_num, u8 array2_list_num,u16 (*double_buffer)[8])
{
	u8 list_num, i, j;
	if (array1_list_num == array2_list_num)//都为8或者都为4的情况
	{
		for (i = 0; i<array1_list_num; i++)
		{
			for (j=0;j<8;j++)
			{
				*(*(double_buffer + i) + j) = *(*(array1 + i) + j);
			}
		
		}
		for (j=0;j<2;j++)
		{
			list_num = List_num_Diff(*(array2 + j), *(array2 + j + 1));
			for (i = 0; i<array1_list_num; i++)
			{
				*(*(double_buffer + i) + list_num) = *(*(array2 + i) + list_num);
			}
		}
		
	}

	if ((array1_list_num == 4 && array2_list_num == 8) || (array1_list_num == 8 && array2_list_num == 4))//一个是8，另外一个是4的情况
	{
		for (i = 0; i<4; i++)
		{
			for (j=0;j<8;j++)
			{
				*(*(double_buffer + i) + j) = *(*(array1 + i) + j);
			}	
		}

		for (j = 0; j<2; j++)
		{
			list_num = List_num_Diff(*(array2 + j), *(array2 + j + 1));
			for (i = 0; i<4; i++)
			{
				*(*(double_buffer + i) + list_num) = *(*(array2 + i) + list_num);
			}
		}

		if (array1_list_num == 4 && array2_list_num == 8)
		{
			for (i = 4; i<8; i++)
			{
				for (j = 0; j<8; j++)
				{
					*(*(double_buffer + i) + j) = *(*(array2 + i) + j);
				}

			}
		}

		else
		{
			for (i = 4; i<8; i++)
			{
				for (j = 0; j<8; j++)
				{
					*(*(double_buffer + i) + j) = *(*(array1 + i) + j);
				}
			}
		}
	}


	if (array1_list_num>array2_list_num)
	{
		return array1_list_num;
	}
	else
	{
		return array2_list_num;

	}
}


/*  函数名：Initial_Data(u16 *array,u8 start_num,u8 end_num)
 *函数功能：为solvecube_data赋值
 *输    入: array:二维数组指针,start_num:数组开始标号,end_num:数组结束标号
 *输    出: 无
 *调用情况: 被Analy_UsartString()函数调用
 *备    注：数组的列数为8，二维数组指针作为函数参数时列数要确定
 */
void Initial_Data(u16 (*array)[8],u16 start_line,u16 end_line)
{
	u8 i,j;
	
	for(j=start_line;j<end_line;j++)
	{
		for(i=0;i<8;i++)
		{
			pos[j][i]=*(*(array+(j-start_line))+i);			
		}
	
	}
}



u8 Judge_Merge(u8 char1,u8 char2)
{
	u8 merge_flag;
		
				if((char1=='L'&&char2=='R')||(char1=='R'&&char2=='L')||(char1=='F'&&char2=='B')||(char1=='B'&&char2=='F'))
				{
					merge_flag=1;
				}	
				else 
				{
					merge_flag=0;
				}
	
	return merge_flag;
	
}

/*  函数名:Analy_UsartString()
 *函数功能：解析串口传来的字符串，并把魔方解算步骤放在solvecube_data数组中
 *输    入：无
 *输    出：solvecube_data数组的行标号
 *调用情况：被SolvecubeArray_ToBufferArray()调用
 *备    注：
 */
u16 Analy_UsartString(void)
{
		u8 i=1;
		u8 j;
		u8 combine_line_add=0;
		u8 line_num1=0;
		u8 line_num2=0;
		
		u16 curr_line=1;//pos第0行填充初始位置
		u16 char1_buffer[8][8];
		u16 char2_buffer[8][8];
		u16 merge_buffer[8][8];
	
		for(j=0;j<8;j++)
		{	
			pos[0][j]=initial_position[j];//这一行是不执行的
		}
	
		while(rece_string[i]!='!')
		{
				if((Judge_Merge(rece_string[i],rece_string[i+2]))&&(rece_string[i+2]!=rece_string[i+3])&&(rece_string[i]!=rece_string[i+1]))//可以连续执行的两个动作组
				{
						line_num1=Analy_One_Instruction(rece_string[i],rece_string[i+1],char1_buffer);
						line_num2=Analy_One_Instruction(rece_string[i+2],rece_string[i+3],char2_buffer);																																		
					
						combine_line_add=Merge_Movement(char1_buffer,char2_buffer,line_num1,line_num2,merge_buffer);
						Initial_Data(merge_buffer,curr_line,curr_line+combine_line_add);
						curr_line+=combine_line_add;
						i+=4;
				}
				else//执行单个动作组
				{
						line_num1=Analy_One_Instruction(rece_string[i],rece_string[i+1],char1_buffer);
						Initial_Data(char1_buffer,curr_line,curr_line+line_num1);
						curr_line+=line_num1;
						i+=2;
				}

		}
		
	return (curr_line-1);
		
}


/*功能描述：调试舵机角度时使用，解析串口接收到的数据
 *输    入:示例:@1170%1250%1280%1190%1500%1500%1500%1500!
 *输    出:无
 *备    注：
 */
void Analy_String(u8 *rece_string)
{
	u8 i = 0;//从第二个字符开始解析
	u8 j = 0;
	u8 buffer_data[4];
	u16 motor_data;
	u8 space_num = 0;
	u8 motor_bits;


	while (rece_string[i] != '!')
	{

		if (rece_string[i] <= '9'&&rece_string[i] >= '0')
		{
			buffer_data[j++] = rece_string[i];

		}

		else if(rece_string[i] == '%')
		{
			

			for (motor_bits = 0; motor_bits<j; motor_bits++)
			{
				motor_data += asc_to_num(buffer_data[motor_bits])*(u16)pow(10, j-1-motor_bits);//整理前方战果，调整休息一下
			}

			pos[0][space_num] = motor_data;
			pos[1][space_num] = motor_data;
			
			space_num++;
			motor_data = 0;
			j = 0;
			
		}
		
		i++;

	}
	
	for (motor_bits=0; motor_bits<j; motor_bits++)
	{
		
	motor_data += asc_to_num(buffer_data[motor_bits])*(u16)pow(10, j - 1 - motor_bits);
		
	}

	pos[0][space_num] = motor_data;
	pos[1][space_num] = motor_data;

}


/*功能描述:调试拍照时候的舵机角度，水平角度
 *输    入:示例:%1!,%2!,%3！，%4！，%5！，%6！
 *输    出:执行数组的行标号
 *备    注：一定要顺序连续发送！！！
 */
void Debug_Photo_Angle(u8 *string)
{
	
  u8 photo_num; 
	
	photo_num=asc_to_num(string[1]);
	
	switch(photo_num)
	{
		
		case 1: 
		{
			
			PicArray_ToBufferArray(firpic_position,4);
			
	  }break;
		
		case 2: 
		{
			
			PicArray_ToBufferArray(secpic_position,1);

	  }break;

		case 3: 
		{
			
			PicArray_ToBufferArray(thirpic_position,4);
			
	  }break;
		
		case 4: 
		{
			
			PicArray_ToBufferArray(fourpic_position,1);//标号

	  }break;
		
		case 5: 
		{
			
			PicArray_ToBufferArray(fifpic_position,4);

	  }break;
		
		case 6: 
		{
			
			PicArray_ToBufferArray(sixpic_position,1);

	  }break;
	
		case 7: 
		{
			
			PicArray_ToBufferArray(retuinit_position,4);

	  }break;		
		
		
		default:
		{

		}break;

  }
	

}

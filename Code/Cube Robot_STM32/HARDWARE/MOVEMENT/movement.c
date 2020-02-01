/*  文件名：movement.c
 *文件描述：把魔方机器人需要完成的独立动作放入相应的数组中。
 *备    注: 在计算舵机动作数组时,记住用到的是绝对位置，而不是相对位置
 */
#include "stm32f10x.h"
#include "movement.h"
#include "motor.h"
#include "usart.h"
#include "instruction.h"

static const u16 original_position[4]={1420,1400,1410,1490};          /*初始位置1,2,3,4号舵机的角度*/
static const u16 clockwise90_position[4]={530,500,590,550};           /*由初始位置顺时针转动90度时1,2,3,4号舵机的角度*/
static const u16 anticlockwise90_position[4]={2370,2350,2210,2430};   /*由初始位置逆时针转动90度时1,2,3,4号舵机的角度*/

static const u16 clockwise180_position[4]={2270,2310,2340,2340};    

static const u16 wrasp_position[4]={1900,1550,1740,1650};             /*机械手抓紧魔方时5,6,7,8号舵机的角度*/
static const u16 loosen_position[4]={1400,1050,1250,1150};            /*机械手松开魔方时5,6,7,8号舵机的角度*/

//
/*
 *拍照的顺序为左、右、前、后、上、下
 *SD卡中图片编号和魔方方位的对应关系是：1：左，2：右，3：前，4：后，5：上，6：下
 *颜色识别完之后，魔方的状态发生了变化：识别后
 *后、前、右、左、下、上
 *
 */

u16 lines_num=0;//数组的行标号
u16 initial_position[8];//机械手抓紧魔方
u16 firpic_position[5][8];//拍照第1张照片
u16 secpic_position[2][8];//拍照第2张照片
u16 thirpic_position[5][8];//拍照第3张照片
u16 fourpic_position[2][8];//拍照第4张照片
u16 fifpic_position[5][8];//拍照第5张照片
u16 sixpic_position[2][8];//拍照第6张照片
u16 retuinit_position[5][8];//拍摄完照片返回初始化位置，即机械手抓紧魔方的位置




u16 r_clockwise90[4][8]; /*右层顺时针90度*/
u16 l_clockwise90[4][8]; /*左层顺时针90度*/
u16 f_clockwise90[4][8]; /*前层顺时针90度*/
u16 b_clockwise90[4][8]; /*后层顺时针90度*/

u16 r_anticlockwise90[4][8]; /*右层逆时针90度*/
u16 l_anticlockwise90[4][8]; /*左层逆时针90度*/
u16 f_anticlockwise90[4][8]; /*前层逆时针90度*/
u16 b_anticlockwise90[4][8]; /*后层逆时针90度*/

u16 r_clockwise180[4][8]; /*右层180度*/
u16 l_clockwise180[4][8]; /*左层180度*/
u16 f_clockwise180[4][8]; /*前层180度*/
u16 b_clockwise180[4][8]; /*后层180度*/

u16 overturnto_f[6][8]; /*向前翻转90度*/
u16 overturnto_b[6][8]; /*向后翻转90度*/
u16 overturnto_l[6][8]; /*向左翻转90度*/
u16 overturnto_r[6][8]; /*向右翻转90度*/



/*  函数名：Init_TotalArray()
 *函数功能：把所有需要在程序中一开始计算的函数放在此函数中，以初始化数组
 *输    入：无
 *输    出：无
 *备    注：注意调用函数的顺序,运行完需要530us
 */
void Init_TotalArray(void)
{
	Calcul_InitPosition(); /*此函数首先被初始化，因为生成的数组会被以后很多函数用到*/
	Calcul_FirPicPosition();
  Calcul_SecPicPosition();
  Calcul_ThirPicPosition();
  Calcul_FourPicPosition();
  Calcul_FifPicPosition();
  Calcul_SixPicPosition();
  RetuIni_AftPic();
	
	Calcul_Lclockwise90();
	Calcul_Rclockwise90();
	Calcul_Fclockwise90();
	Calcul_Bclockwise90();
	
	Calcul_Lanticlockwise90();
	Calcul_Ranticlockwise90();
	Calcul_Fanticlockwise90();
	Calcul_Banticlockwise90();
	
	Calcul_Lclockwise180();
	Calcul_Rclockwise180();
	Calcul_Fclockwise180();
	Calcul_Bclockwise180();
	
	Calcul_OverturntoB();//向后翻转
	Calcul_OverturntoF();//向前翻转
	Calcul_OverturntoL();//向左翻转
	Calcul_OverturntoR();//向右翻转

}


/*
 *  函数名：Calcul_IniticalPosition()
 *函数功能: 计算舵机的初始角度
 *输    入: 无
 *输    出: 无
 */
void Calcul_InitPosition(void)
{
	u8 i,j;
	for(i=0;i<4;i++)
	{
		initial_position[i]=original_position[i];
	}
	for(i=4,j=0;i<8;i++,j++)
	{
		initial_position[i]=wrasp_position[j];
	}
	
}


/*  函数名：Init_MotorMovement()
 *函数功能：5,6,7,8松开魔方，好让操作者把魔方放入机械手中
 *输    入：无
 *输    出：无
 *备    注：
 */
void Init_MotorMovement(void)
{
		u8 i;
		
		for(i=0;i<8;i++)
			{
			
				pos[0][i]=initial_position[i];/*初始位置*/
			}
			
		for(i=0;i<4;i++)
			{
			
				pos[1][i]=initial_position[i];
			}
			
		for(i=0;i<4;i++)
			{
			
				pos[1][i+4]=loosen_position[i];/*5,6,7,8松开魔方，好让操作者把魔方放入机械手中*/
			}
			
			lines_num=1;
	}


/*  函数名:Calcul_FirPicPosition()
 *函数功能:计算拍照第一个面的舵机运行数组(先抓紧魔方)
 *备    注:数组标号=舵机号-1,拍照左面
 */
void Calcul_FirPicPosition(void)
{
	u8 i;
	
	for(i=0;i<8;i++)                     
	{
	  firpic_position[0][i]=initial_position[i];//不执行
	}

	for(i=0;i<8;i++)                    
	{
	  firpic_position[1][i]=initial_position[i];/*第一行放初始位置*/
	}
	firpic_position[1][5]=loosen_position[1];/*6松开*/
	firpic_position[1][7]=loosen_position[3];/*8松开*/	

	
	for(i=0;i<8;i++)                    
	{
	  firpic_position[2][i]=initial_position[i];  /*5,6,7,8都抓紧，回到初始位置*/
	}
	
	
	/*下面是拍照环节*/
	
	for(i=0;i<8;i++)                    
	{
		firpic_position[3][i]=firpic_position[2][i]; /*第三行暂时复制第二行*/
	}
	firpic_position[3][5]=loosen_position[1];            /*6后退*/
	firpic_position[3][7]=loosen_position[3];            /*8后退*/

 	for(i=0;i<8;i++)                    
	{
		firpic_position[4][i]=firpic_position[3][i]; /*第四行暂时复制第三行*/
	}
	
	  /*拍照左面*/                        
		firpic_position[4][0]=clockwise90_position[0];      /*1顺时针90度,*/
		firpic_position[4][2]=anticlockwise90_position[2];  /*3逆时针90度*/           
}



/*  函数名:Calcul_SecPicPosition()
 *函数功能:计算拍照第二个面的舵机运行数组
 *备    注:数组标号=舵机号-1,先运行Calcul_FirPicPosition()函数，拍照右面
 */
void Calcul_SecPicPosition(void)
{
	u8 i;
	
 	for(i=0;i<8;i++)                     
	{
		secpic_position[0][i]=firpic_position[4][i];
	}
	
 	for(i=0;i<8;i++)                     
	{
		secpic_position[1][i]=secpic_position[0][i];
	}
	
                      /*拍照右面*/   
 secpic_position[1][0]=anticlockwise90_position[0];        /*1逆时针90度*/ 
 secpic_position[1][2]=clockwise90_position[2];	          /*3顺时针90度*/ 

	
}


/*  函数名:Calcul_ThirPicPosition()
 *函数功能:计算拍照第三个面的舵机运行数组
 *备    注:数组标号=舵机号-1，拍照前面
 */
void Calcul_ThirPicPosition(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		thirpic_position[0][i]=secpic_position[1][i];
	}
	
	for(i=0;i<8;i++)
	{
		thirpic_position[1][i]=thirpic_position[0][i];
	}
	
	thirpic_position[1][5]=wrasp_position[1];  /*6前进*/
	thirpic_position[1][7]=wrasp_position[3];  /*8前进*/
	
	for(i=0;i<8;i++)
	{
		thirpic_position[2][i]=thirpic_position[1][i];  
	}
	
	thirpic_position[2][4]=loosen_position[0];   /*5后退*/
	thirpic_position[2][6]=loosen_position[2];   /*7后退*/
	
	for(i=0;i<8;i++)
	{
	  thirpic_position[3][i]=thirpic_position[2][i];  
	}
	
	thirpic_position[3][0]=original_position[0]; /*1回到初始位置*/
	thirpic_position[3][2]=original_position[2]; /*3回到初始位置*/
	
		/*拍照前面*/
	for(i=0;i<8;i++)
	{
	  thirpic_position[4][i]=thirpic_position[3][i];  /*复制第一行到第二行*/
	}
	
	thirpic_position[4][1]=clockwise90_position[1];                 /*2顺时针，小一点*/
	thirpic_position[4][3]=anticlockwise90_position[3];             /*4逆时针，大一点*/

}


/*  函数名:Calcul_FourPicPosition()
 *函数功能:计算拍照第四个面的舵机运行数组
 *备    注:数组标号=舵机号-1，拍照后面
 */
void Calcul_FourPicPosition(void)
{
	 u8 i;
	
	for(i=0;i<8;i++)
		{
			fourpic_position[0][i]=thirpic_position[4][i];   
		}
		
	for(i=0;i<8;i++)
		{
			fourpic_position[1][i]=fourpic_position[0][i];   
		}
		
	/*拍照后面*/
	fourpic_position[1][1]=anticlockwise90_position[1];  /*2逆时针*/
	fourpic_position[1][3]=clockwise90_position[3];      /*4顺时针*/
			
}

/*  函数名:Calcul_FifPicPosition()
 *函数功能:计算拍照第五个面的舵机运行数组
 *备    注:数组标号=舵机号-1,拍照上面
 */
void Calcul_FifPicPosition(void)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		fifpic_position[0][i]=fourpic_position[1][i];
	}
	
	for(i=0;i<8;i++)
	{
		fifpic_position[1][i]=fifpic_position[0][i];
	}
	
  fifpic_position[1][4]=wrasp_position[0];		/*5前进*/
  fifpic_position[1][6]=wrasp_position[2];		/*7前进*/	

	for(i=0;i<8;i++)
	{
		fifpic_position[2][i]=fifpic_position[1][i];
	}
	
	fifpic_position[2][5]=loosen_position[1];   /*6后退*/
	fifpic_position[2][7]=loosen_position[3];   /*8后退*/
	
	
	for(i=0;i<8;i++)
	{
		fifpic_position[3][i]=fifpic_position[2][i];
	}
	
	fifpic_position[3][1]=original_position[1];	/*2回到初始位置*/
	fifpic_position[3][3]=original_position[3];	/*4回到初始位置*/
		
	/*拍照上面*/
	for(i=0;i<8;i++)
	{
		fifpic_position[4][i]=fifpic_position[3][i];
	}
	
	fifpic_position[4][0]=clockwise90_position[0];      /*1顺时针*/
	fifpic_position[4][2]=anticlockwise90_position[2];  /*3逆时针*/
	
}


/*  函数名:Calcul_SixPicPosition()
 *函数功能:计算拍照第六个面的舵机运行数组
 *备    注:数组标号=舵机号-1，拍照下面 
 */
void Calcul_SixPicPosition(void)
{
			u8 i;
			for(i=0;i<8;i++)
			{
				sixpic_position[0][i]=fifpic_position[4][i];
			}
			
			for(i=0;i<8;i++)
			{
				sixpic_position[1][i]=sixpic_position[0][i];
			}
			
			/*拍照下面*/
			sixpic_position[1][0]=anticlockwise90_position[0];/*1逆时针*/
			sixpic_position[1][2]=clockwise90_position[2];    /*3顺时针*/
			
			
}


/*  函数名: RetuIni_AftPic()
 *函数功能: 在拍照完6个面之后回到初始位置
 *备    注：按照原路的逆路径返回，并且和原来的摆放位置相同，实际上只需要两部
 */
void RetuIni_AftPic(void)
{
				u8 i;
			
			for(i=0;i<8;i++)
			{
				retuinit_position[0][i]=sixpic_position[1][i];
			}	
			
			for(i=0;i<8;i++)
			{
				retuinit_position[1][i]=retuinit_position[0][i];
			}	
				
			retuinit_position[1][5]=wrasp_position[1];  /*6前进*/
			retuinit_position[1][7]=wrasp_position[3];  /*8前进*/
			
			for(i=0;i<8;i++)
			{
				retuinit_position[2][i]=retuinit_position[1][i];
			}
			retuinit_position[2][4]=loosen_position[0];  /*5后退*/
			retuinit_position[2][6]=loosen_position[2];  /*7后退*/
			
			for(i=0;i<8;i++)
			{
				retuinit_position[3][i]=retuinit_position[2][i];
			}
			
			retuinit_position[3][0]=original_position[0]; /*1回到初始位置*/ 
			retuinit_position[3][2]=original_position[2]; /*3回到初始位置*/ 
				
			for(i=0;i<8;i++)
			{
				retuinit_position[4][i]=retuinit_position[3][i];
			}
			
			retuinit_position[4][4]=wrasp_position[0];    /*5前进*/
			retuinit_position[4][6]=wrasp_position[2];    /*7前进*/	

}


/*  函数名：PicArray_ToBufferArray(u(*array)[8],line_num)
 *函数功能：把拍照需要执行的数组放入缓存数组，并返回需要执行数组的行标
 *输    入：二维数组指针(*array)[8]，数组行标line_num
 *输    出：数组行标
 *备    注：在把相关数组放入缓存数组时，要关掉定时器，防止插补函数中改变全局变量lines_num，缓存数组是全局数组
 */
void PicArray_ToBufferArray(u16 (*array)[8],u16 arrayline_num)
{
		u8 i,j;
	
	  lines_num=arrayline_num;

		for(j=0;j<=lines_num;j++)
		{
			
			for(i=0;i<8;i++)
			{
				pos[j][i]=*(*(array+j)+i);			
			}
		
		}
	
}

/*  函数名：Calcul_Lclockwise90()
 *函数功能：计算魔方左面顺时针旋转90度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：先调用initial_position()函数
 */
void Calcul_Lclockwise90(void)
{
	
	u8 i;
	
	for(i=0;i<8;i++)
	{
		l_clockwise90[0][i]=initial_position[i];
	}
	
	l_clockwise90[0][1]=anticlockwise90_position[1];/*2逆时针*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise90[1][i]=l_clockwise90[0][i];
	}
	
	l_clockwise90[1][5]=loosen_position[1]; /*6后退*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise90[2][i]=l_clockwise90[1][i];
	}
	
	l_clockwise90[2][1]=original_position[1];/*2回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise90[3][i]=l_clockwise90[2][i];
	}
	
	l_clockwise90[3][5]=wrasp_position[1]; /*6前进*/
	
	
}


/*  函数名：Calcul_Lanticlockwise90()
 *函数功能：计算魔方左面逆时针旋转90度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：先调用initial_position()函数
 */
void Calcul_Lanticlockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		l_anticlockwise90[0][i]=initial_position[i];
	}
	
	l_anticlockwise90[0][1]=clockwise90_position[1];/*2顺时针*/
	
	for(i=0;i<8;i++)
	{
		l_anticlockwise90[1][i]=l_anticlockwise90[0][i];
	}
	
	l_anticlockwise90[1][5]=loosen_position[1]; /*6后退*/
	
	for(i=0;i<8;i++)
	{
		l_anticlockwise90[2][i]=l_anticlockwise90[1][i];
	}
	
	l_anticlockwise90[2][1]=original_position[1];/*2回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		l_anticlockwise90[3][i]=l_anticlockwise90[2][i];
	}
	
	l_anticlockwise90[3][5]=wrasp_position[1]; /*6前进*/
	

}


/*  函数名：Calcul_Clockwise180()
 *函数功能：计算魔方左面旋转180度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：先调用initial_position()函数
 */
void Calcul_Lclockwise180(void)
{
	
	u8 i;
	
	for(i=0;i<8;i++)
	{
		l_clockwise180[0][i]=initial_position[i];
	}
	
	l_clockwise180[0][1]=clockwise180_position[1];/*2旋转180度*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise180[1][i]=l_clockwise180[0][i];
	}
	
	l_clockwise90[1][5]=loosen_position[1]; /*6后退*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise180[2][i]=l_clockwise180[1][i];
	}
	
	l_clockwise180[2][1]=original_position[1];/*2回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		l_clockwise180[3][i]=l_clockwise180[2][i];
	}
	
	l_clockwise180[3][5]=wrasp_position[1]; /*6前进*/
	
}

/*  函数名：Calcul_OverturntoL(void)
 *函数功能：向左翻转90度
 *输    入：无
 *输    出：无
 *调用情况：被	Init_TotalArray()调用，在程序中初始化
 *备    注：
 */
void Calcul_OverturntoL(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		overturnto_l[0][i]=initial_position[i];
	}
	
	overturnto_l[0][5]=loosen_position[1];/*6后退*/
	overturnto_l[0][7]=loosen_position[3];/*8后退*/
	
	for(i=0;i<8;i++)
	{
		overturnto_l[1][i]=overturnto_l[0][i];
	}
	
	overturnto_l[1][0]=anticlockwise90_position[0];       /*1逆时针*/
	overturnto_l[1][2]=clockwise90_position[2];           /*3顺时针，完成向左翻转的动作*/
	
	for(i=0;i<8;i++)
	{
		overturnto_l[2][i]=overturnto_l[1][i];
	}
	
	overturnto_l[2][5]=wrasp_position[1]; /*6前进*/
	overturnto_l[2][7]=wrasp_position[3]; /*8前进*/
	
	for(i=0;i<8;i++)
	{
		overturnto_l[3][i]=overturnto_l[2][i];
	}
	overturnto_l[3][4]=loosen_position[0]; /*5后退*/
	overturnto_l[3][6]=loosen_position[2]; /*7后退*/


	for(i=0;i<8;i++)
	{
		overturnto_l[4][i]=overturnto_l[3][i];
	}
	
	 overturnto_l[4][0]=original_position[0];/*1回到初始位置*/
	 overturnto_l[4][2]=original_position[2];/*3回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		overturnto_l[5][i]=overturnto_l[4][i];
	}
	
	overturnto_l[5][4]=wrasp_position[0]; /*5前进*/
	overturnto_l[5][6]=wrasp_position[2]; /*7前进，此时到达初始位置*/
}


/*  函数名：Calcul_Rclockwise90()
 *函数功能：计算魔方右面顺时针旋转90度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：先调用initial_position()函数
 */
void Calcul_Rclockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		r_clockwise90[0][i]=initial_position[i];
	}
	
	r_clockwise90[0][3]=anticlockwise90_position[3];/*4逆时针*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise90[1][i]=r_clockwise90[0][i];
	}
	
	r_clockwise90[1][7]=loosen_position[3]; /*8后退*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise90[2][i]=r_clockwise90[1][i];
	}
	
	r_clockwise90[2][3]=original_position[3];/*4回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise90[3][i]=r_clockwise90[2][i];
	}
	
	r_clockwise90[3][7]=wrasp_position[3]; /*8前进*/
	
}



/*  函数名：Calcul_Ranticlockwise90()
 *函数功能：计算魔方右面逆时针旋转90度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：先调用initial_position()函数
 */
void Calcul_Ranticlockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		r_anticlockwise90[0][i]=initial_position[i];
	}
	
    r_anticlockwise90[0][3]=clockwise90_position[3];/*4顺时针*/
	
	for(i=0;i<8;i++)
	{
		r_anticlockwise90[1][i]=r_anticlockwise90[0][i];
	}
	
	r_anticlockwise90[1][7]=loosen_position[3]; /*8后退*/
	
	for(i=0;i<8;i++)
	{
		r_anticlockwise90[2][i]=r_anticlockwise90[1][i];
	}
	
	r_anticlockwise90[2][3]=original_position[3];/*4回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		r_anticlockwise90[3][i]=r_anticlockwise90[2][i];
	}
	
	r_anticlockwise90[3][7]=wrasp_position[3]; /*8前进*/
	
}


/*  函数名：Calcul_Rclockwise180()
 *函数功能：计算魔方右面旋转180度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：先调用initial_position()函数
 */
void Calcul_Rclockwise180(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		r_clockwise180[0][i]=initial_position[i];
	}
	
	r_clockwise180[0][3]=clockwise180_position[3];/*4逆时针*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise180[1][i]=r_clockwise180[0][i];
	}
	
	r_clockwise180[1][7]=loosen_position[3]; /*8后退*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise180[2][i]=r_clockwise180[1][i];
	}
	
	r_clockwise180[2][3]=original_position[3];/*4回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		r_clockwise180[3][i]=r_clockwise180[2][i];
	}
	
	r_clockwise180[3][7]=wrasp_position[3]; /*8前进*/
	
}


/*  函数名：Calcul_OverturntoR(void)
 *函数功能：向右翻转90度
 *输    入：无
 *输    出：无
 *调用情况：被	Init_TotalArray()调用，在程序中初始化
 *备    注：
 */
void Calcul_OverturntoR(void)
{
		u8 i;
	
	for(i=0;i<8;i++)
	{
		overturnto_r[0][i]=initial_position[i];
	}
	
	overturnto_r[0][5]=loosen_position[1]; /*6后退*/
	overturnto_r[0][7]=loosen_position[3]; /*8后退*/
	
	for(i=0;i<8;i++)
	{
		overturnto_r[1][i]=overturnto_r[0][i];
	}
	
	overturnto_r[1][0]=clockwise90_position[0];       /*1顺时针*/
	overturnto_r[1][2]=anticlockwise90_position[2];   /*3逆时针*/
	
	for(i=0;i<8;i++)
	{
		overturnto_r[2][i]=overturnto_r[1][i];
	}
	
	overturnto_r[2][5]=wrasp_position[1]; /*6前进*/
	overturnto_r[2][7]=wrasp_position[3]; /*8前进*/
	
	for(i=0;i<8;i++)
	{
		overturnto_r[3][i]=overturnto_r[2][i];
	}
	overturnto_r[3][4]=loosen_position[0]; /*5后退*/
	overturnto_r[3][6]=loosen_position[2]; /*7后退*/


	for(i=0;i<8;i++)
	{
		overturnto_r[4][i]=overturnto_r[3][i];
	}
	
	 overturnto_r[4][0]=original_position[0];/*1回到初始位置*/
	 overturnto_r[4][2]=original_position[2];/*3回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		overturnto_r[5][i]=overturnto_r[4][i];
	}
	
	overturnto_r[5][4]=wrasp_position[0]; /*5前进*/
	overturnto_r[5][6]=wrasp_position[2]; /*7前进，此时到达初始位置*/
		
}

/*  函数名：Calcul_Fclockwise90()
 *函数功能：计算魔方前面顺时针旋转90度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：
 */
void Calcul_Fclockwise90(void)
{
    u8 i;
	
	for(i=0;i<8;i++)
	{
		f_clockwise90[0][i]=initial_position[i];
	}
	
	f_clockwise90[0][2]=anticlockwise90_position[2];/*3逆时针*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise90[1][i]=f_clockwise90[0][i];
	}
	
	f_clockwise90[1][6]=loosen_position[2]; /*7后退*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise90[2][i]=f_clockwise90[1][i];
	}
	
	f_clockwise90[2][2]=original_position[2];/*3回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise90[3][i]=f_clockwise90[2][i];
	}
	
	f_clockwise90[3][6]=wrasp_position[2]; /*7前进*/
	
}


/*  函数名：Calcul_Fanticlockwise90()
 *函数功能：计算魔方前面逆时针旋转90度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：
 */
void Calcul_Fanticlockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		f_anticlockwise90[0][i]=initial_position[i];
	}
	
	f_anticlockwise90[0][2]=clockwise90_position[2];/*3顺时针*/
	
	for(i=0;i<8;i++)
	{
		f_anticlockwise90[1][i]=f_anticlockwise90[0][i];
	}
	
	f_anticlockwise90[1][6]=loosen_position[2]; /*7后退*/
	
	for(i=0;i<8;i++)
	{
		f_anticlockwise90[2][i]=f_anticlockwise90[1][i];
	}
	
	f_anticlockwise90[2][2]=original_position[2];/*3回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		f_anticlockwise90[3][i]=f_anticlockwise90[2][i];
	}
	
	f_anticlockwise90[3][6]=wrasp_position[2]; /*7前进*/
	
}


/*  函数名：Calcul_Fclockwise180()
 *函数功能：计算魔方前面旋转180度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：
 */
void Calcul_Fclockwise180(void)
{
    u8 i;
	
	for(i=0;i<8;i++)
	{
		f_clockwise180[0][i]=initial_position[i];
	}
	
	f_clockwise180[0][2]=clockwise180_position[2];/*3逆时针*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise180[1][i]=f_clockwise180[0][i];
	}
	
	f_clockwise180[1][6]=loosen_position[2]; /*7后退*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise180[2][i]=f_clockwise180[1][i];
	}
	
	f_clockwise180[2][2]=original_position[2];/*3回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		f_clockwise180[3][i]=f_clockwise180[2][i];
	}
	
	f_clockwise180[3][6]=wrasp_position[2]; /*7前进*/
	
}


/*  函数名：Calcul_OverturntoF(void)
 *函数功能：向前翻转90度
 *输    入：无
 *输    出：无
 *调用情况：被	Init_TotalArray()调用，在程序中初始化
 *备    注：
 */
void Calcul_OverturntoF(void)
{
	
	u8 i;
	
	for(i=0;i<8;i++)
	{
		overturnto_f[0][i]=initial_position[i];
	}
	
	overturnto_f[0][4]=loosen_position[0];/*5后退*/
	overturnto_f[0][6]=loosen_position[2];/*7后退*/
	
	for(i=0;i<8;i++)
	{
		overturnto_f[1][i]=overturnto_f[0][i];
	}
	
	overturnto_f[1][1]=anticlockwise90_position[1];       /*2逆时针*/
	overturnto_f[1][3]=clockwise90_position[3];           /*4顺时针，完成向前翻转的动作*/
	
	for(i=0;i<8;i++)
	{
		overturnto_f[2][i]=overturnto_f[1][i];
	}
	
	overturnto_f[2][4]=wrasp_position[0]; /*5前进*/
	overturnto_f[2][6]=wrasp_position[2]; /*7前进*/
	
	for(i=0;i<8;i++)
	{
		overturnto_f[3][i]=overturnto_f[2][i];
	}
	overturnto_f[3][5]=loosen_position[1]; /*6后退*/
	overturnto_f[3][7]=loosen_position[3]; /*8后退*/


	for(i=0;i<8;i++)
	{
		overturnto_f[4][i]=overturnto_f[3][i];
	}
	
	 overturnto_f[4][1]=original_position[1];/*2回到初始位置*/
	 overturnto_f[4][3]=original_position[3];/*4回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		overturnto_f[5][i]=overturnto_f[4][i];
	}
	
	overturnto_f[5][5]=wrasp_position[1]; /*6前进*/
	overturnto_f[5][7]=wrasp_position[3]; /*8前进，此时到达初始位置*/
   
	
}


/*  函数名：Calcul_	Bclockwise90()
 *函数功能：计算魔方后面顺时针旋转90度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：
 */
void Calcul_Bclockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		b_clockwise90[0][i]=initial_position[i];
	}
	
	b_clockwise90[0][0]=anticlockwise90_position[0];/*1逆时针*/
	
	for(i=0;i<8;i++)
	{
		b_clockwise90[1][i]=b_clockwise90[0][i];
	}
	
	b_clockwise90[1][4]=loosen_position[0]; /*5后退*/
	
	for(i=0;i<8;i++)
	{
		b_clockwise90[2][i]=b_clockwise90[1][i];
	}
	
	b_clockwise90[2][0]=original_position[0];/*1回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		b_clockwise90[3][i]=b_clockwise90[2][i];
	}
	
	b_clockwise90[3][4]=wrasp_position[0]; /*5前进*/
	

}

/*  函数名：Calcul_	Bantianticlockwise90()
 *函数功能：计算魔方后面逆时针旋转90度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：
 */
void Calcul_Banticlockwise90(void)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		b_anticlockwise90[0][i]=initial_position[i];
	}
	
	b_anticlockwise90[0][0]=clockwise90_position[0];    /*1顺时针*/
	
	for(i=0;i<8;i++)
	{
		b_anticlockwise90[1][i]=b_anticlockwise90[0][i];
	}
	
	b_anticlockwise90[1][4]=loosen_position[0];         /*5后退*/
	
	for(i=0;i<8;i++)
	{
		b_anticlockwise90[2][i]=b_anticlockwise90[1][i];
	}
	
	b_anticlockwise90[2][0]=original_position[0];      /*1回到初始位置*/
	
	for(i=0;i<8;i++)
	{
		b_anticlockwise90[3][i]=b_anticlockwise90[2][i];
	}
	
	b_anticlockwise90[3][4]=wrasp_position[0];         /*5前进*/
	
}

/*  函数名：Calcul_	Bclockwise180()
 *函数功能：计算魔方后面顺时针旋转180度的舵机执行数组
 *输    入：无
 *输    出：无
 *调用情况：初始化函数
 *备    注：
 */
void Calcul_Bclockwise180(void)
{
	
		u8 i;
		
		for(i=0;i<8;i++)
		{
			b_clockwise180[0][i]=initial_position[i];
		}
		
		b_clockwise180[0][0]=clockwise180_position[0];/*1逆时针*/
		
		for(i=0;i<8;i++)
		{
			b_clockwise180[1][i]=b_clockwise180[0][i];
		}
		
		b_clockwise180[1][4]=loosen_position[0]; /*5后退*/
		
		for(i=0;i<8;i++)
		{
			b_clockwise180[2][i]=b_clockwise180[1][i];
		}
		
		b_clockwise180[2][0]=original_position[0];/*1回到初始位置*/
		
		for(i=0;i<8;i++)
		{
			b_clockwise180[3][i]=b_clockwise180[2][i];
		}
		
		b_clockwise180[3][4]=wrasp_position[0]; /*5前进*/
		
}


/*  函数名：Calcul_OverturntoB()
 *函数功能：向后翻转90度
 *输    入：无
 *输    出：无
 *调用情况：被	Init_TotalArray()调用，在程序中初始化
 *备    注：
 */
void Calcul_OverturntoB(void)
{
	
			u8 i;
		
		for(i=0;i<8;i++)
		{
			overturnto_b[0][i]=initial_position[i];
		}
		
		overturnto_b[0][4]=loosen_position[0];/*5后退*/
		overturnto_b[0][6]=loosen_position[2];/*7后退*/
		
		for(i=0;i<8;i++)
		{
			overturnto_b[1][i]=overturnto_b[0][i];
		}
		
		overturnto_b[1][1]=clockwise90_position[1];               /*2顺时针*/
		overturnto_b[1][3]=anticlockwise90_position[3];           /*4逆时针，完成向后翻转的动作*/
		
		for(i=0;i<8;i++)
		{
			overturnto_b[2][i]=overturnto_b[1][i];
		}
		
		overturnto_b[2][4]=wrasp_position[0]; /*5前进*/
		overturnto_b[2][6]=wrasp_position[2]; /*7前进*/
		
		for(i=0;i<8;i++)
		{
			overturnto_b[3][i]=overturnto_b[2][i];
		}
		overturnto_b[3][5]=loosen_position[1]; /*6后退*/
		overturnto_b[3][7]=loosen_position[3]; /*8后退*/


		for(i=0;i<8;i++)
		{
			overturnto_b[4][i]=overturnto_b[3][i];
		}
		
		 overturnto_b[4][1]=original_position[1];/*2回到初始位置*/
		 overturnto_b[4][3]=original_position[3];/*4回到初始位置*/
		
		for(i=0;i<8;i++)
		{
			overturnto_b[5][i]=overturnto_b[4][i];
		}
		
		overturnto_b[5][5]=wrasp_position[1]; /*6前进*/
		overturnto_b[5][7]=wrasp_position[3]; /*8前进，此时到达初始位置*/
   
}
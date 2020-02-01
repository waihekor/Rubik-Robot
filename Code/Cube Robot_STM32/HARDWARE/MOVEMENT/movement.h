#ifndef ____MOVEMENT____H____
#define ____MOVEMENT____H____

#include "stm32f10x.h"

extern u16 initial_position[8];//初始位置

extern u16 lines_num;//数组的行标号

/*下面是拍照所需要的动作组*/
extern u16 firpic_position[5][8];
extern u16 secpic_position[2][8];
extern u16 thirpic_position[5][8];
extern u16 fourpic_position[2][8];
extern u16 fifpic_position[5][8];
extern u16 sixpic_position[2][8];
extern u16 retuinit_position[5][8];

extern u16 r_clockwise90[4][8];/*右层顺时针90度*/
extern u16 l_clockwise90[4][8];/*左层顺时针90度*/
extern u16 f_clockwise90[4][8];/*前层顺时针90度*/
extern u16 b_clockwise90[4][8];/*后层顺时针90度*/

extern u16 r_anticlockwise90[4][8];/*右层逆时针90度*/
extern u16 l_anticlockwise90[4][8];/*左层逆时针90度*/
extern u16 f_anticlockwise90[4][8];/*前层逆时针90度*/
extern u16 b_anticlockwise90[4][8];/*后层逆时针90度*/

extern u16 r_clockwise180[4][8]; /*右层180度*/
extern u16 l_clockwise180[4][8]; /*左层180度*/
extern u16 f_clockwise180[4][8]; /*前层180度*/
extern u16 b_clockwise180[4][8]; /*后层180度*/

extern u16 overturnto_f[6][8]; /*向前翻转90度*/
extern u16 overturnto_b[6][8]; /*向后翻转90度*/
extern u16 overturnto_l[6][8]; /*向左翻转90度*/
extern u16 overturnto_r[6][8]; /*向右翻转90度*/

void Calcul_InitPosition(void);
void Init_MotorMovement(void);

void Calcul_FirPicPosition(void);
void Calcul_SecPicPosition(void);
void Calcul_ThirPicPosition(void);
void Calcul_FourPicPosition(void);
void Calcul_FifPicPosition(void);
void Calcul_SixPicPosition(void);
void RetuIni_AftPic(void);
void PicArray_ToBufferArray(u16 (*array)[8],u16 line_num);

void Calcul_Lclockwise90(void);
void Calcul_Rclockwise90(void);
void Calcul_Fclockwise90(void);
void Calcul_Bclockwise90(void);

void Calcul_Lanticlockwise90(void);
void Calcul_Ranticlockwise90(void);
void Calcul_Fanticlockwise90(void);
void Calcul_Banticlockwise90(void);

void Calcul_Lclockwise180(void);
void Calcul_Rclockwise180(void);
void Calcul_Fclockwise180(void);
void Calcul_Bclockwise180(void);

void Calcul_OverturntoR(void);
void Calcul_OverturntoL(void);
void Calcul_OverturntoF(void);
void Calcul_OverturntoB(void);

void Init_TotalArray(void);



#endif

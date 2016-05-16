#include "bsp_ads1115.h"

/*
需要移植的代码：
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ADS1115 IIC从机地址(8位)
#define BSP_ADS1115_ADDR 0x90

//接口函数-ADS1115 IIC初始化
void BSP_ADS1115_Init_Port(void)
{
	HAL_IIC2_Init();
}

//接口函数-ADS1115 IIC失能
void BSP_ADS1115_Default_Port(void)
{
	HAL_IIC2_Default();
}

//接口函数-ADS1115 IIC多字节写入函数
#define BSP_ADS1115_MultipleWrite_Port(star_addr,num,SlaveAddress,send_buf)    HAL_IIC2_MultipleWrite(star_addr,num,SlaveAddress,send_buf)

//接口函数-ADS1115 IIC多字节读取函数
#define BSP_ADS1115_MultipleRead_Port(star_addr,num,SlaveAddress,recv_buf)   HAL_IIC2_MultipleRead(star_addr,num,SlaveAddress,recv_buf)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
 ************************************************************
 *  名称：	BSP_ADS1115_ReadConfig()
 *  功能：	读取Config寄存器
 *	输入：  *value-读出数据的指针
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_ReadConfig(u16 *value)
{
	u8 config_data[2];
	
	if(true == BSP_ADS1115_MultipleRead_Port(0x01,2,BSP_ADS1115_ADDR,config_data))
	{
		*value = (((u16)config_data[0]<<8)+config_data[1]);
		return true;
	}
	else
	{
		*value = 0;
		return false;
	}
}

/*
 ************************************************************
 *  名称：	BSP_ADS1115_ReadConfig()
 *  功能：	写入Config寄存器
 *	输入：  value-写入数值
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_WriteConfig(u16 value)
{
	u8 config_data[2];
	
	config_data[0] = (u8)(0x00FF & (value>>8));	//高位
	config_data[1] = (u8)(0x00FF & value);	//低位
	
	return BSP_ADS1115_MultipleWrite_Port(0x01,2,BSP_ADS1115_ADDR,config_data);
}

/*
 ************************************************************
 *  名称：	BSP_ADS1115_ReadConvert()
 *  功能：	读取Convert寄存器
 *	输入：  *value-读出数据的指针
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_ReadConvert(s16 *value)
{
	u8 convert_data[2];
	
	if(true == BSP_ADS1115_MultipleRead_Port(0x00,2,BSP_ADS1115_ADDR,convert_data))
	{
		*value = (s16)(((u16)convert_data[0]<<8) | (u16)convert_data[1]);
		return true;
	}
	else
	{
		*value = 0;
		return false;
	}
}

/*
 ************************************************************
 *  名称：	BSP_ADS1115_SetRate()
 *  功能：	ADS1115 设定数据速率
 *	输入：  0-8SPS
            1-16SPS
            2-32SPS
            3-64SPS
            4-128SPS
            5-250SPS
            6-475SPS
            7-860SPS
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_SetRate(u8 rate)
{
	u16 temp;

	if(rate > 7) return false;

	if(true == BSP_ADS1115_ReadConfig(&temp))
	{
		temp &= (~0x00E0);	//5-7位
		temp |= ((u16)rate<<5);
		
		return BSP_ADS1115_WriteConfig(temp);
	}
	else
	{
		return false;
	}
}

/*
 ************************************************************
 *  名称：	BSP_ADS1115_SetMode()
 *  功能：	ADS1115 设定工作方式
 *	输入：  0-连续转换模式  1-关断模式
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_SetMode(u8 mode)
{
	u16 temp;
	
	if(mode >1)	return false;
	
	if(true == BSP_ADS1115_ReadConfig(&temp))
	{

		temp &= (~0x0100);	//8位
		temp |= ((u16)mode<<8);

		return BSP_ADS1115_WriteConfig(temp);
	}
	else
	{
		return false;
	}
}

/*
 ************************************************************
 *  名称：	BSP_ADS1115_SetGain()
 *  功能：	ADS1115 设定可编程增益
 *	输入：  0 +-6.144V
            1 +-4.096V
            2 +-2.048V
            3 +-1.024V
						4 +-0.512V
						5 +-0.256V
						6 +-0.256V
            7 +-0.256V
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_SetGain(u8 gain)
{
	u16 temp;

	if(gain > 7) return false;

	if(true == BSP_ADS1115_ReadConfig(&temp))
	{
		temp &= (~0x0E00);	//9-11位
		temp |= ((u16)gain<<9);
		
		return BSP_ADS1115_WriteConfig(temp);
	}
	else
	{
		return false;
	}
}

/*
 ************************************************************
 *  名称：	BSP_ADS1115_SetChannel()
 *  功能：	ADS1115 设定工作通道(非差分)
 *	输入：  0-AIN0
            1-AIN1
            2-AIN2
            3-AIN3
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_SetChannel(u8 channel)
{
	u16 temp;

	if(channel > 3)	return false;

	if(true == BSP_ADS1115_ReadConfig(&temp))
	{
		temp &= (~0x7000);	//12-14
		temp |= 0x4000;	//14位置1
		temp |= ((u16)channel<<12);

		return BSP_ADS1115_WriteConfig(temp);
	}
	else
	{
		return false;
	}
}


/*
 ************************************************************
 *  名称：	BSP_ADS1115_Init()
 *  功能：	ADS1115 初始化
 *	输入：  无
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_Init(void)
{
	BSP_ADS1115_Init_Port();
	
	if(false == BSP_ADS1115_SetRate(4))	return false;	//128SPS
	
	if(false == BSP_ADS1115_SetMode(0))	return false;	//连续转换模式
		
	if(false == BSP_ADS1115_SetGain(2)) return false;	//+-2.048V
	
	if(false == BSP_ADS1115_SetChannel(0)) return false;	//转换通道0(非差分)
	
	return true;
}

/*
 ************************************************************
 *  名称：	BSP_ADS1115_Default()
 *  功能：	ADS1115 设定为关断模式并失能片上外设
 *	输入：  无
 *	输出：  true-成功  false-失败
 ************************************************************
*/
bool BSP_ADS1115_Default(void)
{
	if(true == BSP_ADS1115_SetMode(1))
	{
		BSP_ADS1115_Default_Port();
		return true;
	}
	else
	{
		return false;
	}
}












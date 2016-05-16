#ifndef __BSP_ADS1115_H__
#define __BSP_ADS1115_H__

/*----添加需要包含的头文件----*/
//////////////////////////////////////////////////////////////////
#include "hal_iic2.h"
//////////////////////////////////////////////////////////////////


bool BSP_ADS1115_ReadConfig(u16 *value);
bool BSP_ADS1115_WriteConfig(u16 value);
bool BSP_ADS1115_ReadConvert(s16 *value);

bool BSP_ADS1115_SetRate(u8 rate);
bool BSP_ADS1115_SetMode(u8 mode);
bool BSP_ADS1115_SetChannel(u8 channel);
bool BSP_ADS1115_Init(void);
bool BSP_ADS1115_Default(void);




#endif


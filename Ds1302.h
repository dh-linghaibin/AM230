#ifndef __DS1302_H
#define __DS1302_H

#include "Type.h"

/**********************************************函数定义***************************************************** 
* 函数名称: void Ds1302Init(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
************************************************************************************************************/ 
void Ds1302Init(void);
void Ds1302GetTime(void);
void Ds1302SetTime(u8 set_hour, u8 set_min);
u8 Ds1302GetTimeKeep(u8 cmd);
#endif


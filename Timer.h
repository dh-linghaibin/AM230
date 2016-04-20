

#ifndef TIMER_H
#define TIMER_H

#include "Type.h"

void TimerInit(void);
u8 TimerGetTimeFlag(void);
void TimerClearTimeFlag(void);
u16 TimerGetSpeed(void);
/**********************************************函数定义***************************************************** 
* 函数名称: void TimerSetTimeFlah(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 设置打开值  
* 作    者: by lhb_steven
* 日    期: 2016/4/3
************************************************************************************************************/ 
void TimerSetTimeFlah(u8 cmd);
/**********************************************函数定义***************************************************** 
* 函数名称: u16 TimerGetTimeFlash(void) 
* 输入参数: void 
* 返回参数: u16  
* 功    能: 获取时间值  
* 作    者: by lhb_steven
* 日    期: 2016/4/3
************************************************************************************************************/ 
u16 TimerGetTimeFlash(void);

#endif




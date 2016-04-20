#ifndef __MENU_H
#define __MENU_H

#include "Type.h"
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
************************************************************************************************************/ 
void MenuInit(void);
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuSet(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 菜单操作  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
************************************************************************************************************/ 
void MenuSet(u8 cmd);
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuFlash(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单动画服务函数  
* 作    者: by lhb_steven
* 日    期: 2016/3/30
************************************************************************************************************/ 
void MenuFlash(void);
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuJudgeTime(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能: 判断时间  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
************************************************************************************************************/ 
u8 MenuJudgeTime(void);

#endif


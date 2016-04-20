/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：main.c
*
* 文件标识：lhb
* 摘    要：香水下MINI板子
*
*
* 当前版本：v1.0
* 作    者：lhb Steven
* 完成日期：2016/3/28
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Type.h"
#include "Eeprom.h"
#include "Sys.h"
#include "Lcd.h"
#include "Ds1302.h"
#include "Moto.h"
#include "Music.h"
#include "Button.h"
#include "Menu.h"
#include "Timer.h"

int main(void) {
    SysInit();
    EepromInit();
    ButtonInit();
    MusicInit();
    MotoInit();
    Ds1302Init();
    LcdInit();
    TimerInit();
    MenuInit();
    INTEN
    while(1) {
        MenuSet(ButtonRead());
        if(TimerGetTimeFlag() > 1) {
            TimerClearTimeFlag();
        }
        MenuFlash();//菜单动画服务
        MusicServerTime();//蜂鸣器时间服务
    }
}

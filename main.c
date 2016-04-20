/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�main.c
*
* �ļ���ʶ��lhb
* ժ    Ҫ����ˮ��MINI����
*
*
* ��ǰ�汾��v1.0
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/28
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
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
        MenuFlash();//�˵���������
        MusicServerTime();//������ʱ�����
    }
}

/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�Menu.h
*
* �ļ���ʶ��lhb
* ժ    Ҫ���˵��ں�
*
*
* ��ǰ�汾��v1.0
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/29
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Menu.h"
#include "Ds1302.h"
#include "Lcd.h"
#include "Music.h"
#include "Timer.h"
#include "Moto.h"
#include "Eeprom.h"

typedef struct DataNode
{
    u8 mode;//ģʽ
    u8 work_run_time;//����ʱ��
    u8 work_stop_time;//��Ϣʱ��
    u8 start_time_h;//��ʼʱ�� Сʱ
    u8 start_time_m;//��ʼʱ�� ����
    u8 end_time_h;//����ʱ�� Сʱ
    u8 end_time_m;//����ʱ�� ����
    u8 time_h;//Сʱ
    u8 time_m;//����
    u8 last_time;//�ϴ�ʱ��
    u8 run_time;//�Ѿ����˶���ʱ��
    u8 out;//����˳�
} tDataNode;
/***********************************************��������*****************************************************
* ��    ��: caidan  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
************************************************************************************************************/ 
static tDataNode menu;
/**********************************************��������***************************************************** 
* ��������: void MenuInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: �˵���ʼ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/29
************************************************************************************************************/ 
/*
����������
10:�Ƿ��ǵ�һ���̵�
11:��13:����ʱ�� Сʱ
14:����ʱ�� ����ʼʱ�� Сʱ
12:��ʼʱ�� ����

15:����ʱ�� ����
16:��Ϣʱ�� ����
*/
void MenuInit(void) {
    Ds1302GetTime();
    menu.time_h = Ds1302GetTimeKeep(2);//��ȡʱ��
    menu.time_m = Ds1302GetTimeKeep(1);
    LcdSetNum( menu.time_h/10,menu.time_h%10,
               menu.time_m/10,menu.time_m%10);
    LcdSetPoint(1);
    if(EepromRead(10) != 0x55) {
        EepromWrite(10,0x55);//ֻ�е�һ�ο���
        EepromWrite(11,23);
        EepromWrite(12,30);
        EepromWrite(13,23);
        EepromWrite(14,30);
        EepromWrite(15,3);
        EepromWrite(16,3);
    }
    menu.mode = 1;
    menu.last_time = 80;
    menu.run_time = 0;
    menu.start_time_h = EepromRead(11);
    menu.start_time_m = EepromRead(12);
    menu.end_time_h = EepromRead(13);
    menu.end_time_m = EepromRead(14);
    menu.work_run_time = EepromRead(15);
    menu.work_stop_time = EepromRead(16);
    //������� ��һ�� 10 
}
/**********************************************��������***************************************************** 
* ��������: void MenuSet(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��: �˵�����  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/29
************************************************************************************************************/ 
/*
don't just codeing
menu.mode ״̬��
0���رձ��� ������
1���򿪱��� ���� ��ʾ����ʱ��
2������S1���� ��ʾ��ʼʱ�䣺���ʱ��������ÿ�ʼʱ�䣺Сʱ �̰�S2 S3����
3���̰�S1���� ��ʾ��ʼʱ�䣺���ʱ��������ÿ�ʼʱ�䣺���� �̰�S2 S3����
4���̰�S1���� ��ʾ����ʱ�䣺���ʱ��������ý���ʱ�䣺Сʱ �̰�S2 S3����
5���̰�S1���� ��ʾ����ʱ�䣺���ʱ��������ý���ʱ�䣺���� �̰�S2 S3����
6���̰�S1���� ��ʾ����ʱ��: ��λ���� exp:(on : 02) �̰�S2 S3����
7���̰�S1���� ��ʾ��Ϣʱ�䣺��λ���� exp:(0f : 03) �̰�S2 S3����
8: �̰�S1���� ��ʾ����ʱ�䣺���ʱ���������Сʱ���̰�S2 S3����
9: �̰�S1���� ��ʾ����ʱ�䣺���ʱ��������÷�װ���̰�S2 S3����
-------�ٴζ̰�S1���� 1 ��ʾ����ʱ��
---��2----9֮�ڳ���S1�˳� �����ᱣ��������õ�����
*/
void MenuSet(u8 cmd) { 
    switch(cmd) {
        case 0x01://�������룬����һ��
            MusicSet(20000);//�������볤��һ��
            if( (menu.mode == 1) || (menu.mode == 0) ) {//����ʾ����ʱ���ģʽ�±�����
                menu.mode = 2;
                LcdBcakLedSet(LCDBACKEN);//�򿪱���
                //��ʾ��ʼʱ��
                LcdSetPoint(1);//��ʾС����
                LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                          menu.start_time_m/10,menu.start_time_m%10);
                TimerSetTimeFlah(0);
            } else {
                menu.mode = 1;//�ص�����ʱ��
            }
            menu.out = 0;
            break;
        case 0x10:
            switch( menu.mode ) {
                case 0:
                    LcdBcakLedSet(LCDBACKEN);//�رձ���
                    menu.mode = 1;//��������ģʽ
                    TimerSetTimeFlah(0);
                    menu.run_time = 0;
                    if(MenuJudgeTime() == 0x01) {
                        MotoSet(MOTOEN);
                    }
                    menu.last_time = menu.time_m + 1;
                    break;
                case 1:
                    LcdBcakLedSet(LCDBACKOF);//�򿪱���
                    menu.mode = 0;//����ػ�ģʽ
                    MotoSet(MOTOOF);
                    //ֹͣ����
                    TimerSetTimeFlah(0);
                    menu.run_time = 0;
                    menu.last_time = menu.time_m + 1;
                    break;
                case 2:
                    menu.mode = 3;
                    //���濪ʼʱ�� Сʱ
                    EepromWrite(11,menu.start_time_h);
                    break;
                case 3:
                    menu.mode = 4;
                    //���濪ʼʱ�� ����
                    EepromWrite(12,menu.start_time_m);
                    break;
                case 4:
                    menu.mode = 5;
                    //�������ʱ�� Сʱ
                    EepromWrite(13,menu.end_time_h);
                    break;
                case 5:
                    menu.mode = 6;
                    //�������ʱ�� ����
                    EepromWrite(14,menu.end_time_m);
                    break;
                case 6:
                    menu.mode = 7;
                    //���湤��ʱ��
                    EepromWrite(15,menu.work_run_time);
                    break;
                case 7:
                    menu.mode = 8;
                    //��Ϣʱ��
                    EepromWrite(16,menu.work_stop_time);
                    break;
                case 8:
                    menu.mode = 9;
                    //���ñ���ʱ�� ��Сʱ
                    Ds1302SetTime(menu.time_h,menu.time_m);
                    break;
                case 9:
                    menu.mode = 1;//���뱱��ʱ��
                    //���ñ���ʱ�� ������
                    Ds1302SetTime(menu.time_h,menu.time_m);
                    break;
                default:
                    break;
            }
            menu.out = 0;
            MusicSet(10000);
            break;
        case 0x11:
            switch( menu.mode ) {
                case 0://�ر�
                    break;
                case 1://����
                    break;
                case 2://��˸��ʼСʱ
                    if(menu.start_time_h < 23) {
                        menu.start_time_h++;
                    } else {
                        menu.start_time_h = 0;
                    }
                    break;
                case 3://��˸��ʼ����
                    if(menu.start_time_m < 59) {
                        menu.start_time_m++;
                    } else {
                        menu.start_time_m = 0;
                    }
                    break;
                case 4://��˸����Сʱ
                    if(menu.end_time_h < 23) {
                        menu.end_time_h++;
                    } else {
                        menu.end_time_h = 0;
                    } 
                    break;
                case 5://��˸��������
                    if(menu.end_time_m < 59) {
                        menu.end_time_m++;
                    } else {
                        menu.end_time_m = 0;
                    }
                    break;
                case 6://��˸����ʱ��
                    if(menu.work_run_time < 60) {
                        menu.work_run_time++;
                    } else {
                        menu.work_run_time = 1;
                    }
                    break;
                case 7://��˸��Ϣʱ��
                    if(menu.work_stop_time < 60) {
                        menu.work_stop_time++;
                    } else {
                        menu.work_stop_time = 1;
                    }
                    break;
                case 8://��˸����ʱ��Сʱ
                    if(menu.time_h < 23) {
                        menu.time_h++;
                    } else {
                        menu.time_h = 0;
                    }
                    break;
                case 9://��˸����ʱ��Сʱ
                    if(menu.time_m < 59) {
                        menu.time_m++;
                    } else {
                        menu.time_m = 0;
                    }
                    break;
                default:
                    break;
            }
            menu.out = 0;
            MusicSet(10000);
            break;
        case 0x12:
            switch( menu.mode ) {
                case 0://�ر�
                    break;
                case 1://����
                    break;
                case 2://��˸��ʼСʱ
                    if(menu.start_time_h > 0) {
                        menu.start_time_h--;
                    } else {
                        menu.start_time_h = 23;
                    }
                    break;
                case 3://��˸��ʼ����
                    if(menu.start_time_m > 0) {
                        menu.start_time_m--;
                    } else {
                        menu.start_time_m = 59;
                    }
                    break;
                case 4://��˸����Сʱ
                    if(menu.end_time_h > 0) {
                        menu.end_time_h--;
                    } else {
                        menu.end_time_h = 23;
                    }
                    break;
                case 5://��˸��������
                    if(menu.end_time_m > 0) {
                        menu.end_time_m--;
                    } else {
                        menu.end_time_m = 59;
                    }
                    break;
                case 6://��˸����ʱ��
                    if(menu.work_run_time > 1) {
                        menu.work_run_time--;
                    } else {
                        menu.work_run_time = 60;
                    }
                    break;
                case 7://��˸��Ϣʱ��
                    if(menu.work_stop_time > 1) {
                        menu.work_stop_time--;
                    } else {
                        menu.work_stop_time = 59;
                    }
                    break;
                case 8://��˸����ʱ��Сʱ
                    if(menu.time_h > 0) {
                        menu.time_h--;
                    } else {
                        menu.time_h = 23;
                    }
                    break;
                case 9://��˸����ʱ�����
                    if(menu.time_m > 0) {
                        menu.time_m--;
                    } else {
                        menu.time_m = 59;
                    }
                    break;
                default:
                    break;
            }
            menu.out = 0;
            MusicSet(10000);
            break;
        default:
            break;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuJudgeTime(void) 
* �������: void 
* ���ز���: u8  
* ��    ��: �ж�ʱ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/29
************************************************************************************************************/ 
u8 MenuJudgeTime(void) { 
    if( (menu.start_time_h < menu.end_time_h) || ( (menu.start_time_h == menu.end_time_h) && (menu.start_time_m <= menu.end_time_m) ) ) {
        //��ʼʱ��С�ڽ���ʱ��
        if( ( (menu.time_h > menu.start_time_h) || (menu.time_h == menu.start_time_h) && (menu.time_m >= menu.start_time_m) )
        && ( (menu.time_h < menu.end_time_h) || (menu.time_h == menu.end_time_h) && (menu.time_m <= menu.end_time_m) ) ) {
        //��ʱ�����
            return 0x01;
        } else {
        //����ʱ�����
            return 0x00;
        }
    } else {
        // ��ʼʱ����ڽ���ʱ��, ��������
        if( ( (menu.time_h > menu.start_time_h) || (menu.time_h == menu.start_time_h) && (menu.time_m >= menu.start_time_m) )
        || ( (menu.time_h < menu.end_time_h) || (menu.time_h == menu.end_time_h) && (menu.time_m <= menu.end_time_m) ) ) {
        //��ʱ�����
            return 0x01;
        } else {
        //����ʱ�����
            return 0x00;
        }
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuFlash(void) 
* �������: void 
* ���ز���: void  
* ��    ��: �˵�����������  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/30
************************************************************************************************************/ 
void MenuFlash(void) { 
    static u16 count = 0;
    static u8 count_s = 0;
    if(count_s < 100) {
        count_s++;
    } else {
        count_s = 0;
        if(count < 600) {
            count++;
        } else {
            static u8 flash_flag = 0;
            count = 0;
            //�������� һ��С�߳�
            if(flash_flag == 0) {
                switch( menu.mode ) {
                    case 0://�ر�
                        break;
                    case 1://����
                        Ds1302GetTime();
                        menu.time_h = Ds1302GetTimeKeep(2);//��ȡʱ��
                        menu.time_m = Ds1302GetTimeKeep(1);
                        LcdSetNum( menu.time_h/10,menu.time_h%10,
                                   menu.time_m/10,menu.time_m%10);
                        LcdSetPoint(1);
                        break;
                    case 2://��˸��ʼСʱ
                        LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                                  menu.start_time_m/10,menu.start_time_m%10);
                        break;
                    case 3://��˸��ʼ����
                        LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                                  menu.start_time_m/10,menu.start_time_m%10);
                        break;
                    case 4://��˸����Сʱ
                        LcdSetNum(menu.end_time_h/10,menu.end_time_h%10,
                                  menu.end_time_m/10,menu.end_time_m%10);
                        break;
                    case 5://��˸��������
                        LcdSetNum(menu.end_time_h/10,menu.end_time_h%10,
                                  menu.end_time_m/10,menu.end_time_m%10);
                        break;
                    case 6://��˸����ʱ��
                        LcdSetNum(0,10,menu.work_run_time/10,
                                  menu.work_run_time%10);
                        break;
                    case 7://��˸��Ϣʱ��
                        LcdSetNum(0,11,menu.work_stop_time/10,
                                  menu.work_stop_time%10);
                        break;
                    case 8://��˸����ʱ�� Сʱ
                        LcdSetNum(menu.time_h/10,menu.time_h%10,
                                  menu.time_m/10,menu.time_m%10);
                        break;
                    case 9://��˸����ʱ�� ����
                        LcdSetNum(menu.time_h/10,menu.time_h%10,
                                  menu.time_m/10,menu.time_m%10);
                        break;
                    default:
                        break;
                }
                flash_flag = 1;
            } else {
                switch( menu.mode ) {
                    case 0://�ر�
                        break;
                    case 1://����
                        //�ж��Ƿ��ڹ���ʱ��
                        LcdSetPoint(0);//��˸���
                        if(MenuJudgeTime() == 0x01) {
                            if(menu.last_time != menu.time_m) {
                                menu.last_time = menu.time_m;
                                if(menu.run_time < menu.work_run_time) {
                                    //ʱ�仹û������
                                    MotoSet(MOTOEN);
                                } else if( menu.run_time < (menu.work_stop_time+menu.work_run_time)) {
                                    //ֹͣ����
                                    MotoSet(MOTOOF);
                                } else {
                                    menu.run_time = 0;
                                    MotoSet(MOTOEN);
                                }
                                menu.run_time++;
                            }
//                            if(TimerGetTimeFlash() < menu.work_run_time) {
//                                //ʱ�仹û������
//                                MotoSet(MOTOEN);
//                            } else if(TimerGetTimeFlash() < 
//                                      (menu.work_stop_time+menu.work_run_time) ) {
//                                //ֹͣ����
//                                MotoSet(MOTOOF);
//                            } else {
//                                //���¿�ʼ
//                                TimerSetTimeFlah(0);
//                            }
                        } else {
                            //ֹͣ����
                            MotoSet(MOTOOF);
                            menu.last_time = menu.time_m + 1;
                            menu.run_time = 0;
                        }
                        break;
                    case 2://��˸��ʼСʱ
                        LcdSetNum(12,12,
                                  menu.start_time_m/10,menu.start_time_m%10);
                        break;
                    case 3://��˸��ʼ����
                        LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                                  12,12);
                        break;
                    case 4://��˸����Сʱ
                        LcdSetNum(12,12,
                                  menu.end_time_m/10,menu.end_time_m%10);
                        break;
                    case 5://��˸��������
                        LcdSetNum(menu.end_time_h/10,menu.end_time_h%10,
                                  12,12);
                        break;
                    case 6://��˸����ʱ��
                        LcdSetNum(0,10,12,
                                  12);
                        break;
                    case 7://��˸��Ϣʱ��
                        LcdSetNum(0,11,12,
                                  12);
                        break;
                    case 8://��˸����ʱ�� Сʱ
                        LcdSetNum(12,12,
                                  menu.time_m/10,menu.time_m%10);
                        break;
                    case 9://��˸����ʱ�� ����
                        LcdSetNum(menu.time_h/10,menu.time_h%10,
                                  12,12);
                        break;
                    default:
                        break;
                }
                flash_flag = 0;
            }
            //�Զ��Ƴ�
            if(menu.mode >= 2) {
                if(menu.out < 20) {
                    menu.out++;
                } else {
                    menu.out = 0;
                    menu.mode = 1;//�ص�����ʱ��
                }
            }
        }
    }
}










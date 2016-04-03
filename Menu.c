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
void MenuInit(void) {
    Ds1302GetTime();
    LcdSetNum( Ds1302GetTimeKeep(2)/10,Ds1302GetTimeKeep(2)%10,
              Ds1302GetTimeKeep(1)/10,Ds1302GetTimeKeep(1)%10);
    LcdSetPoint(1);
    
    menu.start_time_h = 10;
    menu.start_time_m = 00;
    menu.end_time_h = 2;
    menu.end_time_m = 00;
    menu.time_h = 2;
    menu.time_m = 01;
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
            MusicSet(60);//�������볤��һ��
            if(menu.mode == 1) {//����ʾ����ʱ���ģʽ�±�����
                menu.mode = 2;
                //��ʾ��ʼʱ��
                LcdSetPoint(1);//��ʾС����
                LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                          menu.start_time_m/10,menu.start_time_m%10);
            }
            break;
        case 0x10:
            switch( menu.mode ) {
                case 0:
                    LcdBcakLedSet(LCDBACKOF);//�رձ���
                    menu.mode = 1;//��������ģʽ
                    break;
                case 1:
                    LcdBcakLedSet(LCDBACKEN);//�򿪱���
                    menu.mode = 0;//����ػ�ģʽ
                    break;
                case 2:
                    menu.mode = 3;
                    break;
                case 3:
                    menu.mode = 4;
                    break;
                case 4:
                    menu.mode = 5;
                    break;
                case 5:
                    menu.mode = 6;
                    break;
                case 6:
                    menu.mode = 7;
                    break;
                case 7:
                    menu.mode = 8;
                    break;
                case 8:
                    menu.mode = 9;
                    break;
                case 9:
                    menu.mode = 1;//���뱱��ʱ��
                    break;
                default:
                    break;
            }
            MusicSet(20);
            break;
        case 0x11:
            switch( menu.mode ) {
                case 0://�ر�
                    break;
                case 1://����
                    break;
                case 2://��˸��ʼСʱ
                    if(menu.start_time_h < 24) {
                        menu.start_time_h++;
                    } else {
                        menu.start_time_h = 0;
                    }
                    break;
                case 3://��˸��ʼ����
                    if(menu.start_time_m < 60) {
                        menu.start_time_m++;
                    } else {
                        menu.start_time_m = 0;
                    }
                    break;
                case 4://��˸����Сʱ
                    if(menu.end_time_h < 24) {
                        menu.end_time_h++;
                    } else {
                        menu.end_time_h = 0;
                    } 
                    break;
                case 5://��˸��������
                    if(menu.end_time_m < 60) {
                        menu.end_time_m++;
                    } else {
                        menu.end_time_m = 0;
                    }
                    break;
                case 6://��˸����ʱ��
                    if(menu.work_run_time < 99) {
                        menu.work_run_time++;
                    } else {
                        menu.work_run_time = 0;
                    }
                    break;
                case 7://��˸��Ϣʱ��
                    if(menu.work_stop_time < 99) {
                        menu.work_stop_time++;
                    } else {
                        menu.work_stop_time = 0;
                    }
                    break;
                case 8://��˸����ʱ��Сʱ
                    if(menu.time_h < 24) {
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
            MusicSet(20);
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
                        menu.end_time_h = 24;
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
                        menu.end_time_h = 24;
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
                    if(menu.work_run_time > 0) {
                        menu.work_run_time--;
                    } else {
                        menu.work_run_time = 99;
                    }
                    break;
                case 7://��˸��Ϣʱ��
                    if(menu.work_stop_time > 0) {
                        menu.work_stop_time--;
                    } else {
                        menu.work_stop_time = 99;
                    }
                    break;
                case 8://��˸����ʱ��Сʱ
                    if(menu.time_h > 0) {
                        menu.time_h--;
                    } else {
                        menu.time_h = 24;
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
            MusicSet(20);
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
    if(count < 10000) {
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
                    if(MenuJudgeTime() == 0x01) {
                    
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
    }
}










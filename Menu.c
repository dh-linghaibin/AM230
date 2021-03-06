/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：Menu.h
*
* 文件标识：lhb
* 摘    要：菜单内核
*
*
* 当前版本：v1.0
* 作    者：lhb Steven
* 完成日期：2016/3/29
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
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
    u8 mode;//模式
    u8 work_run_time;//工作时间
    u8 work_stop_time;//休息时间
    u8 start_time_h;//开始时间 小时
    u8 start_time_m;//开始时间 分钟
    u8 end_time_h;//结束时间 小时
    u8 end_time_m;//结束时间 分钟
    u8 time_h;//小时
    u8 time_m;//分钟
    u8 last_time;//上次时间
    u8 run_time;//已经跑了多少时间
    u8 out;//多久退出
} tDataNode;
/***********************************************变量声明*****************************************************
* 功    能: caidan  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
static tDataNode menu;
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
************************************************************************************************************/ 
/*
掉电记忆分配
10:是否是第一次商店
11:开13:结束时间 小时
14:结束时间 分钟始时间 小时
12:开始时间 分钟

15:工作时间 分钟
16:休息时间 分钟
*/
void MenuInit(void) {
    Ds1302GetTime();
    menu.time_h = Ds1302GetTimeKeep(2);//获取时间
    menu.time_m = Ds1302GetTimeKeep(1);
    LcdSetNum( menu.time_h/10,menu.time_h%10,
               menu.time_m/10,menu.time_m%10);
    LcdSetPoint(1);
    if(EepromRead(10) != 0x55) {
        EepromWrite(10,0x55);//只有第一次开机
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
    //储存分配 第一次 10 
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuSet(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 菜单操作  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
************************************************************************************************************/ 
/*
don't just codeing
menu.mode 状态表
0：关闭背光 不工作
1：打开背光 工作 显示北京时间
2：长按S1进入 显示开始时间：这个时候可以设置开始时间：小时 短按S2 S3操作
3：短按S1进入 显示开始时间：这个时候可以设置开始时间：分钟 短按S2 S3操作
4：短按S1进入 显示结束时间：这个时候可以设置结束时间：小时 短按S2 S3操作
5：短按S1进入 显示结束时间：这个时候可以设置结束时间：分钟 短按S2 S3操作
6；短按S1进入 显示工作时间: 单位：分 exp:(on : 02) 短按S2 S3操作
7：短按S1进入 显示休息时间：单位：分 exp:(0f : 03) 短按S2 S3操作
8: 短按S1进入 显示北京时间：这个时候可以设置小时：短按S2 S3操作
9: 短按S1进入 显示北京时间：这个时候可以设置封装：短按S2 S3操作
-------再次短按S1进入 1 显示北京时间
---在2----9之内长按S1退出 并不会保存这次设置的数据
*/
void MenuSet(u8 cmd) { 
    switch(cmd) {
        case 0x01://长按进入，长叫一声
            MusicSet(20000);//长按进入长叫一声
            if( (menu.mode == 1) || (menu.mode == 0) ) {//在显示北京时间的模式下被按下
                menu.mode = 2;
                LcdBcakLedSet(LCDBACKEN);//打开背光
                //显示开始时间
                LcdSetPoint(1);//显示小数点
                LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                          menu.start_time_m/10,menu.start_time_m%10);
                TimerSetTimeFlah(0);
            } else {
                menu.mode = 1;//回到北京时间
            }
            menu.out = 0;
            break;
        case 0x10:
            switch( menu.mode ) {
                case 0:
                    LcdBcakLedSet(LCDBACKEN);//关闭背光
                    menu.mode = 1;//进入运行模式
                    TimerSetTimeFlah(0);
                    menu.run_time = 0;
                    if(MenuJudgeTime() == 0x01) {
                        MotoSet(MOTOEN);
                    }
                    menu.last_time = menu.time_m + 1;
                    break;
                case 1:
                    LcdBcakLedSet(LCDBACKOF);//打开背光
                    menu.mode = 0;//进入关机模式
                    MotoSet(MOTOOF);
                    //停止工作
                    TimerSetTimeFlah(0);
                    menu.run_time = 0;
                    menu.last_time = menu.time_m + 1;
                    break;
                case 2:
                    menu.mode = 3;
                    //保存开始时间 小时
                    EepromWrite(11,menu.start_time_h);
                    break;
                case 3:
                    menu.mode = 4;
                    //保存开始时间 分钟
                    EepromWrite(12,menu.start_time_m);
                    break;
                case 4:
                    menu.mode = 5;
                    //保存结束时间 小时
                    EepromWrite(13,menu.end_time_h);
                    break;
                case 5:
                    menu.mode = 6;
                    //保存结束时间 分钟
                    EepromWrite(14,menu.end_time_m);
                    break;
                case 6:
                    menu.mode = 7;
                    //保存工作时间
                    EepromWrite(15,menu.work_run_time);
                    break;
                case 7:
                    menu.mode = 8;
                    //休息时间
                    EepromWrite(16,menu.work_stop_time);
                    break;
                case 8:
                    menu.mode = 9;
                    //设置北京时间 ：小时
                    Ds1302SetTime(menu.time_h,menu.time_m);
                    break;
                case 9:
                    menu.mode = 1;//进入北京时间
                    //设置北京时间 ：分钟
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
                case 0://关闭
                    break;
                case 1://开机
                    break;
                case 2://闪烁开始小时
                    if(menu.start_time_h < 23) {
                        menu.start_time_h++;
                    } else {
                        menu.start_time_h = 0;
                    }
                    break;
                case 3://闪烁开始分钟
                    if(menu.start_time_m < 59) {
                        menu.start_time_m++;
                    } else {
                        menu.start_time_m = 0;
                    }
                    break;
                case 4://闪烁结束小时
                    if(menu.end_time_h < 23) {
                        menu.end_time_h++;
                    } else {
                        menu.end_time_h = 0;
                    } 
                    break;
                case 5://闪烁结束分钟
                    if(menu.end_time_m < 59) {
                        menu.end_time_m++;
                    } else {
                        menu.end_time_m = 0;
                    }
                    break;
                case 6://闪烁工作时间
                    if(menu.work_run_time < 60) {
                        menu.work_run_time++;
                    } else {
                        menu.work_run_time = 1;
                    }
                    break;
                case 7://闪烁休息时间
                    if(menu.work_stop_time < 60) {
                        menu.work_stop_time++;
                    } else {
                        menu.work_stop_time = 1;
                    }
                    break;
                case 8://闪烁北京时间小时
                    if(menu.time_h < 23) {
                        menu.time_h++;
                    } else {
                        menu.time_h = 0;
                    }
                    break;
                case 9://闪烁北京时间小时
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
                case 0://关闭
                    break;
                case 1://开机
                    break;
                case 2://闪烁开始小时
                    if(menu.start_time_h > 0) {
                        menu.start_time_h--;
                    } else {
                        menu.start_time_h = 23;
                    }
                    break;
                case 3://闪烁开始分钟
                    if(menu.start_time_m > 0) {
                        menu.start_time_m--;
                    } else {
                        menu.start_time_m = 59;
                    }
                    break;
                case 4://闪烁结束小时
                    if(menu.end_time_h > 0) {
                        menu.end_time_h--;
                    } else {
                        menu.end_time_h = 23;
                    }
                    break;
                case 5://闪烁结束分钟
                    if(menu.end_time_m > 0) {
                        menu.end_time_m--;
                    } else {
                        menu.end_time_m = 59;
                    }
                    break;
                case 6://闪烁工作时间
                    if(menu.work_run_time > 1) {
                        menu.work_run_time--;
                    } else {
                        menu.work_run_time = 60;
                    }
                    break;
                case 7://闪烁休息时间
                    if(menu.work_stop_time > 1) {
                        menu.work_stop_time--;
                    } else {
                        menu.work_stop_time = 59;
                    }
                    break;
                case 8://闪烁北京时间小时
                    if(menu.time_h > 0) {
                        menu.time_h--;
                    } else {
                        menu.time_h = 23;
                    }
                    break;
                case 9://闪烁北京时间分钟
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
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuJudgeTime(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能: 判断时间  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
************************************************************************************************************/ 
u8 MenuJudgeTime(void) { 
    if( (menu.start_time_h < menu.end_time_h) || ( (menu.start_time_h == menu.end_time_h) && (menu.start_time_m <= menu.end_time_m) ) ) {
        //开始时间小于结束时间
        if( ( (menu.time_h > menu.start_time_h) || (menu.time_h == menu.start_time_h) && (menu.time_m >= menu.start_time_m) )
        && ( (menu.time_h < menu.end_time_h) || (menu.time_h == menu.end_time_h) && (menu.time_m <= menu.end_time_m) ) ) {
        //在时间段内
            return 0x01;
        } else {
        //不在时间段内
            return 0x00;
        }
    } else {
        // 开始时间大于结束时间, 跨天的情况
        if( ( (menu.time_h > menu.start_time_h) || (menu.time_h == menu.start_time_h) && (menu.time_m >= menu.start_time_m) )
        || ( (menu.time_h < menu.end_time_h) || (menu.time_h == menu.end_time_h) && (menu.time_m <= menu.end_time_m) ) ) {
        //在时间段内
            return 0x01;
        } else {
        //不在时间段内
            return 0x00;
        }
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuFlash(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单动画服务函数  
* 作    者: by lhb_steven
* 日    期: 2016/3/30
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
            //服务这里 一个小线程
            if(flash_flag == 0) {
                switch( menu.mode ) {
                    case 0://关闭
                        break;
                    case 1://开机
                        Ds1302GetTime();
                        menu.time_h = Ds1302GetTimeKeep(2);//获取时间
                        menu.time_m = Ds1302GetTimeKeep(1);
                        LcdSetNum( menu.time_h/10,menu.time_h%10,
                                   menu.time_m/10,menu.time_m%10);
                        LcdSetPoint(1);
                        break;
                    case 2://闪烁开始小时
                        LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                                  menu.start_time_m/10,menu.start_time_m%10);
                        break;
                    case 3://闪烁开始分钟
                        LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                                  menu.start_time_m/10,menu.start_time_m%10);
                        break;
                    case 4://闪烁结束小时
                        LcdSetNum(menu.end_time_h/10,menu.end_time_h%10,
                                  menu.end_time_m/10,menu.end_time_m%10);
                        break;
                    case 5://闪烁结束分钟
                        LcdSetNum(menu.end_time_h/10,menu.end_time_h%10,
                                  menu.end_time_m/10,menu.end_time_m%10);
                        break;
                    case 6://闪烁工作时间
                        LcdSetNum(0,10,menu.work_run_time/10,
                                  menu.work_run_time%10);
                        break;
                    case 7://闪烁休息时间
                        LcdSetNum(0,11,menu.work_stop_time/10,
                                  menu.work_stop_time%10);
                        break;
                    case 8://闪烁北京时间 小时
                        LcdSetNum(menu.time_h/10,menu.time_h%10,
                                  menu.time_m/10,menu.time_m%10);
                        break;
                    case 9://闪烁北京时间 分钟
                        LcdSetNum(menu.time_h/10,menu.time_h%10,
                                  menu.time_m/10,menu.time_m%10);
                        break;
                    default:
                        break;
                }
                flash_flag = 1;
            } else {
                switch( menu.mode ) {
                    case 0://关闭
                        break;
                    case 1://开机
                        //判断是否在工作时间
                        LcdSetPoint(0);//闪烁光标
                        if(MenuJudgeTime() == 0x01) {
                            if(menu.last_time != menu.time_m) {
                                menu.last_time = menu.time_m;
                                if(menu.run_time < menu.work_run_time) {
                                    //时间还没到工作
                                    MotoSet(MOTOEN);
                                } else if( menu.run_time < (menu.work_stop_time+menu.work_run_time)) {
                                    //停止工作
                                    MotoSet(MOTOOF);
                                } else {
                                    menu.run_time = 0;
                                    MotoSet(MOTOEN);
                                }
                                menu.run_time++;
                            }
//                            if(TimerGetTimeFlash() < menu.work_run_time) {
//                                //时间还没到工作
//                                MotoSet(MOTOEN);
//                            } else if(TimerGetTimeFlash() < 
//                                      (menu.work_stop_time+menu.work_run_time) ) {
//                                //停止工作
//                                MotoSet(MOTOOF);
//                            } else {
//                                //重新开始
//                                TimerSetTimeFlah(0);
//                            }
                        } else {
                            //停止工作
                            MotoSet(MOTOOF);
                            menu.last_time = menu.time_m + 1;
                            menu.run_time = 0;
                        }
                        break;
                    case 2://闪烁开始小时
                        LcdSetNum(12,12,
                                  menu.start_time_m/10,menu.start_time_m%10);
                        break;
                    case 3://闪烁开始分钟
                        LcdSetNum(menu.start_time_h/10,menu.start_time_h%10,
                                  12,12);
                        break;
                    case 4://闪烁结束小时
                        LcdSetNum(12,12,
                                  menu.end_time_m/10,menu.end_time_m%10);
                        break;
                    case 5://闪烁结束分钟
                        LcdSetNum(menu.end_time_h/10,menu.end_time_h%10,
                                  12,12);
                        break;
                    case 6://闪烁工作时间
                        LcdSetNum(0,10,12,
                                  12);
                        break;
                    case 7://闪烁休息时间
                        LcdSetNum(0,11,12,
                                  12);
                        break;
                    case 8://闪烁北京时间 小时
                        LcdSetNum(12,12,
                                  menu.time_m/10,menu.time_m%10);
                        break;
                    case 9://闪烁北京时间 分钟
                        LcdSetNum(menu.time_h/10,menu.time_h%10,
                                  12,12);
                        break;
                    default:
                        break;
                }
                flash_flag = 0;
            }
            //自动推出
            if(menu.mode >= 2) {
                if(menu.out < 20) {
                    menu.out++;
                } else {
                    menu.out = 0;
                    menu.mode = 1;//回到北京时间
                }
            }
        }
    }
}










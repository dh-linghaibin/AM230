/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：button.c
*
* 文件标识：lhb
* 摘    要：按钮检测程序
*
*
* 当前版本：v1.0
* 作    者：lhb Steven
* 完成日期：2016/3/28
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Button.h"

#define BUTTIONS1 PC_IDR_IDR7
#define BUTTIONS2 PC_IDR_IDR6
#define BUTTIONS3 PC_IDR_IDR5

/**********************************************函数定义***************************************************** 
* 函数名称: void ButtonInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 按钮初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/28
************************************************************************************************************/ 
void ButtonInit(void) {
    //S3
    PC_DDR_DDR5 = 0;
    PC_CR1_C15 = 1;
    PC_CR2_C25 = 0; 
    //S2
    PC_DDR_DDR6 = 0;
    PC_CR1_C16 = 1;
    PC_CR2_C26 = 0; 
    //S1
    PC_DDR_DDR7 = 0;
    PC_CR1_C17 = 1;
    PC_CR2_C27 = 0; 
}
/**********************************************函数定义***************************************************** 
* 函数名称: u8 ButtonRead(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能: 读取按钮状态  
* 作    者: by lhb_steven
* 日    期: 2016/3/28
************************************************************************************************************/ 
u8 ButtonRead(void) { 
    static u16 count1 = 0;
    static u16 count1_small = 0;//长按进入
    static u16 count2 = 0;
    static u16 count3 = 0;
    //read s1
    if(BUTTIONS1 == 0) {
        if(count1_small < 50) {
            count1_small++;
        } else {
            count1_small = 0;
            if(count1 < 2000) {
                count1++;
                 if(count1 == 1999) {//这边还需要长按进入
                    return 0x01;
                }
            } else {
            }
        }
    } else {
        if( (count1 > 100) && (count1 < 1900) ) {//短按进入
            count1 = 0;
            return 0x10;
        }
        count1 = 0;
    }
    //read s2
    if(BUTTIONS2 == 0) {
        if(count2 < 5000) {
            count2++;
        } else {
        
        }
    } else {
        if(count2 > 1000) {
            count2 = 0;
            return 0x11;
        }
        count2 = 0;
    }
    //read s3
    if(BUTTIONS3 == 0) {
        if(count3 < 5000) {
            count3++;
        } else {
            
        }
    } else {
        if(count3 > 1000) {
            count3 = 0;
            return 0x12;
        }
        count3 = 0;
    }
    return 0x00;
}







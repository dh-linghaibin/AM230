/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�button.c
*
* �ļ���ʶ��lhb
* ժ    Ҫ����ť������
*
*
* ��ǰ�汾��v1.0
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/28
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Button.h"

#define BUTTIONS1 PC_IDR_IDR7
#define BUTTIONS2 PC_IDR_IDR6
#define BUTTIONS3 PC_IDR_IDR5

/**********************************************��������***************************************************** 
* ��������: void ButtonInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: ��ť��ʼ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/28
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
/**********************************************��������***************************************************** 
* ��������: u8 ButtonRead(void) 
* �������: void 
* ���ز���: u8  
* ��    ��: ��ȡ��ť״̬  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/28
************************************************************************************************************/ 
u8 ButtonRead(void) { 
    static u16 count1 = 0;
    static u16 count1_small = 0;//��������
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
                 if(count1 == 1999) {//��߻���Ҫ��������
                    return 0x01;
                }
            } else {
            }
        }
    } else {
        if( (count1 > 100) && (count1 < 1900) ) {//�̰�����
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







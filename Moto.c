/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�Moto.h
*
* �ļ���ʶ��lhb
* ժ    Ҫ���������
*
*
* ��ǰ�汾��v1.0
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/29
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Moto.h"
#define MOTO PA_ODR_ODR1
/**********************************************��������***************************************************** 
* ��������: void MotoInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: ��ʼ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/29
************************************************************************************************************/ 
void MotoInit(void) { 
    PA_DDR_DDR1 = 1;
    PA_CR1_C11 = 1;
    PA_CR2_C21 = 0;
    MotoSet(MOTOOF);
}
/**********************************************��������***************************************************** 
* ��������: void MotoSet(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��: �رջ�� 1���� 0:�ر�  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/29
************************************************************************************************************/ 
void MotoSet(u8 cmd) { 
    MOTO = cmd;
}


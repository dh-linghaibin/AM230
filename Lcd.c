/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�Lcd.c
*
* �ļ���ʶ��lhb
* ժ    Ҫ��HK1056B����
*
*
* ��ǰ�汾��v1.0
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/29
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Lcd.h"
#include "Delay.h"
#define LCDCS   PB_ODR_ODR4
#define LCDWR   PB_ODR_ODR5
#define LCDDATA PC_ODR_ODR3
#define LCDLED PD_ODR_ODR2
//dat �ĸ�cnt λд��HT1621���ȷ��͸�λ
static void SendBit_HL(u8 dat,u8 cnt)  {
	u8 i;
	for(i=0; i<cnt;i++) {
		if(dat&0x80) {
			LCDDATA = 1;
		} else {
			LCDDATA = 0;
		}
		LCDWR = 0;
		DelayUs(20);
		LCDWR = 1;
		dat <<= 1;
	}
}
//ָ��ģʽ
static void Sendcmd(u8 command)
{
	LCDCS = 0; 
	//д���־��"100"
	SendBit_HL(0x80,3); 
	//д�� 9 λ����,����ǰ 8 λΪ command ����,��� 1 λ����
	SendBit_HL(command,9);  
	LCDCS = 1;
}
//data�ĵ�cntλд��1621
static void SendBit_LH(u8 dat,u8 cnt) {
	u8 i;
	for(i=0; i<cnt;i++) {
		if(dat&0x01) {
			LCDDATA = 1;
		} else {
			LCDDATA = 0;
		}
		LCDWR = 0;
		DelayUs(20);
		LCDWR = 1;
		dat >>= 1;
	}
}

void HtlcdDisAll(void)
{
    u8  i;
	//ȫ��ʾ��Ļ
    LCDCS = 0;      
	//д���־��"101"
    SendBit_HL(0xa0,3);  
	//д�� 6 λ addr
    SendBit_HL(0,6); 
	//����д������
    for(i =0; i <32; i ++) 
    {
        SendBit_LH(0X00,4);
    }
    LCDCS = 1;
}

//0: 0x5f
//1��0x06
//2: 0x3d
//3: 0x2f
//4: 0x66
//5: 0x6b
//6: 0x7b
//7: 0x0e
//8: 0x7f
//9: 0x6f
//n: 0x5e
//f: 0x78

const u8 lcd_num[13] = {
0x5f,0x06,0x3d,0x2f,0x66,0x6b,0x7b,0x0e,0x7f,0x6f,0x5e,0x78,0x00
};

u8 Ht1621Tab[]=
{
0xff,0xff,0xff,0x5e,0x78,0x7f,0x0e,0x00
};
/***********************************************��������*****************************************************
* ��    ��: ����С�ı���־λ  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/30
************************************************************************************************************/ 
static u8 lcd_point_flag = 1;

void LcdSetNum(u8 data1,u8 data2,u8 data3,u8 data4) {
    Ht1621Tab[3] = lcd_num[data1];
    Ht1621Tab[4] = lcd_num[data2];
    Ht1621Tab[5] = lcd_num[data3];
    Ht1621Tab[6] = lcd_num[data4];
    if(lcd_point_flag == 0) {//����С���Ƿ��
        Ht1621Tab[5] &= 0x7f;
    } else {
        Ht1621Tab[5] |= 0x80;
    }
    LCDCS = 0;      
    //д���־��"101"
    SendBit_HL(0xa0,3);  
    //д�� 6 λ addr
    SendBit_HL(12,6); 
    for (u8 i=0;i<8;i++) {
        SendBit_HL(Ht1621Tab[i],8);
    }
    LCDCS = 1;
}
void LcdSetPoint(u8 cmd) {
    if(cmd == 0) {
        Ht1621Tab[5] &= 0x7f;
        lcd_point_flag = 0;
    } else {
        Ht1621Tab[5] |= 0x80;
        lcd_point_flag = 1;
    }
    LCDCS = 0;      
    //д���־��"101"
    SendBit_HL(0xa0,3);  
    //д�� 6 λ addr
    SendBit_HL(12,6); 
    for (u8 i=0;i<8;i++) {
        SendBit_HL(Ht1621Tab[i],8);
    }
    LCDCS = 1;
}
/**********************************************��������***************************************************** 
* ��������: void LcdInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: LCD��io��ʼ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/29
************************************************************************************************************/ 
void LcdInit(void) {
    //CS
    PB_DDR_DDR4 = 1;
    PB_CR1_C14 = 1;
    PB_CR2_C24 = 0; 
    //WR
    PB_DDR_DDR5 = 1;
    PB_CR1_C15 = 1;
    PB_CR2_C25 = 0; 
    //LCDDATATA
    PC_DDR_DDR3 = 1;
    PC_CR1_C13 = 1;
    PC_CR2_C23 = 0; 
    //BLE LED
    PD_DDR_DDR2 = 1;
    PD_CR1_C12 = 1;
    PD_CR2_C22 = 0; 
    LCDLED = 1;//OPEN LED
    Sendcmd(0x01); 
    Sendcmd(0x18);   
    Sendcmd(0x29);  
    Sendcmd(0x03);  
    HtlcdDisAll();
    //LcdSetNum(1,2,3,4);
    //LcdSetPoint(0);
}
/**********************************************��������***************************************************** 
* ��������: void LcdBcakLedSet(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��: �������  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/29
************************************************************************************************************/ 
void LcdBcakLedSet(u8 cmd) { 
    LCDLED = cmd;
}
















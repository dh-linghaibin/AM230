/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：Lcd.c
*
* 文件标识：lhb
* 摘    要：HK1056B驱动
*
*
* 当前版本：v1.0
* 作    者：lhb Steven
* 完成日期：2016/3/29
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Lcd.h"
#include "Delay.h"
#define LCDCS   PB_ODR_ODR4
#define LCDWR   PB_ODR_ODR5
#define LCDDATA PC_ODR_ODR3
#define LCDLED PD_ODR_ODR2
//dat 的高cnt 位写入HT1621，先发送高位
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
//指令模式
static void Sendcmd(u8 command)
{
	LCDCS = 0; 
	//写入标志码"100"
	SendBit_HL(0x80,3); 
	//写入 9 位数据,其中前 8 位为 command 命令,最后 1 位任意
	SendBit_HL(command,9);  
	LCDCS = 1;
}
//data的低cnt位写入1621
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
	//全显示屏幕
    LCDCS = 0;      
	//写入标志码"101"
    SendBit_HL(0xa0,3);  
	//写入 6 位 addr
    SendBit_HL(0,6); 
	//连续写入数据
    for(i =0; i <32; i ++) 
    {
        SendBit_LH(0X00,4);
    }
    LCDCS = 1;
}

//0: 0x5f
//1：0x06
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
/***********************************************变量声明*****************************************************
* 功    能: 两个小的标点标志位  
* 作    者: by lhb_steven
* 日    期: 2016/3/30
************************************************************************************************************/ 
static u8 lcd_point_flag = 1;

void LcdSetNum(u8 data1,u8 data2,u8 data3,u8 data4) {
    Ht1621Tab[3] = lcd_num[data1];
    Ht1621Tab[4] = lcd_num[data2];
    Ht1621Tab[5] = lcd_num[data3];
    Ht1621Tab[6] = lcd_num[data4];
    if(lcd_point_flag == 0) {//两个小点是否打开
        Ht1621Tab[5] &= 0x7f;
    } else {
        Ht1621Tab[5] |= 0x80;
    }
    LCDCS = 0;      
    //写入标志码"101"
    SendBit_HL(0xa0,3);  
    //写入 6 位 addr
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
    //写入标志码"101"
    SendBit_HL(0xa0,3);  
    //写入 6 位 addr
    SendBit_HL(12,6); 
    for (u8 i=0;i<8;i++) {
        SendBit_HL(Ht1621Tab[i],8);
    }
    LCDCS = 1;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void LcdInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: LCD的io初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
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
/**********************************************函数定义***************************************************** 
* 函数名称: void LcdBcakLedSet(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 背光控制  
* 作    者: by lhb_steven
* 日    期: 2016/3/29
************************************************************************************************************/ 
void LcdBcakLedSet(u8 cmd) { 
    LCDLED = cmd;
}
















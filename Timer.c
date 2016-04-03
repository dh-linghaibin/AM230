
#include "Timer.h"
#include "Music.h"

void TimerInit(void) {
    TIM4_IER = 0x00;//       
    TIM4_EGR = 0x01;// 
    TIM4_PSCR = 0x07;// ������ʱ��=��ʱ��/128=16MHZ/128
    TIM4_ARR = 0xFA;// �趨��װ��ʱ�ļĴ���ֵ��255�����ֵ
    //TIM4_CNTR = 0x00;// �趨�������ĳ�ֵ
    // ��ʱ����=(ARR+1)*64=16320uS
    TIM4_IER = 0x01;//   
    TIM4_CR1 = 0x01;  
}

static u8 time_flag = 0;

u8 TimerGetTimeFlag(void) {
    return time_flag;
}

void TimerClearTimeFlag(void) {
    time_flag = 0;
}


#pragma vector=0x19
__interrupt void TIM4_UPD_OVF_IRQHandler(void)
{
    static u16 count_time = 0;
    TIM4_SR = 0x00;
    
    if(count_time < 100) {//400 test for 2016/1/19
        count_time++;
    } else {
        count_time = 0;
        time_flag++;
    }
    if(MusicGetTime() > 0) {
        if(MusicGetTime() == 1) {
            TIM2_CCR1L = 0xff;
        }
        MusicSetTime(MusicGetTime() - 1);
    }
    return;
}


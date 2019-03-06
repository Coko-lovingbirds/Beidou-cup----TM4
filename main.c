

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/fpu.h"
#include "driverlib/qei.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "time.h"
#include "inc/hw_i2c.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "string.h"
#include "driverlib/timer.h"
#include "Sonic.h"
#include "uart/uart1_receive.h"
#include "uart/mpu6050_uart3.h"
#include "uart/uart2_receive.h"
#include "uart/Speak_output.h"


uint32_t speak_count = 0;

bool Speakbusy_flag = false;

/*
uart0  	语音模块  	输出
uart1  	树莓派     	输入输出
uart2  	GPS    	输入
uart3  	mpu		输入
uart4   openmv

Trig 	PF1 	PF2	 	PF3
Echo 	PA7 	PB4 	PD3
*/


void Key_PF4_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

void Busy_PE3_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

void KEY_PF4_Pros(void)
{
    uint8_t ReadPin;
    ReadPin=GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
    if((ReadPin&GPIO_PIN_4)!=GPIO_PIN_4)
    {
        SysCtlDelay(20*(40000000/3000));
        ReadPin=GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
        if((ReadPin&GPIO_PIN_4)!=GPIO_PIN_4)
        {
        	while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4));
            Speak5();
            Shutoff_flag = true;
        }

     }
}

void Spesk_Busy(void)
{
	//PE3
    uint8_t Busy;
    Busy=GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3);
    if(Busy == 0X00)
    {
    	Speakbusy_flag = false;
    }
    else
    {
    	Speakbusy_flag = true;

    }
}



void sonic_judge(void)
{
	if((ui32Distance0 < 300) && (ui32Distance1 >= 300) && (ui32Distance2 >= 300) && (Speakbusy_flag == false))    //左
	{
		Speak2();
	}

	if((ui32Distance0 >= 300) && (ui32Distance1 < 300) && (ui32Distance2 >= 300) && (Speakbusy_flag ==false))   //右
	{
		Speak3();
	}

	if((ui32Distance0 >= 300) && (ui32Distance1 >= 300) && (ui32Distance2 < 300) && (Speakbusy_flag == false))    //前
	{
		Speak1();
	}
	//*************

	if((ui32Distance0 < 300) && (ui32Distance1 < 300) && (ui32Distance2 >= 300) && (Speakbusy_flag == false))
	{
		Speak8();
	}
	if((ui32Distance0 >= 300) && (ui32Distance1 < 300) && (ui32Distance2 < 300) && (Speakbusy_flag == false))
	{
		Speak7();
	}
	if((ui32Distance0 < 300) && (ui32Distance1 >= 300) && (ui32Distance2 < 300) && (Speakbusy_flag == false))
	{
		Speak6();
	}
	if((ui32Distance0 < 300) && (ui32Distance1 < 300) && (ui32Distance2 < 300) && (Speakbusy_flag == false))
	{
		Speak9();
	}


}

void Timer1_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/100);     //10 ms 记一次

	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER1_BASE, TIMER_A);

}

void Timer1IntHandler(void)   //10ms执行一次
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    //countX++;
	speak_count++;
	if(speak_count >= 0xFFFF)
	{
		speak_count = 0;

	}
}


void NVIC_Configure(void)
{
    IntPrioritySet(INT_GPIOA,0x40);
    IntPrioritySet(INT_GPIOB,0x10);
    IntPrioritySet(INT_GPIOD,0x20);
    IntPrioritySet(INT_UART1,0x30);
    IntPrioritySet(INT_UART3,0x50);
    IntPrioritySet(INT_UART2,0x60);
	IntPrioritySet(INT_TIMER1A,0x00);
	IntPrioritySet(INT_UART4,0x70);
}


void Ready_Sonic(void)
{
	 SonicTrig();
	 while((true != bDataReady0)&&(true != bDataReady1)&&(true!=bDataReady2));
	 bDataReady0 = false;
	 bDataReady1 = false;
	 bDataReady2 = false;
//	 UARTprintf("distance0:%4u --distance1:%4u --distance2:%4u \n", ui32Distance0,ui32Distance1,ui32Distance2);

}

void main(void)
{
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL |SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//40Mhz
    FPUEnable();//浮点运算
    FPULazyStackingEnable();
    IntMasterEnable();

    GPIOF_Init();
    Timer0_Init();
    Timer1_Config();
    GPIO_ECHO_Init();
    ConfigureUART0();
    Recive_UART1_Config();
    Recive_UART2_Config();
    Recive_UART3_Config();

    NVIC_Configure();
    Key_PF4_Init();
    SonicTrig();
    Busy_PE3_Init();

    while(1)
    {
    	Spesk_Busy();
    	Ready_Sonic();
    	Printf_MPU_Infos();
        KEY_PF4_Pros();
        sonic_judge();
        u1Data_handle();

        Judgedown_speak();
        Raspi_data_puts();
        SysCtlDelay(100*(SysCtlClockGet()/3000));
    }
}

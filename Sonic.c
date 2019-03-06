/*
 * Sonic.c
 *
 *  Created on: 2018Äê12ÔÂ10ÈÕ
 *      Author: CZW
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "Sonic.h"

uint32_t ui32Distance0 = 0;
volatile bool bDataReady0 = false;
uint32_t ui32Distance1 = 0;
volatile bool bDataReady1 = false;
uint32_t ui32Distance2 = 0;
volatile bool bDataReady2 = false;

void GPIOF_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
//    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
//    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
//    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

}


void IntHandlerGPIOA(void)
{
    uint32_t ui32IntStatus;
    static uint32_t ui32Time[2] = {0};
    static uint8_t ui8EdgeCount = 0;
    double fPeriod = 0;
    ui32IntStatus = GPIOIntStatus(GPIO_PORTA_BASE, true);
    GPIOIntClear(GPIO_PORTA_BASE, ui32IntStatus);
    if((ui32IntStatus & GPIO_PIN_7) == GPIO_PIN_7)
    {
    	ui32Time[ui8EdgeCount++] = TimerValueGet(TIMER0_BASE, TIMER_A);
        GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_FALLING_EDGE);
    	if(ui8EdgeCount > 1) {
    		ui8EdgeCount = 0;
        	fPeriod = ui32Time[1] > ui32Time[0] ? ui32Time[1] - ui32Time[0]
    			           : ui32Time[1] - ui32Time[0] + 0xFFFFFFFF;

    		// ui32Distance = (fPeriod * 25ns * V)/2;
    		//ui32Distance = fPeriod*0.0021875;
    		ui32Distance0 = fPeriod*0.004375;

            // Set the gpio interrupt trigger mode, rising edge.
            GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_RISING_EDGE);

    		bDataReady0 = true;
    	}
    }
}

void IntHandlerGPIOB(void)
{
    uint32_t ui32IntStatus;
    static uint32_t ui32Time[2] = {0};
    static uint8_t ui8EdgeCount = 0;
    double fPeriod = 0;
    ui32IntStatus = GPIOIntStatus(GPIO_PORTB_BASE, true);
    GPIOIntClear(GPIO_PORTB_BASE, ui32IntStatus);
    if((ui32IntStatus & GPIO_PIN_4) == GPIO_PIN_4)
    {
    	ui32Time[ui8EdgeCount++] = TimerValueGet(TIMER0_BASE, TIMER_A);
        GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    	if(ui8EdgeCount > 1) {
    		ui8EdgeCount = 0;
        	fPeriod = ui32Time[1] > ui32Time[0] ? ui32Time[1] - ui32Time[0]
    			           : ui32Time[1] - ui32Time[0] + 0xFFFFFFFF;

    		// ui32Distance = (fPeriod * 25ns * V)/2;
    		//ui32Distance = fPeriod*0.0021875;
    		ui32Distance1 = fPeriod*0.004375;

            // Set the gpio interrupt trigger mode, rising edge.
            GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);

    		bDataReady1 = true;
    	}
    }
}

void IntHandlerGPIOD(void)
{
    uint32_t ui32IntStatus;
    static uint32_t ui32Time[2] = {0};
    static uint8_t ui8EdgeCount = 0;
    double fPeriod = 0;
    ui32IntStatus = GPIOIntStatus(GPIO_PORTD_BASE, true);
    GPIOIntClear(GPIO_PORTD_BASE, ui32IntStatus);
    if((ui32IntStatus & GPIO_PIN_3) == GPIO_PIN_3)
    {
    	ui32Time[ui8EdgeCount++] = TimerValueGet(TIMER0_BASE, TIMER_A);
        GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_FALLING_EDGE);
    	if(ui8EdgeCount > 1) {
    		ui8EdgeCount = 0;
        	fPeriod = ui32Time[1] > ui32Time[0] ? ui32Time[1] - ui32Time[0]
    			           : ui32Time[1] - ui32Time[0] + 0xFFFFFFFF;

    		// ui32Distance = (fPeriod * 25ns * V)/2;
    		//ui32Distance = fPeriod*0.0021875;
    		ui32Distance2 = fPeriod*0.004375;

            // Set the gpio interrupt trigger mode, rising edge.
            GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);

    		bDataReady2 = true;
    	}
    }
}

void GPIO_PA7_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTA_BASE, IntHandlerGPIOA);
    IntEnable(INT_GPIOA);
}

void GPIO_PB4_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTB_BASE, IntHandlerGPIOB);
    IntEnable(INT_GPIOB);
}


void GPIO_PD3_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_3);
    GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTD_BASE, IntHandlerGPIOD);
    IntEnable(INT_GPIOD);
}

void GPIO_ECHO_Init(void)
{
	GPIO_PA7_Init();
	GPIO_PB4_Init();
	GPIO_PD3_Init();
}

void SonicTrig_PF1(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    // Delay some times, 15us.
    SysCtlDelay(15*(SysCtlClockGet()/3000000));
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
}

void SonicTrig_PF2(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    // Delay some times, 15us.
    SysCtlDelay(15*(SysCtlClockGet()/3000000));
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
}

void SonicTrig_PF3(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
    // Delay some times, 15us.
    SysCtlDelay(15*(SysCtlClockGet()/3000000));
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
}
void SonicTrig(void)
{
	SonicTrig_PF1();
    SysCtlDelay(5*(SysCtlClockGet()/3000));
	SonicTrig_PF2();
    SysCtlDelay(5*(SysCtlClockGet()/3000));
	SonicTrig_PF3();
}

void Timer0_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 0xFFFFFFFF);
    // Disable the timer0 interrupt.
    IntDisable(INT_TIMER0A);
	// Enable the timer.
	TimerEnable(TIMER0_BASE, TIMER_A);
    IntMasterEnable();
}




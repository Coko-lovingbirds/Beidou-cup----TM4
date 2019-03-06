/*
 * uart2_receive.c
 *
 *  Created on: 2018年12月10日
 *      Author: CZW
 */
#include <stdint.h>
#include <stdbool.h>
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
#include "string.h"
#include "uart2_receive.h"
#include "Speak_output.h"

uint8_t gps_data[5];
bool UART2_Updated_Flag = false;
double longitude = 5.6252/*经度*/,latitude=9.6363/*纬度*/;


char UART2_Rx_Buffers[50];

void Recive_UART2_Config(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;

    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    UARTConfigSetExpClk(UART2_BASE,SysCtlClockGet(), 57600,
                               (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                                UART_CONFIG_PAR_NONE));
    IntEnable(INT_UART2);
    UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);


}



void UART2IntHandler(void)
{
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART2_BASE, true);
	UARTIntClear(UART2_BASE, ui32Status);

}





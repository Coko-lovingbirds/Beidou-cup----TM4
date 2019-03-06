/*
 * uart2_receive.c
 *
 *  Created on: 2018年10月20日
 *      Author: wmy
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
#include "uart1_receive.h"
#include "mpu6050_uart3.h"
#include "uart2_receive.h"
#include "Speak_output.h"


int longitude_decimal,latitude_ingeter,longitude_ingeter,latitude_decimal;


char UART1_Rx_Buffers[50];
char UART1_Rx_Len;
char UART1_Sender_Address;
char UART1_Rx_Data[50];
bool UART1_Updated_Flag=false;
bool Shutoff_flag = false;
bool Downobject_flag = false;
uint32_t Face_count = 0;




void Recive_UART1_Config(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0| GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE,SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
}



//55 53 22 51  地面有障碍物
//55 53 88 51  地面没有障碍物
//55 54 02 51  刘琪才
//55 54 01 51  陈智威

void UART1IntHandler(void)
{
	uint32_t ui32Status;
	char rx_buffer;
	static bool receive_flag=false;
	int count=0;
	ui32Status = UARTIntStatus(UART1_BASE, true);
	UARTIntClear(UART1_BASE, ui32Status);
	while(UARTCharsAvail(UART1_BASE))
	{
		rx_buffer = (uint8_t)(UARTCharGetNonBlocking(UART1_BASE));
		if(rx_buffer==0x55)
		{
			receive_flag = true;
            memset(UART1_Rx_Buffers,0,50);
            count =0;
		}
		if(rx_buffer==0x51)
		{
			receive_flag = false;
			UART1_Updated_Flag =true;
		}

		if(receive_flag&&rx_buffer!=0x55)
		{
//			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
			UART1_Rx_Buffers[count] = rx_buffer;
			count++;

		}


	}
}


void u1Data_handle(void)
{
	if(UART1_Updated_Flag==true)
	{
		UART1_Updated_Flag = false;
		switch(UART1_Rx_Buffers[0])
		{
			case 0x53:         //ground
				switch(UART1_Rx_Buffers[1])
				{
				case 0x22:
					Downobject_flag = true;
					break;
				case 0x88:
					Downobject_flag = false;
					break;
				}
				break;

			case 0x54: 			//face
				switch(UART1_Rx_Buffers[1])
				{
				case 0x02:
					Face_count = 2;   //lqc
					break;
				case 0x01:
					Face_count = 1;   //czw
					break;
				}
				break;
		}
	}
    memset(UART1_Rx_Buffers,0,50);
}

void Judgedown_speak(void)
{
	if((Downobject_flag)&&(Speakbusy_flag == false))
	{
		Speak4();
		Downobject_flag =false;
	}

	if((Face_count == 2)&&(Speakbusy_flag == false))
	{
		Speak_lqc();
		Face_count = 0;
	}

	if((Face_count == 1)&&(Speakbusy_flag == false))
	{
		Speak_czw();
		Face_count = 0;
	}

}

void Raspi_data_puts(void)
{
//		//纬度
//	latitude_ingeter = (int)latitude;   //整数
//	latitude_decimal = (int)((latitude-latitude_ingeter)*10000);  //小数
//	//经度
//	longitude_ingeter = (int)longitude;   //整数
//	longitude_decimal = (int)((longitude-longitude_ingeter)*10000);  //小数

//	int n0,n1,res0,res1,n2,n3,res2,res3;


	if(Shutoff_flag)     //关机
	{
		Shutoff_flag = false;
		UARTCharPutNonBlocking(UART1_BASE,0xea);
		UARTCharPutNonBlocking(UART1_BASE,0xAD);
		UARTCharPutNonBlocking(UART1_BASE,'\n');
	}
	if(falldown_flag)    //摔倒
	{
		UARTCharPutNonBlocking(UART1_BASE,0xea);
		UARTCharPutNonBlocking(UART1_BASE,0xBB);
		UARTCharPutNonBlocking(UART1_BASE,'\n');
	}
	if(falldown_flag == false)    //不摔倒
	{
		UARTCharPutNonBlocking(UART1_BASE,0xea);
		UARTCharPutNonBlocking(UART1_BASE,0xAA);
		UARTCharPutNonBlocking(UART1_BASE,'\n');
	}


//	UARTCharPutNonBlocking(UART1_BASE,0xea);
//	res0 = longitude_ingeter % 255;    //经度整数
//	n0 = longitude_ingeter / 255;
//	UARTCharPutNonBlocking(UART1_BASE,0xfa);
//	UARTCharPutNonBlocking(UART1_BASE,n0);
//	UARTCharPutNonBlocking(UART1_BASE,res0);
//	UARTCharPutNonBlocking(UART1_BASE,'\n');
//	res1 = longitude_decimal % 255;     //经度小数
//	n1 = longitude_decimal / 255;
//	UARTCharPutNonBlocking(UART1_BASE,0xea);
//	UARTCharPutNonBlocking(UART1_BASE,0xfb);
//	UARTCharPutNonBlocking(UART1_BASE,n1);
//	UARTCharPutNonBlocking(UART1_BASE,res1);
//
//	res2 = latitude_ingeter % 255;    //纬度整数
//	n2 = latitude_ingeter / 255;
//	UARTCharPutNonBlocking(UART1_BASE,0xca);
//	UARTCharPutNonBlocking(UART1_BASE,n2);
//	UARTCharPutNonBlocking(UART1_BASE,res2);
//	UARTCharPutNonBlocking(UART1_BASE,'\n');
//	res3 = latitude_decimal % 255;     //纬度小数
//	n3 = latitude_decimal / 255;
//	UARTCharPutNonBlocking(UART1_BASE,0xda);
//	UARTCharPutNonBlocking(UART1_BASE,0xfb);
//	UARTCharPutNonBlocking(UART1_BASE,n3);
//	UARTCharPutNonBlocking(UART1_BASE,res3);
//
//	UARTCharPutNonBlocking(UART1_BASE,'\n');
//


}




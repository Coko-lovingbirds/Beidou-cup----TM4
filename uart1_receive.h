/*
 * uart1_receive.h
 *
 *  Created on: 2018Äê10ÔÂ20ÈÕ
 *      Author: wmy
 */

#ifndef UART_UART1_RECEIVE_H_
#define UART_UART1_RECEIVE_H_

extern char UART1_Rx_Buffers[50];
extern char UART1_Rx_Len;
extern char UART1_Sender_Address;
extern char UART1_Rx_Data[50];
extern bool UART1_Updated_Flag;
extern bool Shutoff_flag;
extern int longitude_decimal,latitude_ingeter,longitude_ingeter,latitude_decimal;
extern bool Downobject_flag;
extern uint32_t Face_count;
extern bool Speakbusy_flag;

extern void u1Data_handle(void);
extern void Judgedown_speak(void);
extern void Recive_UART1_Config(void);
extern void UART1IntHandler(void);
extern void Raspi_data_puts(void);

#endif /* UART_UART1_RECEIVE_H_ */

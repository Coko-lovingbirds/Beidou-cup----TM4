/*
 * uart2_receive.h
 *
 *  Created on: 2018年12月10日
 *      Author: CZW
 */

#ifndef UART2_RECEIVE_H_
#define UART2_RECEIVE_H_

extern uint8_t gps_data[5];
extern bool UART2_Updated_Flag;
extern double longitude/*经度*/,latitude/*纬度*/;

extern void Recive_UART2_Config(void);
extern void UART2IntHandler(void);


#endif /* UART2_RECEIVE_H_ */

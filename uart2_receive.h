/*
 * uart2_receive.h
 *
 *  Created on: 2018��12��10��
 *      Author: CZW
 */

#ifndef UART2_RECEIVE_H_
#define UART2_RECEIVE_H_

extern uint8_t gps_data[5];
extern bool UART2_Updated_Flag;
extern double longitude/*����*/,latitude/*γ��*/;

extern void Recive_UART2_Config(void);
extern void UART2IntHandler(void);


#endif /* UART2_RECEIVE_H_ */

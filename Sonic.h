/*
 * Sonic.h
 *
 *  Created on: 2018Äê12ÔÂ10ÈÕ
 *      Author: CZW
 */

#ifndef USING_GPS_PROJECT_SONIC_H_
#define USING_GPS_PROJECT_SONIC_H_

extern uint32_t ui32Distance0;
extern volatile bool bDataReady0;
extern uint32_t ui32Distance1;
extern volatile bool bDataReady1;
extern uint32_t ui32Distance2;
extern volatile bool bDataReady2;

extern void GPIOF_Init(void);
extern void GPIO_PA7_Init(void);
extern void GPIO_PB4_Init(void);
extern void GPIO_PB4_Init(void);
extern void GPIO_ECHO_Init(void);
extern void SonicTrig_PF1(void);
extern void SonicTrig_PF2(void);
extern void SonicTrig_PF3(void);
extern void SonicTrig(void);
extern void Timer0_Init(void);


#endif /* USING_GPS_PROJECT_SONIC_H_ */

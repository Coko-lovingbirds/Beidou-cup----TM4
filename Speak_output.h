/*
 * Speak_output.H
 *
 *  Created on: 2018年12月14日
 *      Author: CZW
 */

#ifndef SPEAK_OUTPUT_H_
#define SPEAK_OUTPUT_H_

extern void ConfigureUART0(void);//串口
extern void Speak1(void); //左边有障碍物
extern void Speak2(void); //前方有障碍物
extern void Speak3(void);  //右边有障碍物
extern void Speak4(void);  //下方有障碍物
extern void Speak5(void);  //即将关机
extern void Speak6(void);  //前方左方有障碍物
extern void Speak7(void);  //左方右方有障碍物
extern void Speak8(void);  //前方右方有障碍物
extern void Speak9(void);  //周围有障碍物
extern void Speak_czw(void);
extern void Speak_lqc(void);


#endif /* SPEAK_OUTPUT_H_ */

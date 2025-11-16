#ifndef __MOTOR_H
#define __MOTOR_H
#include <stdint.h>

extern uint16_t Motor_Speed;

void Motor_Init(void);
void Motor_SetSpeed(uint16_t Motor_Speed);
void Motor_Off(void);

void Motor_On(void);
#endif

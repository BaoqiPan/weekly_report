#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>
#include "status.h"
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendString(char* String);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

extern uint16_t Main_Status;
extern uint16_t Memo_Status;


#endif



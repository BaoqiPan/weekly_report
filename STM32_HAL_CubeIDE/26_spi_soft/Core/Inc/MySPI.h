/*
 * MySPI.h
 *
 *  Created on: Apr 9, 2026
 *      Author: 33062
 */

#ifndef INC_MYSPI_H_
#define INC_MYSPI_H_

#include <stdint.h>

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif /* INC_MYSPI_H_ */

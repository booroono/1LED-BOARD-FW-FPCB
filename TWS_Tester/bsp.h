/*
 * bsp.h
 *
 * Created: 2025-04-09 오전 12:32:26
 *  Author: bruno
 */ 


#ifndef BSP_H_
#define BSP_H_

#include "main.h"

typedef struct {
	int8_t cc1_8vOffset, cc3_3vOffset;
	uint16_t ref1_8v, ref3_3v;
}cal_data_t;

extern uint8_t hbCount;
extern uint8_t spiTxBuffer[40];
extern uint8_t spiRxBuffer[40];
extern cal_data_t cal;
extern struct io_descriptor *SPI_io;
extern struct io_descriptor *I2C_io;

void COMM_Reset_Task_Start(void);
void COMM_Reset_Task_Stop(void);
void BSP_Initial(void);
void Auto_Calibration(void);

#endif /* BSP_H_ */
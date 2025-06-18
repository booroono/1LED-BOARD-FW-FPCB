/*
 * usart.h
 *
 * Created: 2025-04-09 오전 12:33:10
 *  Author: bruno
 */ 


#ifndef USART_H_
#define USART_H_

#include "main.h"

typedef enum {
	CMD_NONE = 0,
	VER_CHECK	= 0x70, SIDE_SET, BUZ_OFF, HEART_BEAT,				// etc
	CON_CTRL = 0x80, REF_V_CTRL1, REF_V_CTRL2, ETC_CTRL, AD_READ, SOL_CTRL,	MN_ALL_OFF,		// manual
	TEST_CFG = 0x90, JIG_DOWN, TEST_START, TEST_STOP, TEST_RESULT, TEST_RESET				// test
}cmd_list_e;

typedef struct {
	uint8_t sendAddr, headerCnt, rxCrc, txCrc, rxMode;
	uint16_t length, dataCnt;
	uint8_t header[5];
	uint8_t data[USART_MAX_DATA];
	cmd_list_e cmd;
}usart_data_t;

extern usart_data_t usartData;
void Send_Header(uint8_t cmd, uint16_t length);
void Send_Data(uint8_t cmd, uint16_t length);
void Buffer_Clear(void);

#endif /* USART_H_ */
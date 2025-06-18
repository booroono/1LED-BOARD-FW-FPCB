/*
 * usart.c
 *
 * Created: 2025-04-09 오전 12:28:29
 *  Author: bruno
 */ 

#include "usart.h"

usart_data_t usartData = {0,};

void SERCOM4_2_Handler() {								// usart data receive
	usart_data_t *usart = &usartData;
	
	if(SERCOM4->USART.INTFLAG.bit.RXC)	{
		if(usart->rxMode == USART_MODE_HEADER) {
			usart->header[usart->headerCnt] = USART_read_byte();
			usart->rxCrc ^= usart->header[usart->headerCnt];

			switch (usart->headerCnt) {
				case 0 :
					COMM_Reset_Task_Start();
					if(usart->header[usart->headerCnt] != USART_SOT) {Buffer_Clear();}
					else {usart->headerCnt++;}
					break;
				case 1 :
					if(usart->header[usart->headerCnt] != USART_UI_ADDR) {Buffer_Clear();}
					else {usart->headerCnt++;}
					break;
				case 2 :
				case 3 :
					usart->headerCnt++;
					break;
				case 4 :
					usart->length = ((usart->header[3] << 8) | (usart->header[4] & 0xFF)) + 4;
					usart->rxMode = USART_MODE_DATA;
					usart->headerCnt = 0;
					usart->dataCnt = 0;
					break;
				default :
					Buffer_Clear();
					break;
			}
		}
		else if(usart->rxMode == USART_MODE_DATA) {
			if(usart->dataCnt >= usart->length) {
				Buffer_Clear();
				return;
			}
			usart->data[usart->dataCnt] = USART_read_byte();
			if(usart->length > 4) {
				if(usart->dataCnt < (usart->length - 4)) {
					usart->rxCrc ^= usart->data[usart->dataCnt];
				}
			}
			usart->dataCnt++;
			if(usart->length == usart->dataCnt) {
				if(usart->data[usart->dataCnt - 1] == USART_EOT) {
					if(usart->data[usart->dataCnt - 2] == USART_EOT) {
						if(usart->data[usart->dataCnt - 3] == USART_EOT) {
							if(usart->data[usart->dataCnt - 4] == usart->rxCrc) {
								usart->cmd = usart->header[2];
								Buffer_Clear();
								usart->length -= 4;
							} else {Buffer_Clear();}
						} else {Buffer_Clear();}
					}	else {Buffer_Clear();}
				} else {Buffer_Clear();}
			}
		}	else {Buffer_Clear();}
	}
}

void Send_Header(uint8_t cmd, uint16_t length) {						// usart header trans
	uint8_t header[5] = {USART_SOT, USART_EQ_ADDR, 0, 0, 0};
	usart_data_t *usart = &usartData;
	
	hri_sercomusart_clear_CTRLB_RXEN_bit(SERCOM4);

	usart->txCrc = 0;
	header[2] = cmd;
	header[3] = length >> 8;
	header[4] = length;
	for(uint8_t i = 0; i < 5; i++) {
		USART_write_byte(header[i]);
		usart->txCrc ^= header[i];
		while (!USART_is_byte_sent());
	}
}

void Send_Data(uint8_t cmd, uint16_t length) {							// usart data trans
	usart_data_t *usart = &usartData;

	Send_Header(cmd, length);
	for(uint16_t i = 0; i < length; i++) {
		USART_write_byte(usart->data[i]);
		usart->txCrc ^= usart->data[i];
		while (!USART_is_byte_sent());
	}
	USART_write_byte(usart->txCrc);
	while (!USART_is_byte_sent());
	USART_write_byte(USART_EOT);
	while (!USART_is_byte_sent());
	USART_write_byte(USART_EOT);
	while (!USART_is_byte_sent());
	USART_write_byte(USART_EOT);
	while (!USART_is_byte_sent());
	while (!SERCOM4->USART.INTFLAG.bit.TXC);
	
	hri_sercomusart_set_CTRLB_RXEN_bit(SERCOM4);
}

 void Buffer_Clear(void) {										// usart data clear
	usart_data_t *usart = &usartData;

	COMM_Reset_Task_Stop();
	usart->rxCrc = 0;
	usart->txCrc = 0;
	usart->headerCnt = 0;
	usart->dataCnt = 0;
	usart->rxMode = USART_MODE_HEADER;
}
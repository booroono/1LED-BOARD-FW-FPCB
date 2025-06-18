/*
 * conversion.c
 *
 * Created: 2023-01-25 오후 08:31:07
 *  Author: bruno
 */

#include "conversion.h"

cfg_data_t cfgData = {0,};

void CFG_Conversion(void) {
	uint8_t stepNo = 0, buffer, result = true;
	uint16_t cfgIndex = 0;
	int16_t cfgLength;
	cfg_data_t *cfg = &cfgData;
	usart_data_t *usart = &usartData;

	cfgLength = usart->length;
	stepNo = usart->data[0];
	cfgIndex++;
	switch (stepNo) {
		case STEP_CON_OS :
			cfg->stepLength = usart->data[cfgIndex++]; 
			cfg->stepLength |= usart->data[cfgIndex++];
			buffer = cfg->stepLength / 3;
			for(uint8_t i = 0; i < buffer; i++) {
				cfg->conn.positve[i] = usart->data[cfgIndex++];
				cfg->conn.negative[i] = usart->data[cfgIndex++];
				cfg->conn.refernce[i] = usart->data[cfgIndex++] - 1;
			}
			break;
		case STEP_POGO_OS :
			cfgIndex += 2;
			cfg->pogo.j1 = usart->data[cfgIndex++];
			cfg->pogo.j2 = usart->data[cfgIndex++];
			cfg->pogo.pogo = usart->data[cfgIndex++];
			cfg->pogo.gnd = usart->data[cfgIndex++];	
			break;
		case STEP_LED :
			cfgIndex += 2;
			cfg->led.led = usart->data[cfgIndex++];
			cfg->led.gnd = usart->data[cfgIndex++];
			break;
		case STEP_HALL_SENSOR :
			cfgIndex += 2;
			cfg->hall.v1_8 = usart->data[cfgIndex++];
			cfg->hall.scl = usart->data[cfgIndex++];
			cfg->hall.sda = usart->data[cfgIndex++];
			cfg->hall.gnd = usart->data[cfgIndex++];	
			break;
		case STEP_VBAT_ID :
			cfgIndex += 2;
			cfg->vbatId.v1_8 = usart->data[cfgIndex++];
			cfg->vbatId.batId = usart->data[cfgIndex++];
			cfg->vbatId.gnd = usart->data[cfgIndex++];
			break;
		case STEP_BATTERY :
			cfgIndex += 2;
			cfg->batt.vbatt = usart->data[cfgIndex++];
			cfg->batt.gnd = usart->data[cfgIndex++];
			break;
		case STEP_PROX :
			cfgIndex += 2;
			cfg->prox.v3_3 = usart->data[cfgIndex++];
			cfg->prox.v1_8 = usart->data[cfgIndex++];
			cfg->prox.scl = usart->data[cfgIndex++];
			cfg->prox.sda = usart->data[cfgIndex++];
			cfg->prox.gnd = usart->data[cfgIndex++];	
			break;
		case STEP_MIC :
			cfgIndex += 2;
			cfg->mic.mic = usart->data[cfgIndex++];
			cfg->mic.gnd = usart->data[cfgIndex++];
			break;
	}
	cfgLength -= cfgIndex;
	if((cfgLength < 0) || (stepNo > MAX_STEP)) {result = false;}

	usart->data[0] = result;
	Send_Data(TEST_CFG, 1);
}
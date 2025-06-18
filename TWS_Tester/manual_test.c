/*
 * ext_control.c
 *
 * Created: 2025-04-09 오전 12:36:15
 *  Author: bruno
 */

#include "manual_test.h"

void Manual_Test(void) {
	uint8_t i = 0;
	uint16_t resultLength = 0;
	uint8_t buffer[5];
	usart_data_t *usart = &usartData;
	out_state_t *out = &outState;

	switch (usart->cmd) {
		case VER_CHECK :
			resultLength = 2;
			usart->data[0] = FW_VER >> 8;
			usart->data[1] = FW_VER & 0xFF;
			break;
		case SIDE_SET :
			eqSide = usart->data[0];
			break;
		case BUZ_OFF :
			gpio_set_pin_level(GPIO_BUZ, false);
			break;
		case CON_CTRL :
			for(i = 0; i < 5; i++) {
				Out_Control(i, usart->data[i]);
			}
			break;
		case REF_V_CTRL1 :
		case REF_V_CTRL2 :
		case ETC_CTRL:
			MUX_Control((usart->cmd - REF_V_CTRL1), usart->data[0]);
			break;
		case AD_READ :
			resultLength = ADC_Read(usart->data[0], out->ref1, buffer);
			if (usart->data[0] == SEL_AD_MUX8) {
				if (resultLength == 2) {
					memcpy(&usart->data[3], buffer, resultLength);
					resultLength += 3;
					usart->data[0] = AD_RESULT_V;
					usart->data[1] = 0;
					usart->data[2] = 0;
				} else {
					memcpy(&usart->data[1], buffer, resultLength);
					resultLength += 1;
					usart->data[0] = AD_RESULT_R;
				}
			}
			else {memcpy(&usart->data[0], buffer, resultLength);}
			break;
		case SOL_CTRL :
			Solenoid_Valve_Control(usart->data[0], usart->data[1]);
			break;
		case MN_ALL_OFF :
			Board_Reset();
			break;
	}

 	if (resultLength) {
		Send_Data(usart->cmd, resultLength);
	}
}

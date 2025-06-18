/*
 * ext_control.c
 *
 * Created: 2025-04-09 오전 12:36:15
 *  Author: bruno
 */ 

#include "ext_control.h"

uint8_t vaccumFlag = 0;
out_state_t outState = {0,};
select_logic_t selectLogic = {0,};

void Latch_Control(uint32_t enPin, uint8_t data) {			// latch control
	if(data == 0x00) {
		gpio_set_port_level(GPIO_PORTB, 0x3F, false);
	}
	else {
		data -= 1;
		gpio_set_port_level(GPIO_PORTB, 0x3F, false);
		delay_ms(1);
		gpio_set_port_level(GPIO_PORTB, data, true);
	}
	gpio_set_pin_level(enPin, true);
	delay_us(10);
	gpio_set_pin_level(enPin, false);
}

void Out_Control(uint8_t selectLatch, uint8_t ctrlOut) {					// out control
	uint32_t ctrlGpio = 0;

	switch (selectLatch) {
		case SEL_OUT_POSITIVE :
			ctrlGpio = GPIO_LAT_RES_P_EN;
			outState.conn[SEL_OUT_POSITIVE] = ctrlOut;
			gpio_set_pin_level(GPIO_MUX_RES_P_ENA_AL, true);
			delay_ms(1);
			Latch_Control(ctrlGpio, ctrlOut);
			delay_ms(5);
			if(ctrlOut) {gpio_set_pin_level(GPIO_MUX_RES_P_ENA_AL, false);}
			break;
		case SEL_OUT_NEGATIVE :
			ctrlGpio = GPIO_LAT_RES_N_EN;
			outState.conn[SEL_OUT_NEGATIVE] = ctrlOut;
			gpio_set_pin_level(GPIO_MUX_RES_N_ENA_AL, true);
			delay_ms(1);
			Latch_Control(ctrlGpio, ctrlOut);
			delay_ms(5);
			if(ctrlOut) {gpio_set_pin_level(GPIO_MUX_RES_N_ENA_AL, false);}
			break;
		case SEL_OUT_VOUT :
			ctrlGpio = GPIO_LAT_VOUT_EN;
			outState.conn[SEL_OUT_VOUT] = ctrlOut;
			gpio_set_pin_level(GPIO_MUX_VOUT_ENA_AL, true);
			delay_ms(1);
			Latch_Control(ctrlGpio, ctrlOut);
			delay_ms(5);
			if(ctrlOut) {gpio_set_pin_level(GPIO_MUX_VOUT_ENA_AL, false);}
			break;
		case SEL_OUT_ETC_X :
			ctrlGpio = GPIO_LAT_ETC_X_EN;
			outState.conn[SEL_OUT_ETC_X] = ctrlOut;
			gpio_set_pin_level(GPIO_MUX_ETC_X_ENA_AL, true);
			delay_ms(1);
			Latch_Control(ctrlGpio, ctrlOut);
			delay_ms(5);
			if(ctrlOut) {gpio_set_pin_level(GPIO_MUX_ETC_X_ENA_AL, false);}
			break;
		case SEL_OUT_ETC_Y :
			ctrlGpio = GPIO_LAT_ETC_Y_EN;
			outState.conn[SEL_OUT_ETC_Y] = ctrlOut;
			gpio_set_pin_level(GPIO_MUX_ETC_Y_ENA_AL, true);
			delay_ms(1);
			Latch_Control(ctrlGpio, ctrlOut);
			delay_ms(5);
			if(ctrlOut) {gpio_set_pin_level(GPIO_MUX_ETC_Y_ENA_AL, false);}
			break;
		default : 
			break;
	}
	delay_ms(20);
}  


void Out_All_Off(void) {
	for(uint8_t i = 0; i < 5; i++) {
		Out_Control(i, OUT_OFF);
	}
}

void MUX_Control(uint8_t selectMux, uint8_t ctrlMux) {			// mux control
	switch (selectMux)
	{
		case SEL_MUX8 :
			if(ctrlMux == MUX_OFF) {
				gpio_set_port_level(GPIO_PORTA, 0xF, false);
				outState.ref1 = 0xFF;
			}
			else if(outState.ref1 != ctrlMux) {
				gpio_set_port_level(GPIO_PORTA, 0xF, false);
				delay_ms(5);
				gpio_set_port_level(GPIO_PORTA, (0x8 | ctrlMux), true);
				outState.ref1 = ctrlMux;
				delay_ms(20);
			}
			break;
		case SEL_MUX4 :
			if(ctrlMux == MUX_OFF) {
				gpio_set_port_level(GPIO_PORTA, 0x70, false);
				outState.ref2 = 0xFF;
			}
			else if(outState.ref2 != ctrlMux) {
				gpio_set_port_level(GPIO_PORTA, 0x70, false);
				delay_ms(5);
				gpio_set_port_level(GPIO_PORTA, ((0x4 | ctrlMux) << 4), true);
				outState.ref2 = ctrlMux;
				delay_ms(20);
			}
			break;
		case SEL_MUX2_4 :
			if(ctrlMux == MUX_OFF) {
				gpio_set_port_level(GPIO_PORTB, 0x1C0, false);
				outState.etc = 0xFF;
			}
			else if(outState.etc != ctrlMux) {
				gpio_set_port_level(GPIO_PORTB, 0x1C0, false);
				delay_ms(5);
				gpio_set_port_level(GPIO_PORTB, ((0x4 | ctrlMux) << 6), true);
				outState.etc = ctrlMux;
				delay_ms(20);
			}
			break;
		case SEL_MUX_AD :
			gpio_set_port_level(GPIO_PORTA, 0x700, false);
			delay_ms(5);
			gpio_set_port_level(GPIO_PORTA, (ctrlMux << 8), true);
			delay_ms(20);
			break;
		default : 
			break;
	}
}

void MUX_All_Off(void) {
	for(uint8_t i = 0; i < 3; i++) {
		MUX_Control(i, MUX_OFF);
	}
}

uint8_t Input_Check(const uint8_t pin) {					// Switch Filter
	uint8_t i = 0, j = 0, k = 0;
	uint8_t status = 0;
	
	for(i = 0; i < 11; i++) {
		status = gpio_get_pin_level(pin);				// Low Input
		delay_ms(4);
		if(!status) {j++;}
		else {k++;}
	}
	if(j > k) {return true;}
	
	return false;
}

void Solenoid_Valve_Control(uint8_t control, uint8_t solNumber) {			// sol valve control
  out_state_t *out = &outState;
	
  if(control == SOL_ALL_OFF) {
	gpio_set_pin_level(SOL_PROX + 0x20, false); delay_ms(200);
	gpio_set_pin_level(SOL_POGO_LED_Y + 0x20, false); delay_ms(5);
	gpio_set_pin_level(SOL_LED_X + 0x20, false); delay_ms(5);
	gpio_set_pin_level(SOL_DOWN + 0x20, false); delay_ms(5);
	gpio_set_pin_level(SOL_FPCB + 0x20, false); delay_ms(5);
	gpio_set_pin_level(SOL_VACCUM + 0x20, false); delay_ms(5);
	out->sol = 0;
  } 
  else if(control == SOL_ON) {
	PORT->Group[1].OUT.reg |= (1 << solNumber);
	out->sol |= (1 << (solNumber - 10));
	delay_ms(5);
  }
  else if(control == SOL_OFF) {
	PORT->Group[1].OUT.reg &= ~(1 << solNumber);
	out->sol &= ~(1 << (solNumber - 10));
	delay_ms(5);
  }
  delay_ms(15);
}

void Board_Reset(void) {
	Out_All_Off();
	MUX_All_Off();
	Solenoid_Valve_Control(SOL_ALL_OFF, 0);
	gpio_set_pin_level(GPIO_BUZ, false);
	gpio_set_pin_level(GPIO_LED_OK, true);
	gpio_set_pin_level(GPIO_LED_NG, false);
	//gpio_set_pin_level(GPIO_LED_READY, true);
}
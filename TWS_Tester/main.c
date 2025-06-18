#include "main.h"

int main(void)
{	
  usart_data_t *usart = &usartData;
  /* Initializes MCU, drivers and middleware */
  atmel_start_init();
  BSP_Initial();
  ADS8681_Initial();
  delay_ms(10);
  Auto_Calibration();
  Board_Reset();
  NVIC_EnableIRQ(SERCOM4_2_IRQn);
	
  /* Replace with your application code */
  while (1) {
	if(Input_Check(GPIO_IN_SW3)) {						// reset check
		Send_Data(TEST_RESET, 0);
	  delay_ms(100);
	  Board_Reset();
	}
	if(Input_Check(GPIO_IN_SW2)) {Solenoid_Valve_Control(SOL_ON, SOL_VACCUM);}		// vaccum check
	else {Solenoid_Valve_Control(SOL_OFF, SOL_VACCUM);}
	if(Input_Check(GPIO_IN_SW1) && Input_Check(GPIO_IN_SW4)) {					// start check		
    Send_Data(JIG_DOWN, 0);
		gpio_set_pin_level(GPIO_LED_OK, false);
		gpio_set_pin_level(GPIO_LED_NG, false);
  }  
	if(usart->cmd) {
	  switch (usart->cmd)	{
			case TEST_CFG :
				CFG_Conversion();
				break;
			case TEST_START :
				Test_Mode();
				break;
			case TEST_STOP :
				if(usart->data[0] == 0x01) {										// test end
					testStop = true;
          Board_Reset();
				}	
				else if(usart->data[0] == 0x03) {										// test fail
					gpio_set_pin_level(GPIO_LED_NG, true);
					gpio_set_pin_level(GPIO_BUZ, true);
				}
				break;
			default :
				Manual_Test();
				break;
			}
			usart->cmd = 0;
		}
  }
}

/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

#include <hpl_rtc_base.h>

struct timer_descriptor      TIMER_RTC;
struct spi_m_sync_descriptor SPI;
struct timer_descriptor      TIMER_3;

struct i2c_m_sync_desc I2C;

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_RTC_init(void)
{
	hri_mclk_set_APBAMASK_RTC_bit(MCLK);
	timer_init(&TIMER_RTC, RTC, _rtc_get_timer());
}

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void USART_CLOCK_init()
{

	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_SLOW, CONF_GCLK_SERCOM4_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM4_bit(MCLK);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void USART_PORT_init()
{

	gpio_set_pin_function(PA13, PINMUX_PA13D_SERCOM4_PAD0);

	gpio_set_pin_function(PA12, PINMUX_PA12D_SERCOM4_PAD1);
}

void I2C_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA23,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA23, PINMUX_PA23D_SERCOM5_PAD0);

	gpio_set_pin_pull_mode(PA22,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA22, PINMUX_PA22D_SERCOM5_PAD1);
}

void I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_SLOW, CONF_GCLK_SERCOM5_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM5_bit(MCLK);
}

void I2C_init(void)
{
	I2C_CLOCK_init();
	i2c_m_sync_init(&I2C, SERCOM5);
	I2C_PORT_init();
}

void SPI_PORT_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(PB21, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PB21,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PB21, PINMUX_PB21D_SERCOM7_PAD0);

	gpio_set_pin_level(PB20,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB20, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB20, PINMUX_PB20D_SERCOM7_PAD1);

	gpio_set_pin_level(PB19,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB19, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB19, PINMUX_PB19D_SERCOM7_PAD3);
}

void SPI_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_CORE, CONF_GCLK_SERCOM7_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_SLOW, CONF_GCLK_SERCOM7_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM7_bit(MCLK);
}

void SPI_init(void)
{
	SPI_CLOCK_init();
	spi_m_sync_init(&SPI, SERCOM7);
	SPI_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_3_init(void)
{
	hri_mclk_set_APBBMASK_TC3_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC3_GCLK_ID, CONF_GCLK_TC3_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	timer_init(&TIMER_3, TC3, _tc_get_timer());
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA00

	gpio_set_pin_level(GPIO_MUX8_A,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX8_A, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX8_A, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA01

	gpio_set_pin_level(GPIO_MUX8_B,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX8_B, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX8_B, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA02

	gpio_set_pin_level(GPIO_MUX8_C,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX8_C, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX8_C, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA03

	gpio_set_pin_level(GPIO_MUX8_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX8_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX8_EN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA04

	gpio_set_pin_level(GPIO_MUX4_A,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX4_A, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX4_A, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA05

	gpio_set_pin_level(GPIO_MUX4_B,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX4_B, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX4_B, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA06

	gpio_set_pin_level(GPIO_MUX4_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX4_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX4_EN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA08

	gpio_set_pin_level(GPIO_AD_A,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_AD_A, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_AD_A, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA09

	gpio_set_pin_level(GPIO_AD_B,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_AD_B, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_AD_B, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA10

	gpio_set_pin_level(GPIO_AD_C,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_AD_C, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_AD_C, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA15

	gpio_set_pin_level(GPIO_BUZ,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_BUZ, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_BUZ, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA16

	gpio_set_pin_level(GPIO_LAT_RES_P_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_RES_P_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_RES_P_EN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA17

	gpio_set_pin_level(GPIO_MUX_RES_P_ENA_AL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX_RES_P_ENA_AL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX_RES_P_ENA_AL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA18

	gpio_set_pin_level(GPIO_LAT_RES_N_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_RES_N_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_RES_N_EN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA19

	gpio_set_pin_level(GPIO_MUX_RES_N_ENA_AL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX_RES_N_ENA_AL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX_RES_N_ENA_AL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB00

	gpio_set_pin_level(GPIO_LAT_D0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_D0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_D0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB01

	gpio_set_pin_level(GPIO_LAT_D1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_D1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_D1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB02

	gpio_set_pin_level(GPIO_LAT_D2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_D2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_D2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB03

	gpio_set_pin_level(GPIO_LAT_D3,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_D3, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_D3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB04

	gpio_set_pin_level(GPIO_LAT_D4,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_D4, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_D4, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB05

	gpio_set_pin_level(GPIO_LAT_D5,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_D5, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_D5, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB06

	gpio_set_pin_level(GPIO_MUX2_4_A,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX2_4_A, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX2_4_A, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB07

	gpio_set_pin_level(GPIO_MUX2_4_B,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX2_4_B, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX2_4_B, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB08

	gpio_set_pin_level(GPIO_MUX2_4_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX2_4_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX2_4_EN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB10

	gpio_set_pin_level(GPIO_SOL1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_SOL1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_SOL1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB11

	gpio_set_pin_level(GPIO_SOL2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_SOL2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_SOL2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB12

	gpio_set_pin_level(GPIO_SOL3,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_SOL3, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_SOL3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB13

	gpio_set_pin_level(GPIO_SOL4,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_SOL4, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_SOL4, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB14

	gpio_set_pin_level(GPIO_SOL5,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_SOL5, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_SOL5, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB15

	gpio_set_pin_level(GPIO_SOL6,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_SOL6, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_SOL6, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB16

	gpio_set_pin_level(GPIO_SOL7,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_SOL7, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_SOL7, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB17

	gpio_set_pin_level(GPIO_SOL8,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_SOL8, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_SOL8, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB31

	gpio_set_pin_level(GPIO_LED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LED, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LED, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC00

	// Set pin direction to input
	gpio_set_pin_direction(GPIO_IN_SW1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GPIO_IN_SW1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(GPIO_IN_SW1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC01

	// Set pin direction to input
	gpio_set_pin_direction(GPIO_IN_SW2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GPIO_IN_SW2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(GPIO_IN_SW2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC02

	// Set pin direction to input
	gpio_set_pin_direction(GPIO_IN_SW3, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GPIO_IN_SW3,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(GPIO_IN_SW3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC03

	// Set pin direction to input
	gpio_set_pin_direction(GPIO_IN_SW4, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GPIO_IN_SW4,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(GPIO_IN_SW4, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC05

	// Set pin direction to input
	gpio_set_pin_direction(GPIO_IN_SW6, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GPIO_IN_SW6,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(GPIO_IN_SW6, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC06

	// Set pin direction to input
	gpio_set_pin_direction(GPIO_IN_SW7, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GPIO_IN_SW7,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(GPIO_IN_SW7, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC07

	// Set pin direction to input
	gpio_set_pin_direction(GPIO_IN_SW8, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GPIO_IN_SW8,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(GPIO_IN_SW8, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC10

	gpio_set_pin_level(GPIO_AD_CS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_AD_CS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_AD_CS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC11

	// Set pin direction to input
	gpio_set_pin_direction(GPIO_AD_RVS, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(GPIO_AD_RVS,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(GPIO_AD_RVS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC12

	gpio_set_pin_level(GPIO_AD_RST,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_AD_RST, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_AD_RST, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC16

	gpio_set_pin_level(GPIO_LAT_VOUT_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_VOUT_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_VOUT_EN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC17

	gpio_set_pin_level(GPIO_MUX_VOUT_ENA_AL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX_VOUT_ENA_AL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX_VOUT_ENA_AL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC18

	gpio_set_pin_level(GPIO_LAT_ETC_X_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_ETC_X_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_ETC_X_EN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC19

	gpio_set_pin_level(GPIO_MUX_ETC_X_ENA_AL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX_ETC_X_ENA_AL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX_ETC_X_ENA_AL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC20

	gpio_set_pin_level(GPIO_LAT_ETC_Y_EN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LAT_ETC_Y_EN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LAT_ETC_Y_EN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC21

	gpio_set_pin_level(GPIO_MUX_ETC_Y_ENA_AL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_MUX_ETC_Y_ENA_AL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_MUX_ETC_Y_ENA_AL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC26

	gpio_set_pin_level(GPIO_LED_READY,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LED_READY, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LED_READY, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC27

	gpio_set_pin_level(GPIO_LED_OK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LED_OK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LED_OK, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC28

	gpio_set_pin_level(GPIO_LED_NG,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(GPIO_LED_NG, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GPIO_LED_NG, GPIO_PIN_FUNCTION_OFF);

	TIMER_RTC_init();

	USART_CLOCK_init();
	USART_init();
	USART_PORT_init();

	I2C_init();

	SPI_init();

	delay_driver_init();

	TIMER_3_init();
}

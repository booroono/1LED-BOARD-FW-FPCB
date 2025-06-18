/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static struct timer_task TIMER_RTC_task1, TIMER_RTC_task2;
/**
 * Example of using TIMER_RTC.
 */
static void TIMER_RTC_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_RTC_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_RTC_example(void)
{
	TIMER_RTC_task1.interval = 100;
	TIMER_RTC_task1.cb       = TIMER_RTC_task1_cb;
	TIMER_RTC_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_RTC_task2.interval = 200;
	TIMER_RTC_task2.cb       = TIMER_RTC_task2_cb;
	TIMER_RTC_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_RTC, &TIMER_RTC_task1);
	timer_add_task(&TIMER_RTC, &TIMER_RTC_task2);
	timer_start(&TIMER_RTC);
}

/*
 * \Write data to usart interface
 *
 * \param[in] buf Data to write to usart
 * \param[in] length The number of bytes to write
 *
 * \return The number of bytes written.
 */
static uint32_t USART_write(const uint8_t *const buf, const uint16_t length)
{
	uint32_t offset = 0;

	ASSERT(buf && length);

	while (!USART_is_byte_sent())
		;
	do {
		USART_write_byte(buf[offset]);
		while (!USART_is_byte_sent())
			;
	} while (++offset < length);

	return offset;
}

/*
 * \Read data from usart interface
 *
 * \param[in] buf A buffer to read data to
 * \param[in] length The size of a buffer
 *
 * \return The number of bytes read.
 */
static uint32_t USART_read(uint8_t *const buf, const uint16_t length)
{
	uint32_t offset = 0;

	ASSERT(buf && length);

	do {
		while (!USART_is_byte_received())
			;
		buf[offset] = USART_read_byte();
	} while (++offset < length);

	return offset;
}

/**
 * Example of using USART to write the data which received from the usart interface to IO.
 */
void USART_example(void)
{
	uint8_t data[2];

	if (USART_read(data, sizeof(data)) == 2) {
		USART_write(data, 2);
	}
}

void I2C_example(void)
{
	struct io_descriptor *I2C_io;

	i2c_m_sync_get_io_descriptor(&I2C, &I2C_io);
	i2c_m_sync_enable(&I2C);
	i2c_m_sync_set_slaveaddr(&I2C, 0x12, I2C_M_SEVEN);
	io_write(I2C_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using SPI to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI[12] = "Hello World!";

void SPI_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI, &io);

	spi_m_sync_enable(&SPI);
	io_write(io, example_SPI, 12);
}

void delay_example(void)
{
	delay_ms(5000);
}

static struct timer_task TIMER_3_task1, TIMER_3_task2;

/**
 * Example of using TIMER_3.
 */
static void TIMER_3_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_3_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_3_example(void)
{
	TIMER_3_task1.interval = 100;
	TIMER_3_task1.cb       = TIMER_3_task1_cb;
	TIMER_3_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_3_task2.interval = 200;
	TIMER_3_task2.cb       = TIMER_3_task2_cb;
	TIMER_3_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_3, &TIMER_3_task1);
	timer_add_task(&TIMER_3, &TIMER_3_task2);
	timer_start(&TIMER_3);
}

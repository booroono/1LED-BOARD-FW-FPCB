/*
 * def.h
 *
 * Created: 2025-04-09 오전 12:29:40
 *  Author: bruno
 */ 


#ifndef DEF_H_
#define DEF_H_

/* test define */
#define MAX_STEP								9
#define MAX_CONNECTOR						48

/* usart define */
#define USART_MAX_DATA					420
#define USART_SOT								0x10
#define USART_EOT								0x20
#define USART_UI_ADDR						0x1
#define USART_EQ_ADDR						0x2
#define USART_MODE_HEADER				0
#define USART_MODE_DATA					1

/* adc define */
#define AD_CNT									6
#define AD_GAIN_V_5_1_2					(float)5120.0 / 65535
#define AD_GAIN_V_5_1_2_0				(float)51200.0 / 65535
#define AD_REF_R_1K							1000
#define AD_REF_R_10K						10000
#define AD_REF_R_100K						100000
#define AD_REF_R_1M							1000000
#define AD_REF_R_10M						10000000
#define AD_3_3V									3300
#define AD_1_8V									1800
#define AD_RESULT_V							0
#define AD_RESULT_R							1
#define AD_R_SPAN_100           0.05
#define AD_R_SPAN_4500          0.00113636
#define AD_R_SPAN_119K          0.000873362
#define AD_R_SPAN_318K          0.000251256
#define AD_R_SPAN_2M            0.000029726
#define AD_R_SPAN_10M           0.000005625
#define AD_R_OFFSET_100         0
#define AD_R_OFFSET_4500        4.88638
#define AD_R_OFFSET_119K        6.069869045
#define AD_R_OFFSET_318K        80.100592
#define AD_R_OFFSET_2M          150.548
#define AD_R_OFFSET_10M         198.75
#define AD_WAIT_DELAY           10
/* logic control define */
#define MUX_OFF									0xFF
#define MUX8_R_1K								0
#define MUX8_R_10K							1
#define MUX8_R_100K							2
#define MUX8_R_1M								3
#define MUX8_R_10M							4
#define MUX8_P_3_3V							5
#define MUX8_AMP_1_8V						6
#define MUX8_AMP_3_3V						7
#define MUX4_AMP_1_8V						0
#define MUX4_AMP_3_3V						1
#define MUX4_P_1_8V							2
#define MUX4_P_3_3V							3
#define MUX2_4_ETC_AD						0
#define MUX2_4_I2C							3
#define INT_REF_R								MUX8_R_1M

/* hall Senseor define */
#define HALL_SENSOR_READ_CNT							10
#define HALL_SENSOR_SELF_TEST_ON					0x80
#define HALL_SENSOR_SELF_TEST_OFF					0
#define HALL_SENSOR_SINGLE_TEST						0x1
#define HALL_SENSOR_STATUS_DRDY_bm				0x1
#define HALL_SENSOR_STATUS_ERR_bm					0x20

/* proximity Senseor define */
#define PROX_SENSOR_STOP									0
#define PROX_SENSOR_START									1

/* solvalve define */
#define SOL_OFF									0
#define SOL_ON									1
#define SOL_ALL_OFF							0xFF
#define SOL_VACCUM							10
#define SOL_FPCB								11
#define SOL_DOWN								12
#define SOL_LED_X								13
#define SOL_PROX								14
#define SOL_POGO_LED_Y					15
#define OUT_SOL_VACCUM					0x1
#define OUT_SOL_FPCB						0x2
#define OUT_SOL_DOWN						0x4
#define OUT_SOL_LED_X						0x8
#define OUT_SOL_PROX			      0x10
#define OUT_SOL_POGO_LED_Y			0x20

/* switch define */
#define SW_START1								1
#define SW_VACCUM								2
#define SW_RESET								3
#define SW_START2								4

/* etc define */
#define SIDE_RIGHT							0
#define SIDE_LEFT								1

#endif /* DEF_H_ */
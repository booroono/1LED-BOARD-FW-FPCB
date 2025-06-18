/*
 * ext_control.h
 *
 * Created: 2025-04-09 오전 12:36:25
 *  Author: bruno
 */ 


#ifndef EXT_CONTROL_H_
#define EXT_CONTROL_H_

#include "main.h"

typedef enum {
	OUT_OFF = 0, OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, OUT8, OUT9, OUT10,
	OUT11, OUT12, OUT13, OUT14, OUT15, OUT16, OUT17, OUT18, OUT19, OUT20, 
	OUT21, OUT22, OUT23, OUT24, OUT25, OUT26, OUT27, OUT28, OUT29, OUT30, 
	OUT31, OUT32, OUT33, OUT34, OUT35, OUT36, OUT37, OUT38, OUT39, OUT40,
	OUT41, OUT42, OUT43, OUT44, OUT45, OUT46, OUT47, OUT48
}connector_out_e;

typedef enum {
	SEL_OUT_POSITIVE = 0, SEL_OUT_NEGATIVE,	SEL_OUT_VOUT, SEL_OUT_ETC_X, SEL_OUT_ETC_Y
}slect_latch_e;

typedef enum {
	SEL_MUX8	=	0, SEL_MUX4, SEL_MUX2_4, SEL_MUX_AD
}select_mux_e;

typedef enum { 
	SEL_AD_MUX8 = 0, SEL_AD_MUX4, SEL_AD_ETC, SEL_AD_CC_1_8V, 
	SEL_AD_CC_3_3V, SEL_AD_CDS1, SEL_AD_CDS2, SEL_AD_CDS3
}select_ad_e;

typedef struct {
	uint8_t ref1, ref2, etc, sol;
	uint8_t conn[5];
}out_state_t;

typedef struct {
	slect_latch_e latch;
	select_mux_e mux;
	select_ad_e ad;
}select_logic_t;

extern uint8_t vaccumFlag;
extern out_state_t outState;
extern select_logic_t selectLogic;

void Latch_Control(uint32_t enPin, uint8_t data);
void Out_Control(uint8_t selectLatch, uint8_t ctrlOut);
void Out_All_Off(void);
void MUX_Control(uint8_t selectMux, uint8_t ctrlMux);
void MUX_All_Off(void);
uint8_t Input_Check(const uint8_t pin);
void Solenoid_Valve_Control(uint8_t control, uint8_t solNumber);
void Board_Reset(void);

#endif /* EXT_CONTROL_H_ */
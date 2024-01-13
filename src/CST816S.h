/*****************************************************************************
 * | File      	:   CST816S.h
 * | Author      :   Waveshare team
 * | Function    :   Hardware underlying interface
 * | Info        :
 *                Used to shield the underlying layers of each master
 *                and enhance portability
 *----------------
 * |	This version:   V1.0
 * | Date        :   2022-12-02
 * | Info        :   Basic version
 *
 ******************************************************************************/
#ifndef __CST816S_H
#define __CST816S_H
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"


typedef enum
{

	CST816_GestureID = 0x01,
	CST816_FingerNum,
	CST816_XposH,
	CST816_XposL,
	CST816_YposH,
	CST816_YposL,

	CST816_ChipID = 0xA7,
	CST816_ProjID,
	CST816_FwVersion,
	CST816_MotionMask,

	CST816_BPC0H = 0xB0,
	CST816_BPC0L,
	CST816_BPC1H,
	CST816_BPC1L,

	CST816_IrqPluseWidth = 0xED,
	CST816_NorScanPer,
	CST816_MotionSlAngle,
	CST816_LpScanRaw1H =0XF0,
	CST816_LpScanRaw1L,
	CST816_LpScanRaw2H,
	CST816_LpScanRaw2L,
	CST816_LpAutoWakeTime,
	CST816_LpScanTH,
	CST816_LpScanWin,
	CST816_LpScanFreq,
	CST816_LpScanIdac,
	CST816_AutoSleepTime,
	CST816_IrqCtl,
	CST816_AutoReset,
	CST816_LongPressTime,
	CST816_IOCtl,
	CST816_DisAutoSleep
} CST816S_Register;

/**
 * Whether the graphic is filled
 **/
typedef enum
{
	CST816S_Point_Mode = 1,
	CST816S_Gesture_Mode,
	CST816S_ALL_Mode,
} CST816S_Mode;

typedef enum
{
	CST816S_Gesture_None = 0,
	CST816S_Gesture_Up,
	CST816S_Gesture_Down,
	CST816S_Gesture_Left,
	CST816S_Gesture_Right,
	CST816S_Gesture_Click,
	CST816S_Gesture_Double_Click = 0x0b,
	CST816S_Gesture_Long_Press=0x0c,
} CST816S_Gesture;

typedef struct
{
	uint16_t x_point;
	uint16_t y_point;
	CST816S_Mode mode;
	bool interrupt;
} CST816S_Touch;

//extern CST816S Touch_CTS816;

class CST816S {
public:
	auto init(uint8_t mode) -> uint8_t;
	auto get_Point() -> CST816S_Touch;
	auto get_Gesture(void) -> uint8_t;
private:
    auto I2C_Write(uint8_t reg, uint8_t value) -> void;
	auto I2C_Read(uint8_t reg) -> uint8_t;
	auto WhoAmI() -> uint8_t;
    auto Reset() -> void;
    auto Read_Revision() -> uint8_t;
	auto Wake_up() -> void;
	auto Stop_Sleep() -> void;
	auto Set_Mode(uint8_t mode) -> void;
	static inline bool interrupt_happened;
	static auto gpio_irq_callback(uint gpio, uint32_t event_mask) -> void
    {
        interrupt_happened = true;
    }
	CST816S_Touch Touch_CTS816;
	const uint8_t DEV_SDA_PIN = 6;
	const uint8_t DEV_SCL_PIN = 7;
	const uint8_t Touch_INT_PIN = 21;
	const uint8_t Touch_RST_PIN = 22;
	const uint8_t CST816_ADDR = 0x15;
	i2c_inst_t *I2C_PORT = i2c1;
};

#endif

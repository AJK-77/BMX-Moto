#pragma once

// ======================
// TFT
// ======================

#define TFT_BACKLIGHT 21


// ======================
// I2C
// ======================

#define I2C_SDA 27
#define I2C_SCL 22


// ======================
// TCA8418
// ======================

#define TCA8418_ADDR 0x34
#define TCA8418_INT  35


// ======================
// TCA8418 Keyboard Matrix
// ======================

// Normal matrix
#define TCA_R0 0
#define TCA_R1 1
#define TCA_R2 2
#define TCA_R3 3
#define TCA_R4 4
#define TCA_R5 5
#define TCA_R6 6

#define TCA_C0 0
#define TCA_C1 1
#define TCA_C2 2

// 2ndF - separate 1x1 matrix
#define TCA_R7 7
#define TCA_C3 3


// ======================
// TCA8418 GPIO
// ======================

#define TCA_C4_RESERVE       4
#define TCA_C5_IP5306_KEY    5
#define TCA_C6_BATTERY_25    6
#define TCA_C7_BATTERY_50    7
#define TCA_C8_BATTERY_75    8
#define TCA_C9_BATTERY_100   9


// ======================
// Audio
// ======================

#define AUDIO_OUT 26
// ============================================
// The header file for general project settings
// Spring 2018 Gatech ECE2035
//=============================================
#ifndef GLOBAL_H
#define GLOBAL_H
#define WIDTH 125
#define HEIGHT 50
#define NUMBUCKETS 100
#define BURGER_COUNT   5
#define BABY       5


#define BACKGROUND  0x4E5252
#define BUN         0xE5B149
#define LETTUCE     0x2EFF1F
#define TOMATO      RED
#define CHEESE      0xFFC400
#define MEAT        0x552F0D
#define DARKBLUE    0x0112FF
#define PINK2       0xFF00EF
#define LIGHTBLUE   0x00FFDE
#define LIGHTGREEN  0x66FF00
#define GRAY2       0x787978
#define MOUTH       0xB9B2D1
#define SKIN1       0xFFE2A0
#define SKIN2       0x7C4F31
#define CHAIR       0xC6C3BE
#define CHAIR2      0xE12EDB

// Include all the hardware libraries
#include "mbed.h"
#include "wave_player.h"
#include "MMA8452.h"
#include "uLCD_4DGL.h"
#include "SDFileSystem.h"

// Declare the hardware interface objects
extern uLCD_4DGL uLCD;      // LCD Screen
extern SDFileSystem sd;     // SD Card
extern Serial pc;           // USB Console output
extern MMA8452 acc;       // Accelerometer
extern DigitalIn button1;   // Pushbuttons
extern DigitalIn button2;
extern DigitalIn button3;
extern AnalogOut DACout;    // Speaker
extern PwmOut speaker;
extern wave_player waver;

// === [define the macro of error heandle function] ===
// when the condition (c) is not true, assert the program and show error code
#define ASSERT_P(c,e) do { \
    if(!(c)){ \
        pc.printf("\nERROR:%d\n",e); \
        while(1); \
    } \
} while (0)

// === [error code] ===
#define ERROR_NONE 0 // All good in the hood
#define ERROR_MEH -1 // This is how errors are done

#endif //GLOBAL_H
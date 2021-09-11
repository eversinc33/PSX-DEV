#ifndef GAMEPAD_H_  
#define GAMEPAD_H_

#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	    // Not necessary but include it anyway
#include <libetc.h>	    // Includes some functions that controls the display
#include <libgte.h>	    // GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	    // GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs

#define PAD_SELECT      1
#define PAD_L3          2
#define PAD_R3          4
#define PAD_START       8
#define PAD_UP          16
#define PAD_RIGHT       32
#define PAD_DOWN        64
#define PAD_LEFT        128
#define PAD_L2          256
#define PAD_R2          512
#define PAD_L1          1024
#define PAD_R1          2048
#define PAD_TRIANGLE    4096
#define PAD_CIRCLE      8192
#define PAD_CROSS       16384
#define PAD_SQUARE      32768

#define PAD_TYPE_DIGITAL      0x4
#define PAD_TYPE_DUAL_ANALOG  0x5
#define PAD_TYPE_DUAL_SHOCK   0x6

#define PAD_STATUS_CONNECTED  0

typedef struct _PADTYPE
{
    unsigned char	stat;
    unsigned char	len:4;
    unsigned char	type:4;
    unsigned short	btn;
    unsigned char	rs_x,rs_y;
    unsigned char	ls_x,ls_y;
} PADTYPE;

typedef struct _BUTTONS_PRESSED
{
    int left_pressed;
    int right_pressed;
    int cross_pressed;
} BUTTONS_PRESSED;

BUTTONS_PRESSED getControllerInput(); 
void initGamePad();

#endif // GAMEPAD_H_
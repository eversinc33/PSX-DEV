#ifndef PLAYER_H_  
#define PLAYER_H_

#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	    // Not necessary but include it anyway
#include <libetc.h>	    // Includes some functions that controls the display
#include <libgte.h>	    // GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	    // GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs
#include "GAMEPAD.H"

typedef enum { IDLE, MOVING } CHARACTER_STATE;

typedef struct _PLAYER_CHAR {
    CHARACTER_STATE state;
    int on_ground;
    int facing_left;
    int x;
    int y;
} PLAYER_CHAR;

PLAYER_CHAR initPlayer(int start_x, int start_y);
void movePlayer(PLAYER_CHAR* player);
void drawPlayer(PLAYER_CHAR* player, int frames_passed);

#endif // PLAYER_H_
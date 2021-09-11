#ifndef GRAPHICS_H_  
#define GRAPHICS_H_

#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	    // Not necessary but include it anyway
#include <libetc.h>	    // Includes some functions that controls the display
#include <libgte.h>	    // GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	    // GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs
#include "GAMEPAD.H"

#define OTLEN 8         // Ordering table length, 8 should be enough for 2D

typedef struct _SPRITE {
    u_short texturePage;  // Tpage value
    u_short clut;   // CLUT value
    u_char  u,v;    // UV offset (useful for non page aligned TIMs)
    u_char  width,height;    // Size (primitives can only draw 256x256 anyway)
    CVECTOR col;
} SPRITE;

void initGraphics(void);
void display();
void LoadTexture(u_long *tim, TIM_IMAGE *imageBuffer);
void GetSprite(TIM_IMAGE *image, SPRITE *sprite);
void drawPlayer(PLAYER_CHAR *player, int sprite_row, int sprite_col);
void clearCurrentOrderingTable();

#endif // GRAPHICS_H_
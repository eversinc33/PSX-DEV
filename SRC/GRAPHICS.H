#ifndef GRAPHICS_H_  
#define GRAPHICS_H_

#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	    // Not necessary but include it anyway
#include <libetc.h>	    // Includes some functions that controls the display
#include <libgte.h>	    // GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	    // GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs
#include "STRUCTS.H"

#define OTLEN 8         // Ordering table length, 8 should be enough for 2D

void initGraphics(void);
void display();
void loadSpriteFromCd(SPRITE *sprite, char *sprite_name);
void LoadTexture(u_long *tim, TIM_IMAGE *imageBuffer);
void GetSprite(TIM_IMAGE *image, SPRITE *sprite);
void drawSprite(SPRITE *sprite, int x, int y, int sprite_row, int sprite_col);
void clearCurrentOrderingTable();

#endif // GRAPHICS_H_
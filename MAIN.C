#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	    // Not necessary but include it anyway
#include <libetc.h>	    // Includes some functions that controls the display
#include <libgte.h>	    // GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	    // GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs
#include "SRC/GAMEPAD.H"
#include "SRC/PLAYER.H"
#include "SRC/GRAPHICS.H"
#include "SRC/CDROM.H"
#include "SRC/STRUCTS.H"
#include "SRC/GAME.H"

struct GAME game;

int main() {

    // Reset graphics
    ResetGraph(0);

    // init components
    initCdrom();
    initGraphics();
    initGamePad();

    initPlayer(game.player, 46, 46);

    while(1) {

        // TODO: game object storing player, offsetx/y, frames passed etc
        // TODO: scrolling with offset
        // TODO: tilemap

        clearCurrentOrderingTable();

        updatePlayer(game.player, game.frames_passed);

        display();
        
        game.frames_passed++;
        if (game.frames_passed > 60) {
            game.frames_passed = 0;
        }

        FntPrint("X:%d Y:%d", game.player->x_vel, game.player->y_vel);
    }
    
    return 0;
}
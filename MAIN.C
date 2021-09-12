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

int main() {

    int frames_passed = 0;

    PLAYER_CHAR player = initPlayer(46, 46);
    
    // Reset graphics
    ResetGraph(0);

    // init components
    initCdrom();
    initGraphics();
    initGamePad();
    
    while(1) {

        clearCurrentOrderingTable();

        updatePlayer(&player, frames_passed);

        // Debug position to screen
        FntPrint("X:%d Y:%d", player.x_vel, player.y_vel);

        display();
        
        frames_passed++;
        if (frames_passed > 60) {
            frames_passed = 0;
        }
    }
    
    return 0;
}
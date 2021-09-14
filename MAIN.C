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

int frames_passed;

int main() {

    // Reset graphics
    ResetGraph(0);

    // init components
    initCdrom();
    initGraphics();
    initGamePad();

    PLAYER_CHAR* player;
    initPlayer(player, 46, 46);

    printf("\n[!] Done\n");

    while(1) {

        // TODO: tilemap
        clearCurrentOrderingTable();

        updatePlayer(player, frames_passed);

        display();
        
        frames_passed++;
        if (frames_passed > 60) {
            frames_passed = 0;
        }
        
        FntPrint("X: %d Y: %d", player->x, player->y);

    }
    
    return 0;
}
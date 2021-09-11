#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	    // Not necessary but include it anyway
#include <libetc.h>	    // Includes some functions that controls the display
#include <libgte.h>	    // GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	    // GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs
#include "SRC/GAMEPAD.H"
#include "SRC/PLAYER.H"
#include "SRC/GRAPHICS.H"

int main() {

    int frames_passed = 0;

    PLAYER_CHAR player = initPlayer(46, 46);
    
    initGraphics();
    initGamePad();
    
    while(1) {

        clearCurrentOrderingTable();

        getControllerInput(&player);

        // Debug position to screen
        FntPrint("X:%d Y:%d", player.x, player.y);

        // Sort textured sprites for player character
        if (player.state == IDLE) 
        {
            int col = player.facing_left; // spritesheet column

            if (frames_passed > 30) 
            {
                drawPlayer(&player, 1, col);
            } 
            else 
            {
                drawPlayer(&player, 0, col);
            }
        } 

        // check ground and faill if not grounded
        if (!player.on_ground) {
            player.y++;
        }
        if (player.y == (232 - 32)) {
            player.on_ground = 1;
        }
    
        // Update the display
        display();
        
        frames_passed++;
        if (frames_passed > 60) {
            frames_passed = 0;
        }
    }
    
    return 0;
}
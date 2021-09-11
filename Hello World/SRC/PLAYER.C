#include "PLAYER.H"
#include "GRAPHICS.H"

PLAYER_CHAR initPlayer(int start_x, int start_y) {

    PLAYER_CHAR player;

    player.state = IDLE;
    player.on_ground = 0;
    player.facing_left = 0;
    player.x = start_x;
    player.y = start_y;

    return player;
}

void movePlayer(PLAYER_CHAR* player) {

    BUTTONS_PRESSED buttons_pressed = getControllerInput(); 

    // MOVE
    if (buttons_pressed.left_pressed) 
    {
        (*player).x--;
        (*player).facing_left = 1;
    }
    else if (buttons_pressed.right_pressed) 
    {
        (*player).x++;
        (*player).facing_left = 0;
    }

    // JUMP
    if (buttons_pressed.cross_pressed) 
    {
        // TODO
    }


    // FALL
    if (!player->on_ground) {
        (*player).y++;
    }
    if (player->y == (232 - 32)) { // TODO screensize macro
        (*player).on_ground = 1;
    }
}

void drawPlayer(PLAYER_CHAR *player, int frames_passed) {
    if (player->state == IDLE) 
    {
        int col = player->facing_left; // spritesheet column

        if (frames_passed > 30) 
        {
            drawPlayerSprite(player, 1, col);
        } 
        else 
        {
            drawPlayerSprite(player, 0, col);
        }
    } 
}
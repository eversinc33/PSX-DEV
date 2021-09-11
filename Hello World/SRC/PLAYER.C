#include "PLAYER.H"

PLAYER_CHAR initPlayer(int start_x, int start_y) {

    PLAYER_CHAR player;

    player.state = IDLE;
    player.on_ground = 0;
    player.facing_left = 0;
    player.x = start_x;
    player.y = start_y;

    return player;
}
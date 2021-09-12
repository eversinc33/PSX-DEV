#include "PLAYER.H"
#include "GRAPHICS.H"

int jump_pressed;
int jump_pressed_frames;

PLAYER_CHAR initPlayer(int start_x, int start_y) {

    PLAYER_CHAR player;

    player.state = IDLE;
    player.on_ground = 0;
    player.facing_left = 0;
    player.x = start_x;
    player.y = start_y;
    player.x_vel = 0;
    player.y_vel = 0;

    loadSprite("\\CHAR.TIM;1", &player.sprite);
    
    return player;
}

void updatePlayer(PLAYER_CHAR *player, int frames_passed) {

    fallIfNotOnGround(player);
    detectCollisions(player);
    movePlayer(player);
    setState(player);
    drawPlayer(player, frames_passed);

}

void setState(PLAYER_CHAR *player) {
    
    (*player).state = IDLE; 

    if (player->x_vel != 0) {
        if (player->on_ground) {
            (*player).state = WALK; 
        } 
    } 

    if (!player->on_ground) {
        (*player).state = IN_AIR;
    }
}

void fallIfNotOnGround(PLAYER_CHAR *player) {
    if (!player->on_ground) {
        if (player->y_vel < MAX_FALLING_SPEED) {
            (*player).y_vel += GRAVITY;
        }
    }
}

void detectCollisions(PLAYER_CHAR *player) {
    // TODO screensize macro, proper coll detection

    // Lower screen border
    if (player->y >= (232 - 32) && !player->on_ground) { 
        (*player).on_ground = 1;
        (*player).y_vel = 0;
        (*player).y = 232 - 32;
    }
}

void movePlayer(PLAYER_CHAR *player) {

    BUTTONS_PRESSED buttons_pressed = getControllerInput(); 

    // MOVE
    if (buttons_pressed.left_pressed) 
    {
        (*player).x_vel = -WALK_SPEED;
        (*player).facing_left = 1;
    }
    else if (buttons_pressed.right_pressed) 
    {
        (*player).x_vel = WALK_SPEED;
        (*player).facing_left = 0;
    }
    else 
    {
        (*player).x_vel = 0;
    }

    // JUMP
    if (buttons_pressed.cross_pressed && player->on_ground) 
    {
        // avoid bunny hop
        if (!jump_pressed) {
            (*player).y_vel = -JUMP_SPEED;
            (*player).on_ground = 0;
            jump_pressed = 1;
        }
    }
    if (buttons_pressed.cross_pressed && player->y_vel < 0)
    {
        // if still pressed after x frames, increase height of jump
        if (jump_pressed_frames > JUMP_INCREASE_THRESHOLD && jump_pressed_frames <= JUMP_PRESS_MAX_DURATION) {
            (*player).y_vel = player->y_vel - 1;
        }
        jump_pressed_frames++;
    }
    if (!buttons_pressed.cross_pressed)
    {
        jump_pressed = 0;
        jump_pressed_frames = 0;
    }

    (*player).x += player->x_vel;
    (*player).y += player->y_vel;
}

void drawPlayer(PLAYER_CHAR *player, int frames_passed) {

    int col = player->facing_left; // spritesheet column
    
    if (player->state == IDLE) 
    {
        

        if (frames_passed > 30) 
        {
            drawSprite(&player->sprite, player->x, player->y, 1, col);
        } 
        else 
        {
            drawSprite(&player->sprite, player->x, player->y, 0, col);
        }
    } 

    else if (player->state == WALK) 
    {
        int col = player->facing_left; // spritesheet column

        if (frames_passed > 30) 
        {
            drawSprite(&player->sprite, player->x, player->y, 1, col);
        } 
        else 
        {
            drawSprite(&player->sprite, player->x, player->y, 0, col);
        }
    } 

    else if (player->state == IN_AIR) 
    {
        int col = player->facing_left; // spritesheet column

        if (frames_passed > 30) 
        {
            drawSprite(&player->sprite, player->x, player->y, 1, col);
        } 
        else 
        {
            drawSprite(&player->sprite, player->x, player->y, 0, col);
        }
    } 
}
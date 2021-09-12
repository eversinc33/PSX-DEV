#ifndef GAME_H_  
#define GAME_H_ 

struct GAME {
    PLAYER_CHAR *player;
    int frames_passed;
    int camera_offset_x;
    int camera_offset_y;
};

extern struct GAME game;

#endif // GAME_H_ 
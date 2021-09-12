#ifndef STRUCTS_H_  
#define STRUCTS_H_

typedef enum { IDLE, WALK, IN_AIR } CHARACTER_STATE;

typedef struct _SPRITE {
    u_short texturePage;  // Tpage value
    u_short clut;   // CLUT value
    u_char  u,v;    // UV offset (useful for non page aligned TIMs)
    u_char  width,height;    // Size (primitives can only draw 256x256 anyway)
    CVECTOR col;
} SPRITE;

typedef struct _PLAYER_CHAR {
    CHARACTER_STATE state;
    SPRITE sprite;
    int on_ground;
    int facing_left;
    int x;
    int y;
    int x_vel;
    int y_vel;
} PLAYER_CHAR;

#endif // STRUCTS_H_
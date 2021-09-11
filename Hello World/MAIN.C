#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	    // Not necessary but include it anyway
#include <libetc.h>	    // Includes some functions that controls the display
#include <libgte.h>	    // GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	    // GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs

#define OTLEN 8         // Ordering table length, 8 should be enough for 2D

DISPENV disp[2];        // Display double buffer
DRAWENV draw[2];        // Draw double buffer
int db = 0;

u_long ot[2][OTLEN];    // Double buffered ordering table
char pribuff[2][32768]; // Double buffered primitive buffer
char *nextpri;          // Pointer to next primitive

#define PAD_SELECT      1
#define PAD_L3          2
#define PAD_R3          4
#define PAD_START       8
#define PAD_UP          16
#define PAD_RIGHT       32
#define PAD_DOWN        64
#define PAD_LEFT        128
#define PAD_L2          256
#define PAD_R2          512
#define PAD_L1          1024
#define PAD_R1          2048
#define PAD_TRIANGLE    4096
#define PAD_CIRCLE      8192
#define PAD_CROSS       16384
#define PAD_SQUARE      32768

#define PAD_TYPE_DIGITAL      0x4
#define PAD_TYPE_DUAL_ANALOG  0x5
#define PAD_TYPE_DUAL_SHOCK   0x6

#define PAD_STATUS_CONNECTED  0

typedef struct _PADTYPE
{
    unsigned char	stat;
    unsigned char	len:4;
    unsigned char	type:4;
    unsigned short	btn;
    unsigned char	rs_x,rs_y;
    unsigned char	ls_x,ls_y;
} PADTYPE;

u_char padbuff[2][34]; // Pad buffer arrays

typedef struct _SPRITE {
    u_short texturePage;  // Tpage value
    u_short clut;   // CLUT value
    u_char  u,v;    // UV offset (useful for non page aligned TIMs)
    u_char  width,height;    // Size (primitives can only draw 256x256 anyway)
    CVECTOR col;
} SPRITE;

SPRITE my_sprite;
TIM_IMAGE my_image;   
extern u_long char_sprite[]; // Refers to assembled tim-obj

typedef enum { IDLE, MOVING } CHARACTER_STATE;

typedef struct _PLAYER_CHAR {
    CHARACTER_STATE state;
    int x;
    int y;
} PLAYER_CHAR;

// Draw the screen
void display() {
    
    // Wait for any graphics processing to finish
    DrawSync(0);          

    // Wait for vertical retrace      
    VSync(0);                   

    // Apply the current buffers
    PutDispEnv(&disp[db]);      
    PutDrawEnv(&draw[db]);

    // Enable the display
    SetDispMask(1);             

    // Draw the ordering table
    DrawOTag(ot[db]+OTLEN-1);   

    // Flush debug font
    FntFlush(-1);
    
    // Swap buffers & reset next primitive pointer
    db = !db;                    
    nextpri = pribuff[db];  
}

// Load texture from .TIM obj into imageBuffer
void LoadTexture(u_long *tim, TIM_IMAGE *imageBuffer) {

    // Read .TIM parameters
    OpenTIM(tim);
    ReadTIM(imageBuffer);

    // Load pixel into framebuffer
    LoadImage(imageBuffer->prect, (u_long*)imageBuffer->paddr);
    DrawSync(0);

    // Upload CLUT to framebuffer if present
    if (imageBuffer->mode & 0x8) {
        LoadImage(imageBuffer->crect, (u_long*)imageBuffer->caddr);
        DrawSync(0);
    }
}

void GetSprite(TIM_IMAGE *image, SPRITE *sprite) {

    // Get tpage value
    sprite->texturePage = getTPage(image->mode&0x3, 0, 
        image->prect->x, image->prect->y);
        
    // Get CLUT value
    if( image->mode & 0x8 ) {
        sprite->clut = getClut(image->crect->x, image->crect->y);
    }
    
    // Set sprite size
    sprite->width = image->prect->w << (2 - (image->mode & 0x3));
    sprite->height = image->prect->h;
    
    // Set UV offset
    sprite->u = (image->prect->x & 0x3f) << (2 - (image->mode & 0x3));
    sprite->v = image->prect->y & 0xff;
    
    // Set neutral color
    sprite->col.r = 128;
    sprite->col.g = 128;
    sprite->col.b = 128;
    
}

void init(void) {
	
    // Reset graphics
    ResetGraph(0);

    // Init double buffers (draw- and display-buffers)
    SetDefDispEnv(&disp[0], 0, 0, 320, 240);
    SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
    SetDefDispEnv(&disp[1], 0, 240, 320, 240);
    SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

    // Clear double buffer background in dark purple
    draw[0].isbg = 1;               
    setRGB0(&draw[0], 63, 0, 127);  
    draw[1].isbg = 1;
    setRGB0(&draw[1], 63, 0, 127);

    // Set initial primitive pointer address to beginning of the primitive buffer
    nextpri = pribuff[0];           
 
    // load textures 
    LoadTexture(char_sprite, &my_image);
    GetSprite(&my_image, &my_sprite);

    // Load the internal font texture
    FntLoad(960, 0);
    // Create the text stream
    FntOpen(0, 8, 320, 224, 0, 100);

    // apply initial drawing environment
    PutDrawEnv(&draw[!db]);
    
    // Initialize the pads
    InitPAD( padbuff[0], 34, padbuff[1], 34 );
    
    // Begin polling
    StartPAD();
    
    // To avoid VSync Timeout error, may not be defined in PsyQ
    ChangeClearPAD(1);
 
}

// Add spriteToRender to the ordering table
void sortSprite(int x, int y, SPRITE *spriteToRender, DRAWENV *currentDrawEnv, u_long *currentOrderingTable) {

    SPRT *sprt = (SPRT*)nextpri;   

    // set texture page of sprite as tpage
    currentDrawEnv->tpage = spriteToRender->texturePage;

    // initialize
    setSprt(sprt);                 

    // set values (pos, size, uv, clut, color)
    setXY0(sprt, x, y);           
    setWH(sprt, spriteToRender->width, spriteToRender->height); 
    setUV0(sprt, spriteToRender->u, spriteToRender->v);
    sprt->clut = spriteToRender->clut;
    setRGB0(sprt, 128, 128, 128);

    // Sort primitive to the ordering table
    addPrim(currentOrderingTable, sprt);        

    // Advance next primitive address pointer
    nextpri += sizeof(SPRT);        
}

void sortSpriteSheetSprite(int x, int y, int sprite_width, int sprite_height, int sprite_row, int sprite_col, SPRITE *spriteToRender, DRAWENV *currentDrawEnv, u_long *currentOrderingTable) {

    SPRT *sprt = (SPRT*)nextpri;   

    // calculate the u, v of the sprite to render from the sheet
    int u = sprite_row * sprite_width;
    int v = sprite_col * sprite_height;

    // set texture page of sprite as tpage
    currentDrawEnv->tpage = spriteToRender->texturePage;

    // initialize
    setSprt(sprt);                 

    // set values (pos, size, uv, clut, color)
    setXY0(sprt, x, y);           
    setWH(sprt, sprite_width, sprite_height); 
    setUV0(sprt, u, v);
    sprt->clut = spriteToRender->clut;
    setRGB0(sprt, 128, 128, 128);

    // Sort primitive to the ordering table
    addPrim(currentOrderingTable, sprt);        

    // Advance next primitive address pointer
    nextpri += sizeof(SPRT);        
}

// Add a cube primitive to the ordering table
void sortCube64(int x, int y, u_long *currentOrderingTable) {

    TILE *tile = (TILE*)nextpri;    
    int width  = 64;
    int height = 64;

    // initialize
    setTile(tile);

    // set values  
    setXY0(tile, x, y);           
    setWH(tile, width, height);      
    setRGB0(tile, 255, 255, 0);     

    // Sort primitive to the ordering table
    addPrim(currentOrderingTable, tile);          
        
    // Advance next primitive address pointer
    nextpri += sizeof(TILE);        
}

void getControllerInput(PLAYER_CHAR *player) {
    
  	PADTYPE *pad;
    pad = (PADTYPE*)padbuff[0];

    if (pad->stat == PAD_STATUS_CONNECTED) 
    {
        if ((pad->type == PAD_TYPE_DIGITAL) || (pad->type == PAD_TYPE_DUAL_ANALOG) || (pad->type == PAD_TYPE_DUAL_SHOCK)) 
        {
            if(!(pad->btn & PAD_LEFT)) 
            {
                (*player).x--;
            } 
            else if(!(pad->btn & PAD_RIGHT)) 
            {
                (*player).x++;
            }

            if(!(pad->btn & PAD_UP)) 
            {
                (*player).y--;
            } 
            else if(!(pad->btn & PAD_DOWN)) 
            {
                (*player).y++;
            }
        }
    }
}

int main() {

    int frames_passed = 0;

    PLAYER_CHAR player = {
        IDLE,
        46,
        46
    };
    
    init();
    
    while(1) {
    
        getControllerInput(&player);

        // Debug position to screen
        FntPrint("X:%d Y:%d", player.x, player.y);

        // Clear ordering table
        ClearOTagR(ot[db], OTLEN);      
    
        // Sort textured sprites for player character
        if (player.state == IDLE) 
        {
            if (frames_passed > 30) 
            {
                sortSpriteSheetSprite(player.x, player.y, 16, 32, 1, 0, &my_sprite, &draw[db], ot[db]);
            } 
            else 
            {
                sortSpriteSheetSprite(player.x, player.y, 16, 32, 0, 0, &my_sprite, &draw[db], ot[db]);
            }
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
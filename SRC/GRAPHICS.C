#include "GRAPHICS.H"
#include "CDROM.H"

DISPENV disp[2];        // Display double buffer
DRAWENV draw[2];        // Draw double buffer
int db = 0;

u_long ot[2][OTLEN];    // Double buffered ordering table
char pribuff[2][32768]; // Double buffered primitive buffer
char *nextpri;          // Pointer to next primitive

SPRITE player_sprite;
TIM_IMAGE player_image;   
u_long *filebuff;      // Pointer for the file loaded from the disc

void initGraphics(void) {

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
 
    // load player texture
    if (filebuff = loadFileFromCdrom("\\CHAR.TIM;1"))
    {    
        LoadTexture(filebuff, &player_image);
        GetSprite(&player_image, &player_sprite);
        
        free(filebuff);
    }
    else
    {
        printf( "Error: CHAR.TIM file not found.\n" );
    }
    
    // Load the internal font texture
    FntLoad(960, 0);
    // Create the text stream
    FntOpen(0, 8, 320, 224, 0, 100);

    // apply initial drawing environment
    PutDrawEnv(&draw[!db]); 
}

void clearCurrentOrderingTable() {
    ClearOTagR(ot[db], OTLEN); 
}

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
    sprite->u = (image->prect->x % 64) << (2 - (image->mode & 0x3));
    sprite->v = image->prect->y & 0xff;
    
    // Set neutral color
    sprite->col.r = 128;
    sprite->col.g = 128;
    sprite->col.b = 128;
    
}

void drawPlayerSprite(PLAYER_CHAR *player, int sprite_row, int sprite_col) {

    SPRT *sprt = (SPRT*)nextpri;   

    // calculate the u, v of the sprite to render from the sheet
    int u = sprite_row * 16; // TODO get from player
    int v = sprite_col * 32;
    u_long *current_ordering_table = ot[db];

    // set texture page of sprite as tpage for current draw environment
    draw[db].tpage = player_sprite.texturePage;  // TODO get sprite to render from player

    // initialize
    setSprt(sprt);                 

    // set values (pos, size, uv, clut, color)
    setXY0(sprt, player->x, player->y);           
    setWH(sprt, 16, 32);  // TODO get width and height of sprite from player 
    setUV0(sprt, u, v);
    sprt->clut = player_sprite.clut; // TODO get sprite to render from player
    setRGB0(sprt, 128, 128, 128);

    // Sort primitive to the ordering table
    addPrim(current_ordering_table, sprt);        

    // Advance next primitive address pointer
    nextpri += sizeof(SPRT);        
}

/*
// Add a cube primitive to the ordering table
void sortCube64(int x, int y, u_long *curr_ot) {

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
    addPrim(curr_ot, tile);          
        
    // Advance next primitive address pointer
    nextpri += sizeof(TILE);        
}

// Add sprite_to_render to the ordering table
void sortSprite(int x, int y, SPRITE *sprite_to_render, DRAWENV *curr_draw_env, u_long *curr_ot) {

    SPRT *sprt = (SPRT*)nextpri;   

    // set texture page of sprite as tpage
    curr_draw_env->tpage = sprite_to_render->texturePage;

    // initialize
    setSprt(sprt);                 

    // set values (pos, size, uv, clut, color)
    setXY0(sprt, x, y);           
    setWH(sprt, sprite_to_render->width, sprite_to_render->height); 
    setUV0(sprt, sprite_to_render->u, sprite_to_render->v);
    sprt->clut = sprite_to_render->clut;
    setRGB0(sprt, 128, 128, 128);

    // Sort primitive to the ordering table
    addPrim(curr_ot, sprt);        

    // Advance next primitive address pointer
    nextpri += sizeof(SPRT);        
}
*/
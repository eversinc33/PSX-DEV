#include "CDROM.H"
#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	    // Not necessary but include it anyway
#include <libetc.h>	    // Includes some functions that controls the display
#include <libgte.h>	    // GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	    // GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs
#include <libcd.h>     

TIM_IMAGE player_image; 
u_long *filebuff;      // Pointer for the file loaded from the disc

void initCdrom() {
    CdInit();
}

u_long *loadFileFromCdrom(char *filename)
{
    CdlFILE filePos;
    int     sectors_to_read;
    u_long  *buff;
    
    buff = NULL;
    
    // locate the file on the CD 
    if (CdSearchFile(&filePos, filename) == NULL)
    {
        printf( "%s not found.", filename );
    }
    else
    {
        sectors_to_read = (filePos.size + 2047) / 2048;
        
        buff = (u_long*) malloc(2048 * sectors_to_read);
        
        CdControl(CdlSetloc, (u_char*) &filePos.pos, 0);
        CdRead(sectors_to_read, buff, CdlModeSpeed);

        CdReadSync(0, 0);
    }
    
    return buff;
}
#ifndef CDROM_H_  
#define CDROM_H_ 

#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h

void initCdrom();
u_long *loadFileFromCdrom(char *filename);

#endif // CDROM_H_ 
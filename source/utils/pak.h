
#ifndef __PAK_PAK_H__
#define __PAK_PAK_H__

int		PAK_doPak(u8 * Dest,u8 const * source,int insize);
int		PAK_doUnpak(u8 * Dest,u8 const * Source);
int		PAK_findPakSize(u8 const * souce,int insize);
int		PAK_getLastAmountOfDataRead(void);



#endif	

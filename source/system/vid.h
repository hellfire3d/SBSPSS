/******************/
/*** GFx System ***/
/******************/

#ifndef __VID_HEADER_
#define __VID_HEADER_

#include	"system\global.h"

/*****************************************************************************/
#define	GEOM_SCREEN_H			(350)

#define	INGAME_SCREENW			(512)
#define	INGAME_SCREENH			(256)

#define	INGAME_RENDER_OFS_X		(8)
#define	INGAME_RENDER_OFS_Y		(4)
#define	INGAME_SCREENOFS_X		(-INGAME_SCREENW/2)
#define	INGAME_SCREENOFS_Y		(-INGAME_SCREENH/2)

/*****************************************************************************/
typedef void (*VbFuncType)(void);

/*****************************************************************************/
struct sVidScreen
{
	DRAWENV		Draw;
	DISPENV		Disp;
};

/*****************************************************************************/
extern int		FrameFlipFlag;

/*****************************************************************************/
void		VidInit();

void		VidScrOn();
void		VidSetRes(int x, int y);
void		VidSetClearScreen(int );
void		VidSetClearColor( const CVECTOR & col );

int			VidGetXOfs();
int 		VidGetYOfs();
void		VidSetXYOfs(int x,int y);
int 		VidGetScrW();
int 		VidGetScrH();
sVidScreen	*VidGetScreen();
sVidScreen	*VidGetDispScreen();
sVidScreen	*VidGetDrawScreen();
u32			VidGetFrameCount();
u32			VidGetTickCount();
int			VidGetVblsThisFrame();

void		VidSwapDraw();

void 		VidAddVSyncFunc(VbFuncType v);
void 		VidRemoveVSyncFunc(VbFuncType v);

void 		VRamViewer();

void		SetUpLoadIcon(void *fh);
void		StartLoad(int _loadX=-1,int _loadY=-1);
void		StopLoad();

void		SetScreenImage(u8 *Ptr);
u8			*GetScreenImage();
void		ClearScreenImage();

u8			*LoadPakScreen(int Filename);

#endif

/******************/
/*** GFx System ***/
/******************/

#ifndef __VID_HEADER_
#define __VID_HEADER_

#include	"system\global.h"

/*****************************************************************************/
#define	GEOM_SCREEN_H	350

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

void	VidInit();

void	VidScrOn();
void	VidSetRes(int x, int y);
void	VidSetClearScreen(int );
void	VidSetClearColor( const CVECTOR & col );

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

void		VidSwapDraw();

void 		VidAddVSyncFunc(VbFuncType v);
void 		VidRemoveVSyncFunc(VbFuncType v);

void 		VRamViewer();

void		StartLoad(int _loadX=430,int _loadY=202);
void		StopLoad();

void		SetScreenImage(u8 *Ptr);
u8			*GetScreenImage();
void		ClearScreenImage();

#endif

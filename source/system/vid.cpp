/******************/
/*** GFx System ***/
/******************/

#include "system/global.h"
#include "system\vid.h"
#include "gfx\prim.h"
#include "fileio\fileio.h"

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif

/*****************************************************************************/
#define MaxVBFuncs 	4

/*****************************************************************************/
static void (*VbFunc)(void);
static VbFuncType VbFuncList[MaxVBFuncs];

static u32			FrameCounter=0,TickCount=0,TickBuffer[2];
static u32			s_lastFrameCounter=0,s_vblsThisFrame=0;
static sVidScreen 	Screen[2];
static int			ScreenXOfs=0,ScreenYOfs=0;
static int			ScreenW, ScreenH;
static RECT 		ScreenRect;
/*static*/ int		FrameFlipFlag=0;
static int			ClearScreen=0;
static	u8			*ScreenImage=0;

static const CVECTOR s_defClearCol = {0, 0, 0};

/*****************************************************************************/
/*** Loading Icon Cack *******************************************************/
/*****************************************************************************/
POLY_FT4	LoadPoly[2];
static int	LoadX=430;
static int	LoadY=161;
static int	LoadHalfWidth;
static int	LoadIconSide;
static int	DrawLoadIcon=0;
static RECT	LoadBackRect;
static int	LoadBackY;
static int	LoadTime=0;


/*****************************************************************************/
// Altered to keep aspect ratio
s8	LoadTab[]=
{ 
	21,21,21,21,20,20,20,20,19,19,19,18,18,17,17,16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,17,18,18,19,19,19,20,20,20,20,20,21
};
const int	LoadTabSize=sizeof(LoadTab)/sizeof(s8);
/*****************************************************************************/
void	LoadingIcon()
{
int			Dst;
int			rgb;
POLY_FT4	*PolyPtr=&LoadPoly[LoadIconSide];

		Dst=LoadTab[LoadTime];
		
		PolyPtr->x0=PolyPtr->x2=LoadX-Dst+LoadHalfWidth+2;
		PolyPtr->x1=PolyPtr->x3=LoadX+Dst+LoadHalfWidth+2;

		rgb=128-(LoadTab[(LoadTime+LoadTabSize/2)%LoadTabSize]*3);
		setRGB0(PolyPtr,rgb,rgb,rgb);

		MoveImage(&LoadBackRect,LoadX,LoadY+LoadBackY);

		PutDrawEnv(&Screen[FrameFlipFlag^1].Draw);
		DrawPrim(PolyPtr);

		LoadTime++;
		if (LoadTime>=LoadTabSize)	LoadTime=0;
		if(LoadTime==LoadTabSize/2)	LoadIconSide^=1;
}

/*****************************************************************************/
void	StartLoad(int _loadX,int _loadY)
{
/*	
sFrameHdr	*fh;
SpriteBank	*spr;

		spr=UIGetSharedSpriteBank();
		LoadX=_loadX;
		LoadY=_loadY;

		setPolyFT4(&LoadPoly[0]);
		fh=spr->getFrameHeader(FRM__CD);
		setXYWH(&LoadPoly[0],LoadX,LoadY,fh->W,fh->H);
		setUVWH(&LoadPoly[0],fh->U,fh->V,fh->W,fh->H);
		LoadPoly[0].tpage=fh->TPage;
		LoadPoly[0].clut=fh->Clut;

		setPolyFT4(&LoadPoly[1]);
		fh=spr->getFrameHeader(FRM__CDFRONT);
		setXYWH(&LoadPoly[1],LoadX,LoadY,fh->W,fh->H);
		setUVWH(&LoadPoly[1],fh->U,fh->V,fh->W,fh->H);
		LoadPoly[1].tpage=fh->TPage;
		LoadPoly[1].clut=fh->Clut;

		LoadHalfWidth=fh->W/2;

		Screen[0].Draw.isbg=Screen[1].Draw.isbg=0;

		PutDrawEnv(&Screen[FrameFlipFlag^1].Draw);
		PutDispEnv(&Screen[FrameFlipFlag].Disp);

		LoadBackY=Screen[FrameFlipFlag^1].Disp.disp.y;
		setRECT(&LoadBackRect,LoadX,LoadY+(LoadBackY^256),fh->W+4,fh->H+4);

		LoadTime=0;
		DrawLoadIcon=1;
		LoadIconSide=0;
*/		
}

/*****************************************************************************/
void	StopLoad()
{

	while(LoadTime) 
		{
		VSync(0);
		}

	Screen[0].Draw.isbg=Screen[1].Draw.isbg=1;

	DrawLoadIcon=0;
}

/*****************************************************************************/
/*** VSync *******************************************************************/
/*****************************************************************************/
extern "C"
{
static void VidVSyncCallback()
{
	int i;
	FrameCounter++;
	TickCount++;
	if (DrawLoadIcon) LoadingIcon();
	if (VbFunc)
		{
		VbFunc();
		VbFunc = NULL;
		}
	for (i=0; i< MaxVBFuncs; i++) if (VbFuncList[i]) VbFuncList[i]();
}
}
/*****************************************************************************/
void VidAddVSyncFunc(VbFuncType v) 
{
	int i;
	for (i=0; i<MaxVBFuncs; i++) 
		{
		if (!VbFuncList[i]) 
			{
			VbFuncList[i] = v;
			return;
			}
		}
	ASSERT(!"Number of Vsync Funcs == MaxVBFuncs");

}

/*****************************************************************************/
void VidRemoveVSyncFunc(VbFuncType v)
{
	int i;
	for (i=0; i<MaxVBFuncs; i++) 
		{
		if (VbFuncList[i] == v) 
			{
			VbFuncList[i] = NULL;
			return;
			}
		}
	ASSERT(!"VSYNC Func Not Found");
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int			VidGetXOfs()				{return(ScreenXOfs);}
int 		VidGetYOfs()				{return(ScreenYOfs);}
void		VidSetXYOfs(int x,int y)	{ScreenXOfs = x;	ScreenYOfs = y;}
int 		VidGetScrW()				{return(ScreenW);}
int 		VidGetScrH()				{return(ScreenH);}
sVidScreen	*VidGetScreen()				{return &Screen[FrameFlipFlag];}
sVidScreen	*VidGetDispScreen()			{return (VidGetScreen());}
sVidScreen	*VidGetDrawScreen()			{return &Screen[FrameFlipFlag^1];}
u32			VidGetFrameCount()			{return(FrameCounter);}
u32			VidGetTickCount()			{return(TickBuffer[FrameFlipFlag^1]);}
int			VidGetVblsThisFrame()		{return s_vblsThisFrame;}

void		SetScreenImage(u8 *Ptr)		{ScreenImage=Ptr;}
u8			*GetScreenImage()			{return ScreenImage;}
void		ClearScreenImage()			{ScreenImage=0;}

/*****************************************************************************/
void	ClearVRam()
{
#if	defined(__VERSION_DEBUG__) && !defined(__USER_CDBUILD__)
RECT 	Rect;
//Clear All Videoram
	setRECT(&Rect,512,0,512,512);
	ClearImage(&Rect,0,0,0);

int	X;
	for (X=0;X<16;X++)
    {
u8	C0=0xff*(X&1);
	DrawSync(0); setRECT(&Rect,X*64,  0,1024/16,256); ClearImage(&Rect,C0,0,0xff-C0);
	DrawSync(0); setRECT(&Rect,X*64,256,1024/16,256); ClearImage(&Rect,0xff-C0,0,C0);
    }
#endif
}

/*****************************************************************************/
void VidScrOn()
{
	Screen[0].Draw.isbg = ClearScreen;
	Screen[1].Draw.isbg = ClearScreen;
	VSync(0);							//	wait for V-BLANK
	SetDispMask(1);						//	display on
}

/*****************************************************************************/
void VidSetDrawEnv()
{
	int	x = VidGetScrW();
	int	y = VidGetScrH();

	SetDefDrawEnv( &Screen[0].Draw, 0, 0, x, y );
	SetDefDispEnv( &Screen[0].Disp, 0, y, x, y );

	SetDefDrawEnv( &Screen[1].Draw, 0, y, x, y );
	SetDefDispEnv( &Screen[1].Disp, 0, 0, x, y );

	Screen[0].Draw.isbg = ClearScreen;
	Screen[1].Draw.isbg = ClearScreen;

	Screen[0].Draw.dtd = 1;
	Screen[1].Draw.dtd = 1;

	VidSetClearColor( s_defClearCol );

	SetDrawEnv( &Screen[0].Draw.dr_env, &Screen[0].Draw );
	SetDrawEnv( &Screen[1].Draw.dr_env, &Screen[1].Draw );
}


/*****************************************************************************/

void VidSetClearColor( const CVECTOR & col )
{
	Screen[0].Draw.r0 = Screen[1].Draw.r0 = col.r;
	Screen[0].Draw.g0 = Screen[1].Draw.g0 = col.g;
	Screen[0].Draw.b0 = Screen[1].Draw.b0 = col.b;
}

/*****************************************************************************/
void VidSetClearScreen(int Flag)
{
	ClearScreen = Flag;
	VidSetDrawEnv();
}

/*****************************************************************************/
void VidSetRes(int x, int y)
{
	ASSERT( y == 256 );

	if ((VidGetScrW() != x) || (VidGetScrH() != y))
		{
		RECT	clrRect;

		ScreenW=x;
		ScreenH=y;

		VidSetDrawEnv();
		SetGeomOffset( (x / 2), (y / 2) );
		}
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void VidInit()
{
// Wap up a loading screen
//u8	*screenData=CFileIO::loadFile(SCREENS_LOADING_GFX,"Loading Screen");
//	SetScreenImage(screenData);

//	VidSetXYOfs( ScreenXOfs, ScreenYOfs );

	SetDispMask(0);
	#if defined(__TERRITORY_USA__) || defined(__TERRITORY_JAP__)
		SetVideoMode( MODE_NTSC );
	#else
		SetVideoMode( MODE_PAL );
	#endif

	VSync(0);
	VSync(0);
	ResetGraph(0);
	SetGraphDebug(0);
	ClearVRam();
	InitGeom();
	SetGeomScreen(GEOM_SCREEN_H);

	VidSetRes( 512, 256 );
	DrawSync(0);
	VidSwapDraw();
	DrawSync(0);
	VidSwapDraw();
	DrawSync(0);
	SetScreenImage(0);
//	MemFree(screenData);
	VidScrOn();

// Init VBL
	VbFunc = NULL;
	for (int i=0; i<MaxVBFuncs; i++) VbFuncList[i] = NULL;
	VSyncCallback( VidVSyncCallback );

}

/*****************************************************************************/
int pofx=0;
int pofy=0;
#ifdef __USER_paul__
int ScreenClipBox=1;
#else
int ScreenClipBox=0;
#endif
void	VidSwapDraw()
{
DRAWENV	*Draw;
DISPENV *Disp;
int		LastFrame=FrameFlipFlag;
int		ScrH=VidGetScrH()*FrameFlipFlag;

		FrameFlipFlag^=1;
		TickBuffer[FrameFlipFlag]=TickCount; TickCount=0;
		Draw=&Screen[FrameFlipFlag].Draw;
		Disp=&Screen[FrameFlipFlag].Disp;
		Disp->disp.x=0;
		Disp->disp.y=ScrH;
		Disp->disp.w=512;
		Disp->disp.h=256;
		Disp->screen.x=ScreenXOfs;
		Disp->screen.y=ScreenYOfs;
		Disp->screen.w=256;
		Disp->screen.h=256;
		PutDispEnv(Disp); 
		PutDrawEnv(Draw);

// If set, load background screen
		if (ScreenImage) LoadImage2(&Screen[LastFrame].Disp.disp ,(u_long*)ScreenImage);


if(ScreenClipBox==1)
{
	DrawLine(15,25,512-15,25,255,0,0,0);
	DrawLine(15,256-25,512-15,256-25,255,0,0,0);
	DrawLine(15,25,15,256-25,255,0,0,0);
	DrawLine(512-15,25,512-15,256-25,255,0,0,0);

	DrawLine(0,0,511,0,0,255,0,0);
	DrawLine(0,255,511,255,0,255,0,0);
	DrawLine(0,0,0,255,0,255,0,0);
	DrawLine(511,0,511,255,0,255,0,0);
}
if(ScreenClipBox==2)
{
	POLY_F4 *f4;
	f4=GetPrimF4();
	setXYWH(f4,0,0,512,20);
	setRGB0(f4,50,50,50);
	AddPrimToList(f4,0);
	f4=GetPrimF4();
	setXYWH(f4,0,256-20,512,20);
	setRGB0(f4,50,50,50);
	AddPrimToList(f4,0);
	f4=GetPrimF4();
	setXYWH(f4,512-10,20,10,256-40);
	setRGB0(f4,50,50,50);
	AddPrimToList(f4,0);
	f4=GetPrimF4();
	setXYWH(f4,0,20,10,256-40);
	setRGB0(f4,50,50,50);
	AddPrimToList(f4,0);
}

	// How many frames since we last flipped the display?
	int	fc=FrameCounter;
	s_vblsThisFrame=fc-s_lastFrameCounter;
	s_lastFrameCounter=fc;

	// Can get 0 vbls during start up, and it breaks things
	if(s_vblsThisFrame==0)
	{
		s_vblsThisFrame=1;
	}
}


/*****************************************************************************/
/*** VRAM VIEWER *************************************************************/
/*****************************************************************************/
#define	UseVRamViewer

#ifdef		UseVRamViewer
#include	"pad\pads.H"
void 	VRamViewer()
{
bool 	Done=0;
sVidScreen	*Scr=VidGetScreen();
u16 	Pad;
int 	OldX=Scr->Disp.disp.x,OldY=Scr->Disp.disp.y;

        while(!Done)
	        {
            PadUpdate();
            DbgPollHost();

#ifdef __USER_ARTDEV__
            Pad=PadGetHeld(1);
#else
            Pad=PadGetHeld(0);
#endif
#ifdef __USER_paul__
			// my finger was hurting..
            if((PadGetDown(0) & PAD_SELECT)) Done=1;
#else
            if(!(Pad & PAD_SELECT)) Done=1;
#endif

            if(Pad&PAD_LEFT)	if(Scr->Disp.disp.x) 		Scr->Disp.disp.x--;
            if(Pad&PAD_RIGHT)	if(Scr->Disp.disp.x<700)	Scr->Disp.disp.x++;
            if(Pad&PAD_UP) 		if(Scr->Disp.disp.y) 		Scr->Disp.disp.y--;
            if(Pad&PAD_DOWN)	if(Scr->Disp.disp.y<256) 	Scr->Disp.disp.y++;
            PutDispEnv(&Scr->Disp);
            PutDrawEnv(&Scr->Draw);
	        }

        Scr->Disp.disp.x=OldX;
        Scr->Disp.disp.y=OldY;

}
#endif
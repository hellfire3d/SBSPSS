/*=========================================================================

	fmv.cpp

	Author:  	A star studded cavalcade of everyone ever lumbered with
				doing the FMV on a climax game
	Created:
	Project:	PRLSR PSX
	Purpose:	Hacked up FMV player nicked from SBK && POP && Diablo && TPW. Duff.

	Copyright (c) 1998 / 1999 / 2000 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */
#include	"system\global.h"
#include	"fileio/fileio.h"
#include	"fileio/filetab.h"
#include	"gfx/prim.h"
#include	"system/vid.h"
#include	"utils/utils.h"

/*	Module Header File
	------------------ */
#include	"fmv/fmv.h"

/*	Std Lib
	------- */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libpress.h>
#include <libcd.h>
#include <libetc.h>

/*	Other Libs
	---------- */
#include "..\..\tools\vlc\include\vlc_bit.h"


//#if	defined(__USER_paul__)
#define	FORCE_FMV	1	
//#endif

#if		__FILE_SYSTEM__==CD | FORCE_FMV
#define	ENABLE_FMV	
#endif

#if		defined(__USER_CDBUILD__)
#undef	FORCE_FMV
#endif

/*	Graphics
	-------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

#define	FMVChannelL		(1<<0)
#define	FMVChannelR		(1<<1)
#define	FMVChannelLR	(FMVChannelL|FMVChannelR)

#define FMVWidth	(320)
#define FMVHeight	(240)
#define IS_RGB24	1	/* 0:RGB16, 1:RGB24 */


#if defined(__TERRITORY_USA__)
#define	TerrOfs		(0)
#else
#define	TerrOfs		(28/2)
#endif


#if IS_RGB24==1
#define	PPW	3/2
#define	MODE	3		/* 24bit */
#else
#define	PPW	1
#define	MODE	2		/* 16bit	*/
#endif

#define RING_SIZE			32

#define	CDMidVol			127

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */


typedef struct FMVDATA
{
	int		m_filename;
	int		m_numOfFrames;
} FmvData;


/*
 *  Global movie decoding information
 */
typedef struct
	{
	u_long		*vlcbuf[2];
	int			vlcid;
	u_short		*imgbuf;
	RECT		rect[2];
	int			rectid;
	RECT		slice;
	int			isdone;
	} DECENV;

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
extern "C"
{
	void	strCallback();
};

static void SetCDVolume(int val = CDMidVol);
static void	strInit(CdlLOC *loc);
static int	strNextVlc(DECENV	*dec);
static void	strSync(DECENV *dec,int mode);
static void	strKickCD(CdlLOC *loc);
static void strSetDefDecEnv(DECENV *dec,int Height);
static u_long *strNext(DECENV *dec);

/*----------------------------------------------------------------------
	Vars
	---- */

static FmvPerFrameFunc	s_funcToCall;
static DECENV		dec;
static int			QuitFlag;
static u_short		*dcTab;
static u16			AniNoFrames,LastFrame=0;;
static u8			*PlaybackBuffer;
static u_long		*SectorBuffer;


/*----------------------------------------------------------------------
	Data
	---- */

static const FmvData s_fmvData[MAX_FMVS]=
{
	{FILEPOS_THQ_STR,		240-10	},
	{FILEPOS_INTRO_STR,		615-10	},
};


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		FMV_play(int fmvNumber,FmvPerFrameFunc func)
{
#ifdef	ENABLE_FMV
CdlLOC 			loc;
int				startSector;
DISPENV			disp;
DRAWENV			draw;
int				YOfs;
int				frame = 0;
int				id;
int				Pad=0;
	
#ifdef	FORCE_FMV
static	int	HasInit=0;
		if (!HasInit)
			{
			while (!CdInit());
			CFileIO::FindAllFilePos();
			HasInit=1;
			}
#endif
// Cos it uses prim pool, best check its big enough
	CdReadyCallback(0);	// clear it in case XA is being naughty
	ASSERT(PRIMPOOL_SIZE>FMVWidth*(FMVHeight+1));
	InitOTagR(OtPtr,MAX_OT);	// Ensure no prims need the Prim Pool
		
SYSTEM_DBGMSG("[FMV] Playing fmv %d ( fid:%d )",fmvNumber,s_fmvData[fmvNumber].m_filename);
	startSector=CFileIO::GetFilePos(s_fmvData[fmvNumber].m_filename);
SYSTEM_DBGMSG("[FMV] Start sector %d",startSector);
	s_funcToCall=func;


	PlaybackBuffer=(unsigned char*)MemAlloc(320*(FMVHeight+1),"FmvPlay");
	SectorBuffer=(u_long*)MemAlloc(RING_SIZE*2048,"FmvPlay");		// 65536
	dcTab=(u_short*)MemAlloc(sizeof(DECDCTTAB),"FmvPlay");
	DecDCTvlcBuild3(dcTab);
	DecDCTvlcSize3(320*FMVHeight);

	VSync(0);
	SetDispMask(0);
	AniNoFrames	= s_fmvData[fmvNumber].m_numOfFrames;
	LastFrame=0;
SYSTEM_DBGMSG("[FMV] Number of frames is %d",AniNoFrames);

	
	SetCDVolume();

	CdIntToPos(startSector,&loc);
	strSetDefDecEnv(&dec, FMVHeight);
	YOfs=((256-FMVHeight)+TerrOfs)/2;
	strInit(&loc);

	if (strNextVlc(&dec)!=-1)
	while(1)
		{
		DecDCTin(dec.vlcbuf[dec.vlcid], MODE);

		DecDCTout((u_long*)dec.imgbuf, dec.slice.w*dec.slice.h/2);
		if (strNextVlc(&dec)==-1) break;
		
		strSync(&dec, 0);
		VSync(0);

	   	id = dec.rectid? 0: 1;
		SetDefDispEnv(&disp, dec.rect[id].x, dec.rect[id].y,dec.rect[id].w, dec.rect[id].h);
		SetDefDrawEnv(&draw, dec.rect[id].x, dec.rect[id].y,dec.rect[id].w, dec.rect[id].h);

		disp.isrgb24 = IS_RGB24;

		#if IS_RGB24==1
			disp.disp.w = disp.disp.w*2/3;
		#else
			disp.disp.w = disp.disp.w*2/2;
		#endif

		disp.screen.x = VidGetXOfs();
		disp.screen.y = YOfs+VidGetYOfs();
		disp.screen.h = FMVHeight;
		PutDispEnv(&disp);
		PutDrawEnv(&draw);
		SetDispMask(1);

		if (s_funcToCall)
			{
			if (s_funcToCall())
				{
				SetCDVolumeOff();

				Pad=true;
				break;
				}
			}

		if(QuitFlag) break;				//do once only.
		}
	SetCDVolumeOff();
	CdControlB(CdlPause,0,0);
	VSync(0);
	DrawSync(0);
	DecDCToutCallback(0);
	CdReadyCallback(0);
	DecDCTReset(0);
	VSync(0);
	DrawSync(0);

	StUnSetRing();
	VSync(0);
	DrawSync(0);

	MemFree(dcTab);
	MemFree(SectorBuffer);
	MemFree(PlaybackBuffer);

	SetDispMask(0);

	RECT	r;
	setRECT(&r,0,0,512,512);
	ClearImage(&r,0,0,0);
	VSync(0);
	DrawSync(0);
	SetDispMask(1);
//	SetPrimCheck();

SYSTEM_DBGMSG("[FMV] Finished!");

	return Pad;
#else
	return (true);
#endif

}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void strSetDefDecEnv(DECENV *dec,int Height)
{

	static u_long*		vlcbuf0;
	static u_long*		vlcbuf1;
	static u_short*	imgbuf;
	int	x0,y0,x1,y1;

	x0=0;
	y0=0;
	x1=0;
	y1=240;

	vlcbuf0 = (u_long*) MakePtr(PrimListStart,FMVWidth*(FMVHeight+1)*0);
	vlcbuf1 = (u_long*) MakePtr(PrimListStart,FMVWidth*(FMVHeight+1)*1);
	imgbuf = (u_short*) MakePtr(PlaybackBuffer,FMVWidth*(FMVHeight+1)*0);

	dec->vlcbuf[0] = vlcbuf0;
	dec->vlcbuf[1] = vlcbuf1;
	dec->vlcid     = 0;
	dec->imgbuf    = imgbuf;
	dec->rectid    = 0;
	dec->isdone    = 0;
	setRECT(&dec->rect[0], x0, y0, FMVWidth*PPW, FMVHeight);
	setRECT(&dec->rect[1], x1, y1, FMVWidth*PPW, FMVHeight);
	setRECT(&dec->slice,   x0, y0,  16*PPW, FMVHeight);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void strInit(CdlLOC *loc)
{
	DecDCTReset(0);

	QuitFlag = 0;

	DecDCToutCallback( strCallback );

	StSetRing(SectorBuffer, RING_SIZE);

	StSetStream(IS_RGB24, 1, 0xffffffff, 0, 0);

	strKickCD(loc);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void strCallback()
{
#if IS_RGB24==1
extern	long StCdIntrFlag;
	if(StCdIntrFlag)
		{
		StCdInterrupt();
		StCdIntrFlag = 0;
		}
#endif

	LoadImage(&dec.slice, (u_long *)dec.imgbuf);

	dec.slice.x += dec.slice.w;

	if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w)
		DecDCTout((u_long*)dec.imgbuf, dec.slice.w*dec.slice.h/2);
	else
		{
		dec.isdone = 1;
		dec.rectid = dec.rectid? 0: 1;
		dec.slice.x = dec.rect[dec.rectid].x;
		dec.slice.y = dec.rect[dec.rectid].y;
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int strNextVlc(DECENV *dec)
{
	int cnt = 16;//WAIT_TIME;
	u_long	*next;

	while ((next = strNext(dec)) == 0)
		{
//		if (--cnt == 0) 
				return(-1);
		}

	dec->vlcid = dec->vlcid? 0: 1;				/* swap ID */
	DecDCTvlc3(next, dec->vlcbuf[dec->vlcid]/*,dcTab*/);	/* VLC decode */

	StFreeRing(next);									/* free used frame */
	return(0);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static u_long *strNext(DECENV *dec)
{
	RECT		rect;
	u_long		*addr;
	StHEADER	*sector;
	int			cnt = WAIT_TIME/8;
	static 	int	width  = 0;
	static 	int	height = 0;

	while(StGetNext((u_long **)&addr,(u_long **)&sector))
		{
		if (--cnt == 0) return(0);
		}

	if (sector->frameCount >= AniNoFrames) QuitFlag = 1;
	if (sector->frameCount<LastFrame) QuitFlag=1;	// Fix for nasty over-run bug
	LastFrame=sector->frameCount;
	
	if (width != sector->width || height != sector->height)
		{
		setRECT(&rect, 0, 0, FMVWidth, 480);
		ClearImage(&rect, 0, 0, 0);
		width  = sector->width;
		height = sector->height;
		}

	dec->rect[0].w = dec->rect[1].w = width*PPW;
	dec->rect[0].h = dec->rect[1].h = height;
	dec->slice.h   = height;

	return(addr);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void strSync(DECENV *dec,int mode)
{
	volatile u_long	cnt = WAIT_TIME;

	while (dec->isdone == 0)
		{
		if (--cnt == 0)
			{
			SYSTEM_DBGMSG("time out in decoding");
			dec->isdone = 1;
			dec->rectid = dec->rectid? 0: 1;
			dec->slice.x = dec->rect[dec->rectid].x;
			dec->slice.y = dec->rect[dec->rectid].y;
			}
		}
	dec->isdone = 0;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void strKickCD(CdlLOC *loc)
{
	while (CdControl(CdlSeekL, (u_char *)loc, 0) == 0);
	while(CdRead2(CdlModeStream|CdlModeSpeed|CdlModeRT) == 0);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SetCDVolumeOff()
{
//	SetCDVolume(0);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SetCDVolumeOn()
{
	SetCDVolume();
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SetCDVolume(int val)
{
CdlATV	Vol;
int		VolL,VolR;
int		Channel=FMVChannelLR;
SpuCommonAttr	Attr;

		VolL=val;
		VolR=val;

		Vol.val0 = VolL;		// CdL -> SpuL
		Vol.val1 = VolL;		// CdL -> SpuR
		Vol.val2 = VolR;		// CdR -> SpuR
		Vol.val3 = VolR;		// CdR -> SpuL
		CdMix(&Vol);

		Attr.mask = (SPU_COMMON_CDVOLL|SPU_COMMON_CDVOLR|SPU_COMMON_CDMIX); 
		Attr.cd.volume.left	=32000;
		Attr.cd.volume.right=32000;
		Attr.cd.mix=SPU_ON;
		SpuSetCommonAttr(&Attr);
}

/*===========================================================================
 end */

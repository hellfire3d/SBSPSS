#include 	<sys\types.h>
#include 	<libapi.h>
#include 	<libetc.h>
#include 	<libgte.h>
#include 	<libgpu.h>
#include	<libcd.h>
#include	<kernel.h>
#include	<libspu.h>
#include	<memory.h>
#include	<libsnd.h>
#include	<stdio.h>

#include 	"psxboot\PsxBoot.H"
#include	"fileio\filetab.h"
#include	"fileio\filetab.cpp"

#define	PiracyFilename	"\\PIRACY.GFX;1"
#define	LegalFilename	"\\LEGAL.GFX;1"
#define	ScreenDelay		50*(5-2)
#define	ExeFilename		"\\PRLSR.BIN;1"

#define CHUNKSIZE	2048

#define	PrgAddr		0x80010000
#define SCRATCH_RAM 0x1f800000

//#define	DBGMsg	printf
#define	DBGMsg

/*******************************************************************************/
DB		*CurDBuf,DBuf[2];

/*****************************************************************************/
static volatile int cdread_status=0;
static u_char current_cd_mode;
static int cd_speed=-1;//,buf_sec=-1;
static void cdread_callback(u_char intr, u_char *result)
{
	if (intr==CdlComplete)
		{
		cdread_status=0;
		} 
	else
	if (intr==CdlDiskError)
		{
		cdread_status=-1;
		} 
	else
		{
		cdread_status=-2;
		}
}

/*****************************************************************************/
static void set_cd_speed(int speed)
{
	if (speed!=cd_speed)
		{
		current_cd_mode=speed?CdlModeSpeed:0;

		do	{	// Yuk!
			while (CdControl(CdlSetmode,&current_cd_mode,0)==0);
			} while (CdSync(0,NULL)!=CdlComplete);

		VSync(5);

		cd_speed=speed;
	}
}

/*******************************************************************************/
void LoadFile(char *Filename,char *Dst,long Length)
{
	CdlFILE	RetFile;
	int Len;
	
	DBGMsg("Load %s\n",Filename);

	while ( !CdSearchFile(&RetFile,(char *)Filename) )	DBGMsg("Not Found\n");

	if(!Length)
		Len=(RetFile.size);
	else
		Len=Length;

unsigned char	mode=CdlModeSpeed;

	DBGMsg("Found\n");

	while (1)
		{
		set_cd_speed(1);
		CdReadCallback(cdread_callback);

		while ( (CdControlB(CdlSetloc,(u_char *)&RetFile.pos,NULL)==0) );

		const unsigned long NumSectors=(unsigned long) (Len / CHUNKSIZE)+1;

		cdread_status=1;
		int OK=CdRead(NumSectors,(u_long *)Dst,current_cd_mode);

		if (OK)
			{
			int C=0;
			while(cdread_status>0)
				{
				C++;
				}

			if (cdread_status==0) break;
			}

		}

	DrawSync(0);

	CdReadCallback(0);

}

/*****************************************************************************/
/*** System Stuff ************************************************************/
/*****************************************************************************/
void 	InitSys(void)
		{
int 	Count;
		ResetCallback();
		ResetGraph(0);
		SetGraphDebug(0);
		while (!CdInit());
//		InitGeom();
		CdSetDebug(0);
//		SpuInit();
		SsInit();

#if		defined(__TERRITORY_USA__)
		SetVideoMode(MODE_NTSC);
#else
		SetVideoMode(MODE_PAL);
#endif

		SetDefDrawEnv(&DBuf[0].draw, 0,0, FRAME_X, FRAME_Y);
		SetDefDispEnv(&DBuf[0].disp, 0,0, FRAME_X, FRAME_Y);
		SetDefDrawEnv(&DBuf[1].draw, 0,0, FRAME_X, FRAME_Y);
		SetDefDispEnv(&DBuf[1].disp, 0,0, FRAME_X, FRAME_Y);

		setRECT(&DBuf[0].disp.screen, SCREEN_X, SCREEN_Y, 0, FRAME_Y);
		setRECT(&DBuf[1].disp.screen, SCREEN_X, SCREEN_Y, 0, FRAME_Y);
		DBuf[0].disp.isrgb24=DBuf[1].disp.isrgb24=0;
		DBuf[0].draw.isbg = DBuf[1].draw.isbg = 1;
		CurDBuf=DBuf;

#if		!defined(__TERRITORY_USA__)
		DBuf[0].disp.screen.y=24;
		DBuf[1].disp.screen.y=24;
#endif

// Stop flicker!!
		Count=2;
		while(Count--)
			{
			PutDrawEnv(&CurDBuf->draw);
			PutDispEnv(&CurDBuf->disp);
			DrawSync(0);
			}
		DBuf[0].draw.isbg = DBuf[1].draw.isbg = 0;

		SetDispMask(1);

		}

/*****************************************************************************/

void ShowScreen(char *Filename,int Delay,int FadeOutOnly)
{
u_long		Count;
char		Fade;
RECT		PicRect;
POLY_FT4	fp,*Poly=&fp;
int			*Ptr;
// Set up shade polys
//	SetPolyFT4(Poly);
//	setXYWH(Poly,0,0,SCR_WIDTH,SCR_HEIGHT);
//	Poly->tpage=GetTPage(2,0,512,0);

//	if (!FadeOutOnly)
	{
		setRECT(&PicRect,0,0,1024,512);
		ClearImage(&PicRect,0,0,0);


// load Screen
		LoadFile(Filename,(char*)LoadBuffer,FRAME_X * FRAME_Y*2);
		setRECT(&PicRect,0,0,FRAME_X,FRAME_Y);
		LoadImage(&PicRect,(u_long*)LoadBuffer);

// Fade in
		Fade=0;
//		while(Fade<128)
//		{
//			setRGB0(Poly,Fade,Fade,Fade);
//			Fade+=2;
//			DrawPrim(Poly);
//			DrawSync(0);
//			VSync(0);
//		}

// Wait a while
//		if (Delay<0) return;
//		Count=Delay;
//		while(Count--)
//		{
//			DrawSync(0);
//		}
	}
// Fade Out

//	Fade=128;
//	while(Fade>0)
//	{
//		setRGB0(&Poly[0],Fade,Fade,Fade);
//		Fade-=2;
//		DrawPrim(&Poly[0]);
//		DrawPrim(&Poly[1]);
//		DrawSync(0);
//	}
//
	DrawSync(0);
}

/*****************************************************************************/
void	(*Func)()=(void (*)())0x80010000;
int		main()
{
		InitSys();

		ShowScreen(LegalFilename,-1,0);
		CalcFilePos((int*)SCRATCH_RAM);
int		*Ptr=(int*)SCRATCH_RAM;
//		for (int Loop=0;Loop<FILEPOS_MAX;Loop++)	printf("%i\n",*Ptr++);
		while (1)
			{
			int count = VSync( -1 );

			LoadFile(ExeFilename,(char *)PrgAddr,0);

			while ((VSync( -1 ) - count) < ScreenDelay);

			RECT clearRect;
			setRECT(&clearRect,0,0,1024,512);
//			ClearImage(&clearRect,0,0,0);
//			DrawSync(0);
//			SetDispMask(0);
			EnterCriticalSection();
			FlushCache();
			ExitCriticalSection();
			StopCallback();
			Func();
	        (((void(**)(void))PrgAddr)[0])();
			}

		return(0);
}

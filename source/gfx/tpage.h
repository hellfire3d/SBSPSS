/**********************/
/*** PSX Vram Stuff ***/
/**********************/

#ifndef __VRAM_H__
#define __VRAM_H__

#include	<BigLump.h>

/*************************************************************************************************/
#define	TPAGE_MAX_ANIM_TEX	8

typedef struct
	{
	u16	NoOfFrames,NoOfTPages;
	u16	TPageStart;
	u16	TPageWidth,TPageHeightInPixels;
	u16	NumOfSpareBoxes;
	u16	Compress,Pad;
	} sTPageHdr;

typedef struct
	{
	u16	TPage;
	u16	Clut;
	s8	XOfs,YOfs;

	u8	W,H;
	u8	U,V;
	u8	Rotated;
	u8	Cycle;
	} sFrameHdr;

struct TPAGE_DESC
{
	u8	Half;
	u8	tpage;
	u8	xoffset;
	u8	yoffset;
};

struct	sTPageInfo
{
	s16			RefCount;
	FileEquate	TPageName;
	s16			XOfs,YOfs;
	u16			AnimTexCount;
	sFrameHdr	AnimTexFrame[TPAGE_MAX_ANIM_TEX];
};

struct	sTPageCache
{
	sTPageInfo	Info[2];
};

/********************************************************************************************************/

void		TPInit();
TPAGE_DESC	TPLoadTex(FileEquate Filename);
TPAGE_DESC	TPLoadTexWithHeaders( FileEquate Filename, sFrameHdr **hdrs );
void		TPFree( const TPAGE_DESC & desc ,int blah=0);

#endif
/**********************/
/*** PSX Vram Stuff ***/
/**********************/

#ifndef		__TPAGE_H__
#define		__TPAGE_H__

#ifndef		__FILE_EQUATES_H__
#include	<BigLump.h>
#endif

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

/********************************************************************************************************/

void		TPInit();
TPAGE_DESC	TPLoadTex(FileEquate Filename);
TPAGE_DESC	TPLoadTexWithHeaders( FileEquate Filename, sFrameHdr **hdrs );
void		TPFree( const TPAGE_DESC & desc ,int blah=0);

#endif
/**************************/
/*** Anim Texture Stuph ***/
/**************************/

#ifndef _ANIMTEX_HEADER_
#define _ANIMTEX_HEADER_

#include "gfx\tpage.h"

/*****************************************************************************/
class CAnimTex
{
public:
		CAnimTex()
			{
			NextTex=0;
			}
static	void	AddAnimTex(sFrameHdr *Frame,FileEquate Filename);
static	void	AnimateTex();
static	void	GetTexData();
//static	void	DumpThisTPage(int _tpage);
static	void	DumpThisTPage(FileEquate TexName);
static	void	DumpAll();

private:
	CAnimTex	*NextTex;
	RECT		Rect;
	s16			Speed,Count;
	s16			PixPerWord;
	u32			*TexData;
	FileEquate	TexName;
	u8			TPage;

};
/*
typedef struct
	{
	u16	TPage;
	u16	Clut;
	s8	XOfs;
	s8	YOfs;

	s8	W;
	s8	H;
	u8	U;
	u8	V;
	u8	Rotated;
	u8	Cycle;
	} sFrameHdr;


*/



/*****************************************************************************/

#endif
/**************************/
/*** Anim Texture Stuph ***/
/**************************/

#ifndef _ANIMTEX_HEADER_
#define _ANIMTEX_HEADER_

#include "gfx\tpage.h"
#include <DStructs.h>

/*****************************************************************************/
class CAnimTex
{
public:
		CAnimTex();
		~CAnimTex();
		
static	void	AddAnimTex(sFrameHdr *Frame,FileEquate Filename);
static	void	AnimateTex();
static	void	GetTexData();
static	void	DumpThisTPage(FileEquate TexName);

private:
		CAnimTex	*NextTex;
		RECT		Rect;
		s16			Speed,Count;
		s16			PixPerWord;
		u32			*TexData;
		FileEquate	TexName;
		u8			TPage;

};

/*****************************************************************************/

class CPakTex
{
public:
		enum
		{
			PAKTEX_MAX=16,
		};

		CPakTex();

static	void	Init(int MaxSize);
static	void	Shutdown();
static	void	Add(u8 *PakSpr,RECT *DstRect);
static	void	DMAPakTex();
protected:
		u8		*PakSpr;
		RECT	*DstRect;
private:
static	CPakTex	PakTexList[];
static	int		PakTexCount;
static	u8		*UnpackBuffer;
};

/*****************************************************************************/

#endif
/**************************/
/*** Anim Texture Stuph ***/
/**************************/


#include "system\global.h"
#include "mem\memory.h"
#include "fileio\fileio.h"
#include "gfx\animtex.h"
#include "gfx\tpage.h"
#include "utils\utils.h"

#ifndef __SYSTEM_GSTATE_H__
#include 	"system\gstate.h"
#endif

CAnimTex	*AnimTexList=0;

/*****************************************************************************/
CAnimTex::CAnimTex()
{
		NextTex=0;
}

/*****************************************************************************/
CAnimTex::~CAnimTex()
{
	if (TexData) MemFree(TexData);
}

/*****************************************************************************/
void	CAnimTex::AddAnimTex(sFrameHdr *Frame,FileEquate Filename)
{
int			TPageX,TPageY,X,Y,W,H;
CAnimTex	*ThisTex=new ("CAnimTex::AddAnimTex") CAnimTex;

		ThisTex->NextTex=AnimTexList;
		AnimTexList=ThisTex;

		X=(u8)Frame->U;
		Y=(u8)Frame->V;
		W=(u8)Frame->W;
		H=(u8)Frame->H;
		
		switch (((Frame->TPage)>>7)&0x003)
			{
			case 0:
				ThisTex->PixPerWord=4;
				break;
			case 1:
				ThisTex->PixPerWord=2;
				break;
			case 2:
				ThisTex->PixPerWord=1;
				break;
			default:
				ASSERT(!"Unknown Pixel Depth");
				break;
			};

		X/=ThisTex->PixPerWord;
		W/=ThisTex->PixPerWord;

		TPageX=(Frame->TPage<<6)&0x7c0;
		TPageY=(Frame->TPage<<4)&0x100;
		TPageX+=X;
		TPageY+=Y;

		ThisTex->Rect.x=TPageX;
		ThisTex->Rect.y=TPageY;
		ThisTex->Rect.w=W;
		ThisTex->Rect.h=H;
		ThisTex->TexData=0;
		ThisTex->Count=0; ThisTex->Speed=-1;
		ThisTex->TPage=Frame->TPage;
		ThisTex->TexName=Filename;

		int	Size=W*H;
		ThisTex->TexData=(u32*)MemAlloc(Size*sizeof(u16), "AnTx");
		DrawSync(0);
		StoreImage(&ThisTex->Rect,(u32*)ThisTex->TexData);

}

/*****************************************************************************/

void	CAnimTex::DumpThisTPage(FileEquate TexName)
{
CAnimTex	*PrevTex, *ThisTex, *NextTex;

	PrevTex=NULL;
	ThisTex=AnimTexList;

	while(ThisTex)
		{
		NextTex=ThisTex->NextTex;
		if(ThisTex->TexName==TexName)
			{
			if (PrevTex) PrevTex->NextTex=NextTex;
			if (ThisTex==AnimTexList) AnimTexList=NextTex;
			delete ThisTex;
			}
		else
			{
			PrevTex=ThisTex;
			}
		ThisTex=NextTex;
		}
}

/*****************************************************************************/


void	CAnimTex::AnimateTex()
{
CAnimTex	*ThisTex=AnimTexList;
RECT		Rect;
int			Count,CountComp;
int			H,W;
int			Time = GameState::getTimeSinceLast();

		while (ThisTex)
			{
			Rect=ThisTex->Rect;			
			H=Rect.h;

			Count=((ThisTex->Count>>2)+ThisTex->Rect.h)%ThisTex->Rect.h;	// Allow for backwards
			CountComp=H-Count;

// Top Chunk
			if (Count)
				{
				int Offset = (CountComp*Rect.w)>>1;
				Rect.h=Count;
				LoadImage(&Rect,ThisTex->TexData+Offset);
				}
// Bottom Chunk
			if (CountComp)
				{
				Rect.y+=Count;
				Rect.h=CountComp;
				LoadImage(&Rect,ThisTex->TexData);
				}
			ThisTex->Count+=(ThisTex->Speed * Time)>> 12;
			ThisTex->Count%=(ThisTex->Rect.h<<2);
			ThisTex=ThisTex->NextTex;
			}

}

/*****************************************************************************/


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

// If this function changes then DumpThisTPage() will probly need to change too  (PKG)

void	CAnimTex::AddAnimTex(sFrameHdr *Frame,FileEquate Filename)
{
int			TPageX,TPageY,X,Y,W,H;
CAnimTex	*ThisTex=new ("CAnimTex::AddAnimTex") CAnimTex;

		if(AnimTexList)
			{
			ThisTex->NextTex=AnimTexList;
			}
		AnimTexList=ThisTex;

		X=(u8)Frame->U;
		Y=(u8)Frame->V;
		W=(u8)Frame->W;
		H=(u8)Frame->H;
		
		ThisTex->PixPerWord=0;
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
/*
void		CAnimTex::GetTexData()
{
CAnimTex	*ThisTex=AnimTexList;

		while (ThisTex)
			{
			if (!ThisTex->TexData)
				{
				RECT	Rect=ThisTex->Rect;
				ThisTex->TexData=(u32*)MemAlloc((Rect.w*Rect.h)*sizeof(u16), "AnTx");
				DrawSync(0);
				StoreImage(&Rect,(u32*)ThisTex->TexData);
				}
			ThisTex=ThisTex->NextTex;
			}
}
*/
/*****************************************************************************/

void	CAnimTex::DumpThisTPage(FileEquate TexName)
{
	CAnimTex	*prevTex, *thisTex,*nextTex;

	prevTex=NULL;
	thisTex=AnimTexList;

	while(thisTex)
	{
		nextTex=thisTex->NextTex;
		if(thisTex->TexName==TexName)
		{
			if(prevTex)
				prevTex->NextTex=nextTex;
			if(thisTex->TexData)
				MemFree(thisTex->TexData);
			if(thisTex==AnimTexList)
				AnimTexList=nextTex;
			delete thisTex;
		}
		else
		{
			prevTex=thisTex;
		}
		thisTex=nextTex;
	}
//	CAnimTex::DumpAll();
}

/*****************************************************************************/
void	CAnimTex::DumpAll()
{
	ASSERT(!"DONT USE CAnimTex::DumpAll()");	
}
/*
void	CAnimTex::DumpAll()
{
	CAnimTex	*thisTex,*nextTex;

	thisTex=AnimTexList;
	while(thisTex)
	{
		nextTex=thisTex->NextTex;
		if(thisTex->TexData)
			MemFree(thisTex->TexData);
		delete thisTex;
		thisTex=nextTex;
	}
	AnimTexList=NULL;
}

*/
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


/******************/
/*** Actor Bank ***/
/******************/

#include "system\global.h"
#include "mem\memory.h"
#include "fileio\fileio.h"
#include "gfx\actor.h"
#include "utils\utils.h"
#include "utils\pak.h"
#include "gfx\prim.h"

#include <dstructs.h>

/*****************************************************************************/
CActorGfx	*CActorPool::ActorList[CActorPool::MAX_ACTORS];

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CActorGfx::Init(FileEquate _Filename)
{
		CActorPool::GetActor(Filename);
}

/*****************************************************************************/
CActorGfx::CActorGfx(FileEquate _Filename)
{
int		i;
		Filename=_Filename;
		SpriteBank=(sSpriteAnimBank*)CFileIO::loadFile(Filename);

		SpriteBank->AnimList=(sSpriteAnim*)		MakePtr(SpriteBank,(int)SpriteBank->AnimList);
		SpriteBank->FrameList=(sSpriteFrame*)	MakePtr(SpriteBank,(int)SpriteBank->FrameList);
		SpriteBank->Palette=(u8*)				MakePtr(SpriteBank,(int)SpriteBank->Palette);

// FixUp AnimList
		for (i=0; i<SpriteBank->AnimCount; i++)
		{
			sSpriteAnim	*ThisAnim=&SpriteBank->AnimList[i];
			ThisAnim->Anim=(u16*)	MakePtr(SpriteBank,(int)ThisAnim->Anim);
		}
// FixUp FrameList
		for (i=0; i<SpriteBank->FrameCount; i++)
		{
			sSpriteFrame	*ThisFrame=&SpriteBank->FrameList[i];
			ThisFrame->PAKSpr=(u8*)				MakePtr(SpriteBank,(int)ThisFrame->PAKSpr);
		}

		DAVE_DBGMSG("Anims %i\n",SpriteBank->AnimCount);
}

/*****************************************************************************/
CActorGfx::~CActorGfx()
{
		MemFree(SpriteBank);
}

/*****************************************************************************/
int	TPP=0;
int	TPA=0;
int	TPX=512;
int	TPY=256;
int	RR=128;
int	GG=128;
int	BB=128;
int	XX=32;
int	YY=32;
//int	PW=16;
void	CActorGfx::Render(DVECTOR &Pos,int Anim,int Frame,bool FlipX)
{
sSpriteAnim		&ThisAnim=SpriteBank->AnimList[Anim];
u16				FrameNo=ThisAnim.Anim[Frame];
sSpriteFrame	&ThisFrame=SpriteBank->FrameList[FrameNo];
POLY_FT4		*Ft4;
u8	Buffer[256*256];

			PAK_doUnpak(Buffer,ThisFrame.PAKSpr);
// clut
RECT		Rect;
			Rect.x=512;
			Rect.y=511;
			Rect.w=SpriteBank->ColorCount;
			Rect.h=1;
			LoadImage( &Rect, (u32*)SpriteBank->Palette);
		
// Gfx
			Rect.x=512;
			Rect.y=256;
			Rect.w=ThisFrame.W/4;
			Rect.h=ThisFrame.H;
			LoadImage( &Rect, (u32*)Buffer);

			Ft4=GetPrimFT4();
			setXYWH(Ft4,Pos.vx-ThisFrame.XOfs,Pos.vy-ThisFrame.YOfs,ThisFrame.W,ThisFrame.H);
			setUVWH(Ft4,0,0,ThisFrame.W,ThisFrame.H);
			setRGB0(Ft4,RR,GG,BB);
			setTPage(Ft4,0,0,TPX,TPY);
			setClut(Ft4, TPX, 511);
			AddPrimToList(Ft4,0);

			Ft4=GetPrimFT4();
			setXYWH(Ft4,Pos.vx-ThisFrame.XOfs,Pos.vy-ThisFrame.YOfs,ThisFrame.W,ThisFrame.H);
			Ft4->x0-=XX;
			Ft4->x1-=XX;
			Ft4->y0+=YY;
			Ft4->y1+=YY;

			setUVWH(Ft4,0,0,ThisFrame.W,ThisFrame.H);
			setRGB0(Ft4,0,0,0);
			setSemiTrans(Ft4,1);
			setTPage(Ft4,0,0,TPX,TPY);
			setClut(Ft4, TPX, 511);
			AddPrimToList(Ft4,0);

}

/*****************************************************************************/
void	CActorGfx::Dump()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CActorPool::Init()
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			ActorList[i]=0;
		}
}

/*****************************************************************************/
int		CActorPool::FindIdx(FileEquate Filename)
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			if (ActorList[i] && ActorList[i]->GetFilename()==Filename) return(i);
		}
		return(-1);
}

/*****************************************************************************/
int		CActorPool::FindFreeIdx()
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			if (!ActorList[i]) return(i);
		}
		return(-1);
}

/*****************************************************************************/
/*** Load ********************************************************************/
/*****************************************************************************/
CActorGfx	*CActorPool::GetActor(FileEquate Filename)
{
CActorGfx	*NewActor;
int			Idx;
// Already Loaded?
		Idx=FindIdx(Filename);
		if (Idx!=-1) return(ActorList[Idx]);

// Create and Load
		Idx=FindFreeIdx();
		ASSERT(Idx!=-1);

		NewActor=new ("ActorPool") CActorGfx(Filename);
		ActorList[Idx]=NewActor;

		return(NewActor);
}

/*****************************************************************************/
/*** Dump ********************************************************************/
/*****************************************************************************/
void	CActorPool::DumpActor(FileEquate Filename)
{
CActorGfx	*ThisActor;
int			Idx;

// Is Loaded?
		Idx=FindIdx(Filename);
		ASSERT(Idx!=-1); 
		ThisActor=ActorList[Idx];
		ActorList[Idx]=0;
		delete ThisActor;
}


/******************/
/*** Actor Bank ***/
/******************/

#include "system\global.h"
#include "mem\memory.h"
#include "fileio\fileio.h"
#include "utils\utils.h"
#include "utils\pak.h"
#include "gfx\prim.h"
#include "gfx\actor.h"

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
int		TPP=0;
int		TPA=0;
int		TPX=512;
int		TPY=256;
int		ShadowXOfs=32;
int		ShadowYOfs=32;

POLY_FT4	*CActorGfx::Render(DVECTOR &Pos,int Anim,int Frame,bool XFlip,bool YFlip,bool Shadow)
{
sSpriteAnim		&ThisAnim=SpriteBank->AnimList[Anim];
u16				FrameNo=ThisAnim.Anim[Frame];
sSpriteFrame	&ThisFrame=SpriteBank->FrameList[FrameNo];
u8				Buffer[64*64];

			PAK_doUnpak(Buffer,ThisFrame.PAKSpr);
// clut
RECT		Rect;
			Rect.x=512;
			Rect.y=TPY;
			Rect.w=SpriteBank->ColorCount;
			Rect.h=1;
			LoadImage( &Rect, (u32*)SpriteBank->Palette);
		
// Gfx
			Rect.x=512;
			Rect.y=TPY+4;
			Rect.w=ThisFrame.W/4;
			Rect.h=ThisFrame.H;
			LoadImage( &Rect, (u32*)Buffer);

POLY_FT4	*Ft4=GetPrimFT4();
			SetUpFT4(Ft4,&ThisFrame,Pos.vx,Pos.vy,XFlip,YFlip);
			setRGB0(Ft4,128,128,128);
			setTPage(Ft4,0,0,TPX,TPY+4);
			setClut(Ft4, TPX, TPY);
			AddPrimToList(Ft4,0);

			if (Shadow)
			{
				POLY_FT4	*sFt4=GetPrimFT4();
				*sFt4=*Ft4;
				sFt4->x0-=ShadowXOfs;
				sFt4->x1-=ShadowXOfs;
				sFt4->y0+=ShadowYOfs;
				sFt4->y1+=ShadowYOfs;
				setSemiTrans(sFt4,1);
				setRGB0(sFt4,0,0,0);
				AddPrimToList(sFt4,0);
			}

			return(Ft4);
}

/*****************************************************************************/
void	CActorGfx::SetUpFT4(POLY_FT4 *Ft4,sSpriteFrame *ThisFrame,int X,int Y,bool XFlip,bool YFlip)
{
int		U=0;
int		V=4;
int		W=ThisFrame->W;
int		H=ThisFrame->H;

		if (XFlip)
		{
			Ft4->u0=U+W-1;
			Ft4->u1=U;
			Ft4->u2=U+W-1;
	 		Ft4->u3=U;
		}
		else
		{
			Ft4->u0=U;
			Ft4->u1=U+W;
			Ft4->u2=U;
 			Ft4->u3=U+W;
		}

		if (YFlip)
		{
			Ft4->v0=V+H-1;
			Ft4->v1=V+H-1;
			Ft4->v2=V;
			Ft4->v3=V;
		}
		else
		{
			Ft4->v0=V;
			Ft4->v1=V;
			Ft4->v2=V+H;
			Ft4->v3=V+H;
		}

		X+=ThisFrame->XOfs;
		Y+=ThisFrame->YOfs;

		setXYWH(Ft4,X,Y,W,H);
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


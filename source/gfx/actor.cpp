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
u8			CActorPool::UnpackBuffer[CActorPool::MAX_ACTOR_SIZE];

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CActorGfx::Init(FileEquate _Filename)
{
		CActorPool::GetActor(Filename);
}

int		DefTPX=512;
int		DefTPY=256;
int		ShadowXOfs=32;
int		ShadowYOfs=32;
int		TPInc=64;

/*****************************************************************************/
CActorGfx::CActorGfx(FileEquate _Filename,int Idx)
{
int		i;

		Filename=_Filename;
		ActorNo=Idx;
		SpriteBank=(sSpriteAnimBank*)CFileIO::loadFile(Filename);

		SpriteBank->AnimList=(sSpriteAnim*)		MakePtr(SpriteBank,(int)SpriteBank->AnimList);
		SpriteBank->FrameList=(sSpriteFrame*)	MakePtr(SpriteBank,(int)SpriteBank->FrameList);
		SpriteBank->Palette=(u8*)				MakePtr(SpriteBank,(int)SpriteBank->Palette);

// FixUp AnimList
		DAVE_DBGMSG("Anims %i\n",SpriteBank->AnimCount);
		for (i=0; i<SpriteBank->AnimCount; i++)
		{
			sSpriteAnim	*ThisAnim=&SpriteBank->AnimList[i];
			ThisAnim->Anim=(u16*)	MakePtr(SpriteBank,(int)ThisAnim->Anim);
		}
// FixUp FrameList
		DAVE_DBGMSG("Anims %i\n",SpriteBank->FrameCount);
		for (i=0; i<SpriteBank->FrameCount; i++)
		{
			sSpriteFrame	*ThisFrame=&SpriteBank->FrameList[i];
			ThisFrame->PAKSpr=(u8*)				MakePtr(SpriteBank,(int)ThisFrame->PAKSpr);
		}

		TexX=DefTPX+(TPInc*ActorNo);
		TexY=DefTPY+4;
		ClutX=TexX;
		ClutY=DefTPY;

// upload clut
RECT		Rect;
			Rect.x=ClutX;
			Rect.y=ClutY;
			Rect.w=SpriteBank->ColorCount;
			Rect.h=1;
			LoadImage( &Rect, (u32*)SpriteBank->Palette);
}

/*****************************************************************************/
CActorGfx::~CActorGfx()
{
		Dump();
}

/*****************************************************************************/
sSpriteFrame	*CActorGfx::GetFrame(int Anim,int Frame)
{
sSpriteAnim		*ThisAnim=SpriteBank->AnimList+Anim;
u16				ThisFrame=ThisAnim->Anim[Frame];
	
				return(SpriteBank->FrameList+ThisFrame);
}

/*****************************************************************************/
POLY_FT4	*CActorGfx::Render(DVECTOR &Pos,int Anim,int Frame,bool XFlip,bool YFlip,bool Shadow)
{
sSpriteFrame	*FrameGfx=GetFrame(Anim,Frame);
		
			PAK_doUnpak(CActorPool::UnpackBuffer,FrameGfx->PAKSpr);
// Gfx
RECT		Rect;
			Rect.x=TexX;
			Rect.y=TexY;
			Rect.w=FrameGfx->W/4;
			Rect.h=FrameGfx->H;

			LoadImage( &Rect, (u32*)CActorPool::UnpackBuffer);

POLY_FT4	*Ft4=GetPrimFT4();
			SetUpFT4(Ft4,FrameGfx,Pos.vx,Pos.vy,XFlip,YFlip);
			setRGB0(Ft4,128,128,128);
			setTPage(Ft4,0,0,TexX,TexY);
			setClut(Ft4, ClutX, ClutY);
			AddPrimToList(Ft4,10);

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
				AddPrimToList(sFt4,10);
			}

			return(Ft4);
}

/*****************************************************************************/
void	CActorGfx::SetUpFT4(POLY_FT4 *Ft4,sSpriteFrame *Frame,int X,int Y,bool XFlip,bool YFlip)
{
int		U=0;
int		V=4;
int		W=Frame->W;
int		H=Frame->H;

		if (XFlip)
			{
			X-=Frame->XOfs;
			X-=W;
			Ft4->u0=U+W-1;
			Ft4->u1=U;//-1;
			Ft4->u2=U+W-1;
	 		Ft4->u3=U;//-1;

			}
		else
		{
			X+=Frame->XOfs;
			Ft4->u0=U;
			Ft4->u1=U+W;
			Ft4->u2=U;
 			Ft4->u3=U+W;

		}

		if (YFlip)
			{
			Y-=Frame->YOfs;
			Y-=H;
			Ft4->v0=V+H-1;
			Ft4->v1=V+H-1;
			Ft4->v2=V;//-1;
			Ft4->v3=V;//-1;
			}
		else
		{
			Y+=Frame->YOfs;
			Ft4->v0=V;
			Ft4->v1=V;
			Ft4->v2=V+H;
			Ft4->v3=V+H;
		}

		setXYWH(Ft4,X,Y,W,H);
}

/*****************************************************************************/
void	CActorGfx::Dump()
{
		MemFree(SpriteBank);
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

		NewActor=new ("ActorPool") CActorGfx(Filename,Idx);
		ActorList[Idx]=NewActor;

		return(NewActor);
}

/*****************************************************************************/
/*** Dump ********************************************************************/
/*****************************************************************************/
// Dumps all apart from spongeybob
void	CActorPool::DumpActors()
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			if (ActorList[i] && ActorList[i]->GetFilename()!=ACTORS_SPONGEBOB_SBK)
			{
				CActorGfx	*ThisActor=ActorList[i];
				delete ThisActor;
				ActorList[i]=0;
			}
		}
}

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
int		ShadowXOfs=32;
int		ShadowYOfs=32;

/*****************************************************************************/

CActorPool::ACTOR_TYPE CActorPool::actorType[38] =
{
	ACTOR_PLAYER,		//SpongeBob=0
	ACTOR_FRIEND_NPC,	//BarnacleBoy=1
	ACTOR_FRIEND_NPC,	//Gary=2
	ACTOR_FRIEND_NPC,	//Krusty=3
	ACTOR_FRIEND_NPC,	//MermaidMan=4
	ACTOR_FRIEND_NPC,	//Patrick=5
	ACTOR_FRIEND_NPC,	//Sandy=6
	ACTOR_FRIEND_NPC,	//Squidward=7
	ACTOR_FRIEND_NPC,	//Plankton=8
	ACTOR_UNKNOWN,
	ACTOR_ENEMY_NPC,	//SmallJellyfish-Level1=10
	ACTOR_ENEMY_NPC,	//SmallJellyfish-Level2=11
	ACTOR_ENEMY_NPC,	//Motherjellyfish=12
	ACTOR_ENEMY_NPC,	//Anenome-Level1=13
	ACTOR_ENEMY_NPC,	//SpikeyAnenome=14
	ACTOR_ENEMY_NPC,	//Anenome-Level3=15
	ACTOR_ENEMY_NPC,	//BabyOctopus=16
	ACTOR_ENEMY_NPC,	//Ballblob=17
	ACTOR_ENEMY_NPC,	//Boogermonster=18
	ACTOR_ENEMY_NPC,	//Caterpillar=19
	ACTOR_ENEMY_NPC,	//Clam-Level1=20
	ACTOR_ENEMY_NPC,	//Clam-Level2=21
	ACTOR_ENEMY_NPC,	//Eyeball=22
	ACTOR_ENEMY_NPC,	//Flamingskull=23
	ACTOR_ENEMY_NPC,	//FlyingDutchman=24
	ACTOR_ENEMY_NPC,	//Ghost=25
	ACTOR_ENEMY_NPC,	//GiantWorm=26
	ACTOR_ENEMY_NPC,	//HermitCrab=27
	ACTOR_ENEMY_NPC,	//IronDogFish=28
	ACTOR_ENEMY_NPC,	//PuffaFish=29
	ACTOR_ENEMY_NPC,	//SeaSnake=30
	ACTOR_ENEMY_NPC,	//Sharkman=31
	ACTOR_ENEMY_NPC,	//SharkSub=32
	ACTOR_ENEMY_NPC,	//Skeletalfish=33
	ACTOR_ENEMY_NPC,	//SpiderCrab=34
	ACTOR_ENEMY_NPC,	//Squiddart=35
	ACTOR_ENEMY_NPC,	//Stomper=36
	ACTOR_ENEMY_NPC,	//DustDevil=37
};

/*****************************************************************************/
sActorPool	CActorPool::ActorPool[MAX_ACTORS];

CSlotCache	CActorGfx::SlotCache;
u8			CActorGfx::UnpackBuffer[CActorPool::MAX_ACTOR_SIZE];

/*****************************************************************************/
/*** Slot Cache **************************************************************/
/*****************************************************************************/
void	CSlotCache::Init()
{
		for (int Y=0; Y<DYN_SLOTH; Y++)
		{
			for (int X=0; X<DYN_SLOTW; X++)
			{
				Cache[X][Y].W=0;
			}
		}
}

/*****************************************************************************/
bool	CSlotCache::FindSlot(int SprW,int SprH,u16 &TexX,u16 &TexY,u8 &u,u8 &v)
{
int		W=(SprW+(DYN_W-1))/DYN_W;
int		H=(SprH+(DYN_H-1))/DYN_H;

		for (int Y=0; Y<DYN_SLOTH; Y++)
		{
			for (int X=0; X<DYN_SLOTW; X++)
			{
				if (TakeSlot(X,Y,W,H))
				{
					printf("TakeSlot %i %i\n",X,Y);
					TexX=DYN_SLOTX+(X*DYN_W/4);
					TexY=DYN_SLOTY+(Y*DYN_H);
					u=(X&3)*64;
					v=(Y&3)*64;
					return(true);
				}
				
			}
		}
		return(false);
}

/*****************************************************************************/
bool	CSlotCache::TakeSlot(int SX,int SY,int SW,int SH)
{
int		W,H;
		if (SX+SW>DYN_SLOTW) return(false);
		if (SY+SH>DYN_SLOTH) return(false);
		if ((SX&3)==3 && SW>1) return(false);

		for (H=0; H<SH; H++)
		{
			for (W=0; W<SW; W++)
			{
				if (Cache[SX+W][SY+H].W) return(false);
			}
		}
// Slot good
		for (H=0; H<SH; H++)
		{
			for (W=0; W<SW; W++)
			{
				Cache[SX+W][SY+H].W=SW;
				Cache[SX+W][SY+H].H=SH;
			}
		}
			
	return(true);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CActorPool::Init()
{
int		i;
		for (i=0; i<MAX_ACTORS; i++)
		{
			ActorPool[i].SpriteBank=0;
		}
		CActorGfx::ResetCache();
}

/*****************************************************************************/
int		CActorPool::FindActorInPool(FileEquate Filename)
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			if (ActorPool[i].SpriteBank && ActorPool[i].Filename==Filename) return(i);
		}
		return(-1);
}

/*****************************************************************************/
int		CActorPool::FindFreeActor()
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			if (ActorPool[i].SpriteBank==0) return(i);
		}
		return(-1);
}

/*****************************************************************************/
int		CActorPool::LoadActor(FileEquate Filename)
{
int		i;

sSpriteAnimBank	*Spr=(sSpriteAnimBank*)CFileIO::loadFile(Filename);

		Spr->AnimList=(sSpriteAnim*)	MakePtr(Spr,(int)Spr->AnimList);
		Spr->FrameList=(sSpriteFrame*)	MakePtr(Spr,(int)Spr->FrameList);
		Spr->Palette=(u8*)				MakePtr(Spr,(int)Spr->Palette);

// FixUp AnimList
		DAVE_DBGMSG("Anims %i\n",Spr->AnimCount);
		for (i=0; i<Spr->AnimCount; i++)
		{
			sSpriteAnim	*ThisAnim=&Spr->AnimList[i];
			ThisAnim->Anim=(u16*)	MakePtr(Spr,(int)ThisAnim->Anim);
		}
// FixUp FrameList
		DAVE_DBGMSG("Anims %i\n",Spr->FrameCount);
		for (i=0; i<Spr->FrameCount; i++)
		{
			sSpriteFrame	*ThisFrame=&Spr->FrameList[i];
			ThisFrame->PAKSpr=(u8*)				MakePtr(Spr,(int)ThisFrame->PAKSpr);
		}

// Store it
int		Idx=FindFreeActor();
		ASSERT(Idx!=-1);

sActorPool	&ThisActor=ActorPool[Idx];

		ThisActor.Filename=Filename;
		ThisActor.SpriteBank=Spr;
		ThisActor.RefCount=1;
		ThisActor.Clut=LoadPalette(ThisActor,Idx);
		
		return(Idx);
}

/*****************************************************************************/
#define	DYN_PALW	64
#define	DYN_PALH	1
#define	DYN_PALX	DYN_PALW*(8+4)
#define	DYN_PALY	511

u16		CActorPool::LoadPalette(sActorPool &ThisActor,int Idx)
{
RECT	R;

		R.x=DYN_PALX+(Idx*DYN_PALW);
		R.y=DYN_PALY;
		R.w=DYN_PALW;
		R.h=DYN_PALH;
		LoadImage( &R, (u32*)ThisActor.SpriteBank->Palette);

int		Clut=getClut(R.x,R.y);
		return(Clut);
}

/*****************************************************************************/
void	CActorPool::AddActor(FileEquate Filename)
{
sActorPool	*Actor;
int			Idx=FindActorInPool(Filename);

		if (Idx!=-1) return;
// Load it
		LoadActor(Filename);
}

/*****************************************************************************/
CActorGfx	*CActorPool::GetActor(FileEquate Filename)
{
CActorGfx	*Actor;
int			Idx;
		
// Find Actor in Pool
		Idx=FindActorInPool(Filename);
		if (Idx==-1) ASSERT(!"Actor Not Loaded");

sActorPool	&ThisActor=ActorPool[Idx];
		Actor=new ("CActorGfx") CActorGfx;
		Actor->SetData(Filename,ThisActor.SpriteBank,ThisActor.Clut);
		ThisActor.RefCount++;
		return(Actor);
}

/*****************************************************************************/
/*** Dump ********************************************************************/
/*****************************************************************************/
// Dumps all apart from spongeybob
void	CActorPool::DumpActors()
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			if (ActorPool[i].SpriteBank && ActorPool[i].Filename!=ACTORS_SPONGEBOB_SBK)
			{
				if (ActorPool[i].SpriteBank) MemFree(ActorPool[i].SpriteBank);
				ActorPool[i].SpriteBank=0;
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CActorGfx::CActorGfx()
{
}

/*****************************************************************************/
CActorGfx::~CActorGfx()
{
}

/*****************************************************************************/
void	CActorGfx::SetData(FileEquate Filename,sSpriteAnimBank *_SpriteBank,u16 _Clut)
{
		SpriteBank=_SpriteBank;
		Clut=_Clut;

		if (Filename!=ACTORS_SPONGEBOB_SBK)
		{
			bool	SlotStatus=SlotCache.FindSlot(_SpriteBank->MaxW,_SpriteBank->MaxH,TexX,TexY,U,V);
			ASSERT(SlotStatus);
		}
		else
		{
			TexX=512;
			TexY=256;
			U=0;
			V=0;
		}
		TPage=getTPage(0,0,TexX,TexY);
}

/*****************************************************************************/
void		CActorGfx::ResetCache()
{
RECT		R;
			SlotCache.Init();
			R.x=512;
			R.y=256;
			R.w=512;
			R.h=250;
			ClearImage(&R,0,255,0);
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

			PAK_doUnpak(UnpackBuffer,FrameGfx->PAKSpr);
// Gfx

RECT		Rect;
			Rect.x=TexX;
			Rect.y=TexY;
			Rect.w=FrameGfx->W/4;
			Rect.h=FrameGfx->H;
			LoadImage( &Rect, (u32*)UnpackBuffer);

POLY_FT4	*Ft4=GetPrimFT4();
			SetUpFT4(Ft4,FrameGfx,Pos.vx,Pos.vy,XFlip,YFlip);
			setRGB0(Ft4,128,128,128);
			Ft4->tpage=TPage;
			Ft4->clut=Clut;
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
u8		W=Frame->W;
u8		H=Frame->H;

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



/*******************/
/*** Actor Stuff ***/
/*******************/

#ifndef __ACTOR_HEADER__
#define __ACTOR_HEADER__

#include "system\global.h"
#include <dstructs.h>

#ifndef	__BigLump_H__
#include <biglump.h>
#endif

struct	sSlot
{
		u16		W,H;
};

/*****************************************************************************/
struct	sActorPool
{
		FileEquate		Filename;
		sSpriteAnimBank	*SpriteBank;
		u16				RefCount;
		u16				Clut;
};

/*****************************************************************************/
class	CSlotCache
{
public:
		enum
		{
			DYN_W			=64,
			DYN_H			=64,
			DYN_TPAGEW		=4*256,
			DYN_TPAGEH		=1*256,
			DYN_SLOTW		=(DYN_TPAGEW/DYN_W),
			DYN_SLOTH		=(DYN_TPAGEH/DYN_H),
			DYN_SLOTX		=512+256,
			DYN_SLOTY		=256,
		};
		CSlotCache(){};
		~CSlotCache(){};


		void	Init();
		bool	FindSlot(int SprW,int SprH,u16 &TexX,u16 &TexY,u8 &u,u8 &v);
protected:
		bool	TakeSlot(int SX,int SY,int SW,int SH);

		sSlot	Cache[DYN_SLOTW][DYN_SLOTH];

};
/*****************************************************************************/
class CActorGfx;
class CActorPool
{
public:
		enum
		{
			MAX_ACTORS		=64,
			MAX_ACTOR_SIZE=	128*128,
		};

		enum ACTOR_TYPE
		{
			ACTOR_PLAYER = 0,
			ACTOR_FRIEND_NPC = 1,
			ACTOR_ENEMY_NPC,
			ACTOR_UNKNOWN,
		};

static	void		Init();

static	void		AddActor(FileEquate Filename)		{GetActor(Filename);}
static	CActorGfx	*GetActor(FileEquate Filename);
static	void		DumpActors();
		

static	ACTOR_TYPE	getActorType( int actorNum )			{return actorType[actorNum];}

protected:
static	int			FindActorInPool(FileEquate Filename);
static	int			FindFreeActor();
static	int			LoadActor(FileEquate Filename);
static	u16			LoadPalette(sActorPool &ThisActor,int Idx);


static	sActorPool	ActorPool[];

static	ACTOR_TYPE	actorType[];

};

/*****************************************************************************/
class	CActorGfx
{
public:
		CActorGfx();
virtual	~CActorGfx();

		void		SetData(FileEquate Filename,sSpriteAnimBank *_SpriteBank,u16 _Clut);
static	void		ResetCache();

		POLY_FT4	*Render(DVECTOR &Pos,int Anim,int Frame,bool FlipX=false,bool FlipY=false,bool Shadow=false);

		int			getFrameCount(int Anim)		{return(SpriteBank->AnimList[Anim].FrameCount);}


protected:
		void		SetUpFT4(POLY_FT4 *Ft4,sSpriteFrame *ThisFrame,int X,int Y,bool XFlip,bool YFlip);
		sSpriteFrame	*GetFrame(int Anim,int Frame);

		sSpriteAnimBank	*SpriteBank;

//		RECT			DrawRect;
		u16				Clut;
		u16				TPage,TexX,TexY;
		u8				U,V;

static	CSlotCache	SlotCache;
static	u8			UnpackBuffer[CActorPool::MAX_ACTOR_SIZE];


};

/*****************************************************************************/

#endif
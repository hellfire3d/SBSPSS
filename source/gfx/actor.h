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

/*****************************************************************************/
// Pack together Actor anim & frame for quicker check later
struct	sPoolNode
{
		u16			Actor;
		u16			Anim;
		u16			Frame;
		u16			TPage;
		u16			TexX,TexY;
		u8			U,V;
		sPoolNode	*Prev,*Next;
};

struct	sNodeList
{
		sPoolNode	*List;
		sPoolNode	*LastNode;
};

struct	sPoolSlot
{
		u16			Width,Height;
		u16			RefCount;
		u16			FrameCount;
		sNodeList	NodeList;
		u8			*ListMem;
};

/*****************************************************************************/
struct	sActorPool
{
		FileEquate		Filename;
		sSpriteAnimBank	*ActorGfx;
		int				CacheSlot;
		sNodeList		*CachePool;
		sNodeList		ThisCache;
		sActorPool		*Next;
};

/*****************************************************************************/
class	CActorCache
{
public:
		enum
		{
			TPAGE_W			=256,
			TPAGE_H			=256-4,

			CACHE_X			=512+256,
			CACHE_Y			=256,
			CACHE_W			=4,
			CACHE_H			=1,

			CACHE_PALX		=CACHE_X,
			CACHE_PALY		=511,
			CACHE_PALW		=64,
			CACHE_PALH		=1,

			CACHE_TYPE_MAX	=8,
		};

		CActorCache();
		~CActorCache();

		int			ReserveSlot(int W,int H,int FrameCount);
		void		AllocCache();
		void		Reset();
		void		LoadPalette(sActorPool *NewActor);
		sNodeList	*GetSlotList(int Slot)		{return(&SlotList[Slot].NodeList);}

static	sPoolNode	*RemoveHeadNode(sNodeList *Root);
static	void		RemoveNode(sPoolNode *Node,sNodeList *Root);
static	void		AddNode(sPoolNode *Node,sNodeList *Root);
static	u8			*UnpackBuffer;

protected:
		int			GetSlot(int W,int H);
		void		InitCache(int Type,int Count);
		int			GetSizeType(int Size);

		sPoolSlot	SlotList[CACHE_TYPE_MAX];

		int			CurrentTPX;
		int			CurrentPalette;
		int			SlotCount;

};

/*****************************************************************************/
class CActorGfx;
class CActorPool
{
public:

static	void		Init();
static	void		Reset();
static	void		SetUpCache();		

static	void		AddActor(FileEquate Filename)		{GetActor(Filename);}
static	CActorGfx	*GetActor(FileEquate Filename);

protected:
static	sActorPool	*FindActor(FileEquate Filename);
static	sActorPool	*LoadActor(FileEquate Filename);
static	void		AddActor(sActorPool *ThisActor);

static	CActorCache	Cache;
static	sActorPool	*ActorList,*LastActor;
};

/*****************************************************************************/
class	CActorGfx
{
public:
		enum
		{
			ShadowXOfs	=32,
			ShadowYOfs	=32,
		};

		CActorGfx(sActorPool *ThisActor);
virtual	~CActorGfx();

		POLY_FT4		*Render(DVECTOR &Pos,int Anim,int Frame,bool FlipX=false,bool FlipY=false,bool Shadow=false);
		POLY_FT4		*RotateScale(POLY_FT4 *Ft4,DVECTOR &Pos,int Angle,int XScale,int YScale);

		int				getFrameCount(int Anim)		{return(PoolEntry->ActorGfx->AnimList[Anim].FrameCount);}
		int				GetTotalFrameCount();

		sBBox			&GetBBox()					{return(BBox);}
protected:
		void			SetUpFT4(POLY_FT4 *Ft4,sSpriteFrame *Frame,sPoolNode *Node,int X,int Y,bool XFlip,bool YFlip);
		sSpriteFrame	*GetFrame(int Anim,int Frame);

		sActorPool		*PoolEntry;
		sSpriteFrame	*CurrentFrame;

		sBBox			BBox;
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CModelGfx
{
public:
		CModelGfx(){};
virtual	~CModelGfx(){};

static	void		SetData(sModel *Table,sTri *TList,sQuad *QList,sVtx *VList);
		void		SetModel(int Type);

		void		Render(DVECTOR &Pos);

protected:
static	sModel		*ModelTable;
static	sTri		*ModelTriList;
static	sQuad		*ModelQuadList;
static	sVtx		*ModelVtxList;
		sModel		*Model;
};

/*****************************************************************************/

#endif
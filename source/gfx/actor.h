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
struct	sPoolNode
{
		sSpriteFrameGfx	*Frame;
		RECT			DstRect;
		u16				TPage;
		u8				U,V;
		sPoolNode		*Prev,*Next;
};

struct	sNodeList
{
		sPoolNode	*Head;
		sPoolNode	*Tail;
};

struct	sPoolSlot
{
		u16			Width,Height;
//		u16			RefCount;
//		u16			FrameCount;
		sNodeList	NodeList;
		u8			*ListMem;
		u16			SlotCount;
};

/*****************************************************************************/
struct	sActorPool
{
		FileEquate		Filename;
		s16				CacheSlot;
		sSpriteAnimBank	*ActorGfx;
		sNodeList		*GlobalCache;
		sNodeList		LocalCache;
		sNodeList		LastCache;
		sActorPool		*Next;
};

/*****************************************************************************/
class	CActorCache
{
public:
		enum
		{
			TPAGE_W			=256,
			TPAGE_H			=256-2,	// (allow for 2 lines of palettes)

			CACHE_X			=512,
			CACHE_Y			=256,
			CACHE_W			=8,
			CACHE_H			=1,

			CACHE_PALX		=CACHE_X,
			CACHE_PALY		=510,
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
		sPoolSlot	&GetSlot(int Slot)			{return(SlotList[Slot]);}
		sNodeList	*GetSlotList(int Slot)		{return(&SlotList[Slot].NodeList);}
		int			GetSlotCount()				{return(SlotCount);}

static	sPoolNode	*RemoveHeadNode(sNodeList *Root);
static	void		RemoveNode(sPoolNode *Node,sNodeList *Root);
static	void		AddNode(sPoolNode *Node,sNodeList *Root);
static	void		AddNodeList(sNodeList *Src,sNodeList *Dst);

protected:
		int			ReAllocSlot(int W,int H);
		int			GetSlot(int W,int H);
		void		InitCache(int Type,int Count);
		int			GetSizeType(int Size)		{return((Size+15)&-16);}

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
static	void		CleanUpCache();

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
		CActorGfx(sActorPool *ThisActor);
virtual	~CActorGfx();

		void			setActor(sActorPool *ThisActor);

		POLY_FT4		*Render(DVECTOR &Pos,int Anim,int Frame,bool FlipX=false,bool FlipY=false);
		POLY_FT4		*RotateScale(POLY_FT4 *Ft4,DVECTOR &Pos,int Angle,int XScale,int YScale,bool Force=false);
		void			getFrameOffsets(int _anim,int _frame,int *_x,int *_y);

		int				getFrameCount(int Anim)		{return(PoolEntry->ActorGfx->AnimList[Anim].FrameCount);}
		int				GetTotalFrameCount()		{return(PoolEntry->ActorGfx->FrameCount);}

		sBBox			&GetBBox()					{return(BBox);}

		void			SetOtPos(int Ot)			{OtPos=Ot;}


protected:
		sPoolNode		*CacheFrame(int Anim,int Frame);
		void			SetUpFT4(POLY_FT4 *Ft4,sPoolNode *Node,int X,int Y,bool XFlip,bool YFlip);

		sActorPool		*PoolEntry;
		sSpriteFrame	*CurrentFrame;
		sSpriteFrameGfx	*CurrentFrameGfx;

		sBBox			BBox;
		u32				OtPos;
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CModelGfx
{
public:
		CModelGfx(){};
virtual	~CModelGfx(){};

static	void		SetData(sLevelHdr *LevelHdr);
		void		SetModel(int Type);

static	void		RenderTile(DVECTOR &Pos,int TileID);
static	void		RenderElem(sElem3d *Elem,DVECTOR &Pos,SVECTOR *Angle=0,VECTOR *Scale=0,s32 ClipFlag=0xffffffff,u32 *TransBuffer=(u32*)SCRATCH_RAM);

		void		Render(DVECTOR &Pos,SVECTOR *Angle=0,VECTOR *Scale=0,s32 ClipFlag=0) {RenderElem(Elem,Pos,Angle,Scale,ClipFlag);}
		void		RenderNoClip(DVECTOR &Pos,SVECTOR *Angle=0,VECTOR *Scale=0)	{Render(Pos,Angle,Scale,0xffffffff);}

		sBBox		&GetBBox()		{return(Model->BBox);}

protected:
		void		CacheModelVtx();

static	sModel		*ModelTable;
static	sElem3d		*ElemBank;
static	sTri		*TriList;
static	sQuad		*QuadList;
static	sVtx		*VtxList;
static	u16			*VtxIdxList;


		sModel		*Model;
		sElem3d		*Elem;

};

/*****************************************************************************/

#endif
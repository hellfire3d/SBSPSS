/******************/
/*** Actor Bank ***/
/******************/

#include "system\vid.h"
#include "system\global.h"
#include "mem\memory.h"
#include "fileio\fileio.h"
#include "utils\utils.h"
#include "gfx\prim.h"
#include "gfx\actor.h"
#include "gfx\otpos.h"
#include "gfx\animtex.h"
#include "game\game.h"

#include <dstructs.h>

/*****************************************************************************/
const int	BBOX_ADJ=4;

/*****************************************************************************/
CActorCache	CActorPool::Cache;
sActorPool	*CActorPool::ActorList,*CActorPool::LastActor;

/*****************************************************************************/
/*** Cache *******************************************************************/
/*****************************************************************************/
CActorCache::CActorCache()
{
		for (int i=0;i<CACHE_TYPE_MAX; i++)
		{
			SlotList[i].ListMem=0;
		}
}

/*****************************************************************************/
CActorCache::~CActorCache()
{
}

/*****************************************************************************/
int		CActorCache::ReAllocSlot(int W,int H)
{
int		i;
int		Slot=-1;
int		SlotW=+32000,SlotH=+32000;

// Find best slot which fits in
	
		for (i=0;i<SlotCount; i++)
		{
			int	ThisW=(SlotList[i].Width-W);
			int	ThisH=(SlotList[i].Height-H);

			if (ThisW<SlotW || ThisH<SlotH)
			{
				Slot=i;
				SlotW=ThisW;
				SlotH=ThisH;
			}
		}

		if (SlotList[Slot].Width<W) 
		{
			SlotList[Slot].Width=W; 
		}
		if (SlotList[Slot].Height<H) 
		{
			SlotList[Slot].Height=H;
		}

		return(Slot);
}

/*****************************************************************************/
int		CActorCache::GetSlot(int W,int H)
{
int		i;
int		Slot=0;

		W=GetSizeType(W);
		H=GetSizeType(H);

		for (i=0; i<SlotCount && !Slot; i++)
		{ // Slot exist?
			if (SlotList[i].Width==W && SlotList[i].Height==H) Slot=i;
		}

		if (!Slot)
		{ // Use New Slot
			if (SlotCount>=CACHE_TYPE_MAX)
			{ // No free slots, grab an existing one, and alter size to fit
				Slot=ReAllocSlot(W,H);
			}
			else
			{
			Slot=SlotCount;
			SlotList[Slot].Width=W;
			SlotList[Slot].Height=H;
			SlotCount++;
			}
		}
//		SlotList[Slot].RefCount++;

		return(Slot);
}

/*****************************************************************************/
sPoolNode *CActorCache::RemoveHeadNode(sNodeList *RootNode)
{
sPoolNode *ThisNode=RootNode->Head;
sPoolNode *NextNode=ThisNode->Next;

		RootNode->Head=NextNode;
		ThisNode->Next=0;
		if (NextNode)
		{
			NextNode->Prev=0;
		}
		else
		{ // Its Tail Node
			RootNode->Tail=0;
		}
		return(ThisNode);
}

/*****************************************************************************/
void	CActorCache::RemoveNode(sPoolNode *ThisNode,sNodeList *RootNode)
{
sPoolNode	*PrevNode=ThisNode->Prev;
sPoolNode	*NextNode=ThisNode->Next;

		if (PrevNode)
		{ // Not Head Node
			PrevNode->Next=NextNode;
		}
		else
		{ // Head Node
			RootNode->Head=NextNode;
		}

		if (NextNode)
		{ // Not Tail Node
			NextNode->Prev=PrevNode;
		}
		else
		{ // Tail Node
			RootNode->Tail=PrevNode;
		}
		ThisNode->Next=0;
		ThisNode->Prev=0;
}

/*****************************************************************************/
// Add node to end of list
void	CActorCache::AddNode(sPoolNode *ThisNode,sNodeList *RootNode)
{
sPoolNode	*TailNode=RootNode->Tail;

		if (TailNode)
		{
			TailNode->Next=ThisNode;
		}
		else
		{ // List is empty 
			RootNode->Head=ThisNode;
		}
		ThisNode->Prev=TailNode;
		ThisNode->Next=0;
		ASSERT(ThisNode);
		RootNode->Tail=ThisNode;
}

/*****************************************************************************/
// Add node list to end of list
void	CActorCache::AddNodeList(sNodeList *Src,sNodeList *Dst)
{
sPoolNode	*SrcHead=Src->Head;
sPoolNode	*SrcTail=Src->Tail;
sPoolNode	*DstHead=Dst->Head;
sPoolNode	*DstTail=Dst->Tail;
/*
		if (!SrcHead) return;

		if (DstTail)
		{
			DstTail->Next=SrcHead;
		}
		else
		{ // List is empty 
			Dst->Head=SrcHead;
		}

		SrcHead->Prev=DstTail;
		Dst->Tail=SrcTail;
		Src->Head=0;
		Src->Tail=0;
*/
		while(SrcHead)
		{
			sPoolNode	*Next=SrcHead->Next;
			AddNode(SrcHead,Dst);
			SrcHead=Next;
		}

}

/*****************************************************************************/
int		CActorCache::ReserveSlot(int W,int H,int FrameCount)
{
int		Slot=GetSlot(W,H);
//		SlotList[Slot].FrameCount+=FrameCount;
		return(Slot);
}

/*****************************************************************************/
void	CActorCache::AllocCache()
{
int		TPW=CACHE_W/SlotCount;
int		MaxW=0;
int		MaxH=0;

		TPW=1;

		ASSERT(SlotCount<=CACHE_W);

		for (int i=0; i<SlotCount; i++)
		{
			if (MaxW<SlotList[i].Width)		MaxW=SlotList[i].Width;
			if (MaxH<SlotList[i].Height)	MaxH=SlotList[i].Height;
			InitCache(i,TPW);
		}
		CPakTex::Init(MaxW*MaxH);
}

/*****************************************************************************/
// This needs improving to 'grab' free areas left from bigger grabs (YIKES!)
void	CActorCache::InitCache(int Slot,int TPW)
{
sPoolSlot	*ThisSlot=&SlotList[Slot];
int			W=(TPW*(TPAGE_W))/ThisSlot->Width;
int			H=(TPAGE_H)/ThisSlot->Height;
int			Total=W*H;
sPoolNode	*List;

// Init List
			
			ThisSlot->SlotCount=Total;
			ThisSlot->ListMem=(u8*)MemAlloc(Total*sizeof(sPoolNode),"CacheNodeList");
			List=(sPoolNode*)ThisSlot->ListMem;

// Create List Entries
			for (int Y=0; Y<H; Y++)
			{
				for (int X=0; X<W; X++)
				{
					int	U=(X*ThisSlot->Width);
					int	V=(Y*ThisSlot->Height);
					int	TexX=(CACHE_X+CurrentTPX+(U>>2));
					int	TexY=(CACHE_Y+V);

					List->Frame=0;
					List->DstRect.x=TexX;
					List->DstRect.y=TexY;
					List->U=U&255;
					List->V=V&255;
					List->TPage=getTPage(0,0,TexX,TexY);
					AddNode(List,&ThisSlot->NodeList);
					List++;
				}
			}

			CurrentTPX+=(TPW*TPAGE_W)>>2;
}

/*****************************************************************************/
void	CActorCache::Reset()
{
// Free and init lists
		for (int i=0;i<CACHE_TYPE_MAX; i++)
		{
			if (SlotList[i].ListMem) MemFree(SlotList[i].ListMem);
			SlotList[i].ListMem=0;
			SlotList[i].NodeList.Head=0;
			SlotList[i].NodeList.Tail=0;
//			SlotList[i].RefCount=0;
//			SlotList[i].FrameCount=0;
			SlotList[i].Width=0;
			SlotList[i].Height=0;
		}
		CPakTex::Shutdown();

		CurrentTPX=0;
		CurrentPalette=0;
		SlotCount=0;

// Clear VRam
RECT	R={CACHE_X,CACHE_Y,TPAGE_W*CACHE_W,TPAGE_H*CACHE_H};
		ClearImage(&R,0,255,0);
}

/*****************************************************************************/
void	CActorCache::LoadPalette(sActorPool *Actor)
{
		if (!Actor->ActorGfx->Clut)
		{
			RECT	R;
			int	PalNo;

			if (Actor->Filename==ACTORS_SPONGEBOB_SBK)
			{
				PalNo=0;
			}
			else
			{
				PalNo=CurrentPalette+1;
			}

			int		X=PalNo%CACHE_W;
			int		Y=PalNo/CACHE_W;

			R.x=CACHE_PALX+(X*CACHE_PALW);
			R.y=CACHE_PALY-Y;
			R.w=CACHE_PALW;
			R.h=CACHE_PALH;
			while(DrawSync(1));
			LoadImage( &R, (u32*)Actor->ActorGfx->Palette);
			Actor->ActorGfx->Clut=getClut(R.x,R.y);
			CurrentPalette++;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CActorPool::Init()
{
		ActorList=0;
		Cache.Reset();
}

/*****************************************************************************/
void	CActorPool::Reset()
{
sActorPool	*List=ActorList;
			
			while (List) 
			{
				sActorPool	*Next=List->Next;
				if (List->Filename!=ACTORS_SPONGEBOB_SBK)
				{
					MemFree(List->ActorGfx);
					delete List;
				}
				else
				{
					List->Next=0;			
				}
				List=Next;
			}
		LastActor=ActorList;
		Cache.Reset();
}

/*****************************************************************************/
void	CActorPool::SetUpCache()
{
		Cache.AllocCache();
// SetUp Actors
sActorPool	*List=ActorList;

			while (List)
			{
				List->GlobalCache=Cache.GetSlotList(List->CacheSlot);
				List->LocalCache.Head=0;
				List->LocalCache.Tail=0;
				List->LastCache.Head=0;
				List->LastCache.Tail=0;
				List=List->Next;
			}
}

/*****************************************************************************/
sActorPool	*CActorPool::FindActor(FileEquate Filename)
{
sActorPool	*List=ActorList;

		while (List)
		{
			if (List->ActorGfx && List->Filename==Filename) return(List);
			List=List->Next;
		}
		return(0);
}

/*****************************************************************************/
CActorGfx	*CActorPool::GetActor(FileEquate Filename)
{
sActorPool	*ThisActor;
CActorGfx	*NewActor;
int			TotalFrames=0;
bool		NewFlag=false;
// Find Actor in Pool
		ThisActor=FindActor(Filename);
		if (!ThisActor) 
		{
			ThisActor=LoadActor(Filename);
			NewFlag=true;
		}

		NewActor=new ("CActorGfx") CActorGfx(ThisActor);

		if (NewFlag)
		{
			TotalFrames=NewActor->GetTotalFrameCount();
		}

		ThisActor->CacheSlot=Cache.ReserveSlot(ThisActor->ActorGfx->MaxW,ThisActor->ActorGfx->MaxH,TotalFrames);

		return(NewActor);
}

/*****************************************************************************/
sActorPool	*CActorPool::LoadActor(FileEquate Filename)
{
int		i;
int		TotalFrames=0;

sSpriteAnimBank	*Spr=(sSpriteAnimBank*)CFileIO::loadFile(Filename,"ActorGfx");

		Spr->AnimList=(sSpriteAnim*)		MakePtr(Spr,(int)Spr->AnimList);
		Spr->FrameList=(sSpriteFrameGfx*)	MakePtr(Spr,(int)Spr->FrameList);
		Spr->Palette=(u8*)					MakePtr(Spr,(int)Spr->Palette);

// FixUp AnimList
		for (i=0; i<Spr->AnimCount; i++)
		{
			sSpriteAnim	*ThisAnim=&Spr->AnimList[i];
			ThisAnim->Anim=(sSpriteFrame*)	MakePtr(Spr,(int)ThisAnim->Anim);
			TotalFrames+=ThisAnim->FrameCount;
		}

// FixUp FrameList (not blank ones)
		for (i=0; i<Spr->FrameCount; i++)
		{
			sSpriteFrameGfx	*ThisFrame=&Spr->FrameList[i];
			if (ThisFrame->PAKSpr)
			{
				ThisFrame->PAKSpr=(u8*)				MakePtr(Spr,(int)ThisFrame->PAKSpr);
			}
		}

// Store it
sActorPool	*NewActor;

		NewActor=new ("ActorPool") sActorPool();
		NewActor->Filename=Filename;
		NewActor->ActorGfx=Spr;
		AddActor(NewActor);

		return(NewActor);
}

/*****************************************************************************/
void		CActorPool::AddActor(sActorPool *NewActor)
{
// Insert into list

			if (!LastActor)
			{ // Empty List
				ActorList=NewActor;
			}
			else
			{ 
				LastActor->Next=NewActor;
			}
			NewActor->Next=0;
			Cache.LoadPalette(NewActor);
			LastActor=NewActor;
}

/*****************************************************************************/
void	CActorPool::CleanUpCache()
{
sActorPool	*Actor=ActorList;
			
		while (Actor) 
		{
			Actor->LastCache.Head=Actor->LocalCache.Head;
			Actor->LastCache.Tail=Actor->LocalCache.Tail;
			CActorCache::AddNodeList(&Actor->LocalCache ,Actor->GlobalCache);
			Actor->LocalCache.Head=0;
			Actor->LocalCache.Tail=0;
			ASSERT(Actor->GlobalCache->Head);
			ASSERT(Actor->GlobalCache->Tail);
			Actor=Actor->Next;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CActorGfx::CActorGfx(sActorPool *ThisActor)
{
		setActor(ThisActor);	
}

/*****************************************************************************/
CActorGfx::~CActorGfx()
{
}

/*****************************************************************************/
void	CActorGfx::setActor(sActorPool *ThisActor)
{
		PoolEntry=ThisActor;
		OtPos=OTPOS__ACTOR_POS;
}

/*****************************************************************************/
sPoolNode	*CActorGfx::CacheFrame(int Anim,int Frame)
{
sPoolNode		*ThisNode,*FindNode;

// Calc Frame Ptrs
sSpriteAnimBank	*SpriteBank=PoolEntry->ActorGfx;
sSpriteAnim	*ThisAnim=SpriteBank->AnimList+Anim;

			CurrentFrame=&ThisAnim->Anim[Frame];
			CurrentFrameGfx=&SpriteBank->FrameList[CurrentFrame->FrameIdx];

			if (!CurrentFrameGfx->PAKSpr) return(0);	// Blank Frame

// Try to find Pre-cached sprite
			ThisNode=0;
// Check Local Cache
			FindNode=PoolEntry->LocalCache.Head;
			while (FindNode)
			{ // Try local Cache (From Head forward)
				if (FindNode->Frame==CurrentFrameGfx) 
				{
					ThisNode=FindNode;
					break;
				}
				FindNode=FindNode->Next;
			}

// Check Last Cache
			if (!ThisNode)
			{
				FindNode=PoolEntry->LastCache.Head;
				while (FindNode)
				{
					if (FindNode->Frame==CurrentFrameGfx) 
					{
						ThisNode=FindNode;
						CActorCache::RemoveNode(ThisNode,PoolEntry->GlobalCache);
						CActorCache::AddNode(ThisNode,&PoolEntry->LocalCache);
						break;
					}
					if (FindNode==PoolEntry->LastCache.Tail) break;
					FindNode=FindNode->Next;
				}

			}
			
// Check Global Cache (From Tail back)
			if (!ThisNode)
			{
				FindNode=PoolEntry->GlobalCache->Tail;
				while (FindNode)
				{
					if (FindNode->Frame==CurrentFrameGfx) 
					{
						ThisNode=FindNode;
						CActorCache::RemoveNode(ThisNode,PoolEntry->GlobalCache);
						CActorCache::AddNode(ThisNode,&PoolEntry->LocalCache);
						break;
					}
					FindNode=FindNode->Prev;
				}
			}

// Could not find it, get new
			if (!ThisNode)
			{ // Not cached frame
				ThisNode=CActorCache::RemoveHeadNode(PoolEntry->GlobalCache);
				ASSERT(ThisNode);
				CActorCache::AddNode(ThisNode,&PoolEntry->LocalCache);

				ThisNode->Frame=CurrentFrameGfx;
				
				ThisNode->DstRect.w=CurrentFrameGfx->W>>2;	// div 4 cos 16 color
				ThisNode->DstRect.h=CurrentFrameGfx->H;

				CPakTex::Add(CurrentFrameGfx->PAKSpr,&ThisNode->DstRect);
			}
		return(ThisNode);
}

/*****************************************************************************/
void	CActorGfx::SetUpFT4(POLY_FT4 *Ft4,sPoolNode *Node,int X,int Y,bool XFlip,bool YFlip)
{
u8		W=CurrentFrameGfx->W-1;
u8		H=CurrentFrameGfx->H-1;
u8		U=Node->U;
u8		V=Node->V;
int		AspectX0,AspectX1;

		if (XFlip)
			{
			X-=CurrentFrame->XOfs;
			X-=W;
			Ft4->u0=U+W;
			Ft4->u1=U;//-1;//
			Ft4->u2=U+W;
	 		Ft4->u3=U;//-1;//
			AspectX0=CurrentFrameGfx->AspectX1;
			AspectX1=CurrentFrameGfx->AspectX0;
			}
		else
		{
			X+=CurrentFrame->XOfs;
			Ft4->u0=U;
			Ft4->u1=U+W;
			Ft4->u2=U;
 			Ft4->u3=U+W;
			AspectX0=CurrentFrameGfx->AspectX0;
			AspectX1=CurrentFrameGfx->AspectX1;
		}

		if (YFlip)
			{
			Y-=CurrentFrame->YOfs;
			Y-=H;
			Ft4->v0=V+H;
			Ft4->v1=V+H;
			Ft4->v2=V-1;//
			Ft4->v3=V-1;//
			}
		else
		{
			Y+=CurrentFrame->YOfs;
			Ft4->v0=V;
			Ft4->v1=V;
			Ft4->v2=V+H;
			Ft4->v3=V+H;
		}

		setXYWH(Ft4,X,Y,W,H);

#if		INGAME_SCREENW==512
// Correct Aspect
		Ft4->x0-=AspectX0;
		Ft4->x1+=AspectX1;
		Ft4->x2-=AspectX0;
		Ft4->x3+=AspectX1;
#endif
}

/*****************************************************************************/
POLY_FT4	*CActorGfx::Render(DVECTOR &Pos,int Anim,int Frame,bool XFlip,bool YFlip)
{
sPoolNode	*ThisNode=CacheFrame(Anim,Frame);
			if (!ThisNode) return(0);	// Blank Frame
POLY_FT4	*Ft4=GetPrimFT4();

			SetUpFT4(Ft4,ThisNode,Pos.vx,Pos.vy,XFlip,YFlip);
			Ft4->tpage=ThisNode->TPage;
			Ft4->clut=PoolEntry->ActorGfx->Clut;
			setShadeTex(Ft4,1);
			setRGB0(Ft4,127,127,127);
			addPrim(OtPtr+OtPos,Ft4);

// Set BBox
// Sizes now depend on aspect corrected sizes, so get sizes back from poly
int			BBoxW=Ft4->x1-Ft4->x0;
int			BBoxH=Ft4->y2-Ft4->y0;
int			HalfW=BBoxW>>1;

			BBox.XMin=-HalfW+BBOX_ADJ;
			BBox.XMax=+HalfW-BBOX_ADJ;
			BBox.YMin=-BBoxH+BBOX_ADJ;
			BBox.YMax=0-BBOX_ADJ;

			return(Ft4);
}

/*****************************************************************************/
static const VECTOR	ZeroPos={0,0,0};
static const int	ScaleXAspect=2050;	// Odd hard coded number, sorry

POLY_FT4	*CActorGfx::RotateScale(POLY_FT4 *Ft4,DVECTOR &Pos,int Angle,int XScale,int YScale,bool Force)
{
MATRIX	Mtx;
VECTOR	Scale;

		Angle&=4095;
		if (Angle==0 && XScale==ONE && YScale==ONE && !Force) return(Ft4);

		Scale.vx=XScale;
		Scale.vy=YScale;
		Scale.vz=ONE;
		SetIdentNoTrans(&Mtx);
		RotMatrixZ(Angle,&Mtx);
		ScaleMatrix(&Mtx,&Scale);
		CorrectMatrixScale(&Mtx);
		gte_SetRotMatrix(&Mtx);
		CMX_SetTransMtxXY(&ZeroPos);

		
SVECTOR	I,O;
s32		Tmp;
		I.vz=0;
		I.vy=-CurrentFrameGfx->H;
		I.vx=-CurrentFrameGfx->W0;	RotTransSV(&I,&O,&Tmp); Ft4->x0=O.vx; Ft4->y0=O.vy;
		I.vx=+CurrentFrameGfx->W1;	RotTransSV(&I,&O,&Tmp); Ft4->x1=O.vx; Ft4->y1=O.vy;
		I.vy=0;
		I.vx=-CurrentFrameGfx->W0;	RotTransSV(&I,&O,&Tmp); Ft4->x2=O.vx; Ft4->y2=O.vy;
		I.vx=+CurrentFrameGfx->W1;	RotTransSV(&I,&O,&Tmp); Ft4->x3=O.vx; Ft4->y3=O.vy;

// Adjust BBox
int		XMin,XMax;
int		YMin,YMax;

		XMin=Ft4->x0;
		if (XMin>Ft4->x1) XMin=Ft4->x1;
		if (XMin>Ft4->x2) XMin=Ft4->x2;
		if (XMin>Ft4->x3) XMin=Ft4->x3;
		XMax=Ft4->x0;
		if (XMax<Ft4->x1) XMax=Ft4->x1;
		if (XMax<Ft4->x2) XMax=Ft4->x2;
		if (XMax<Ft4->x3) XMax=Ft4->x3;
		YMin=Ft4->y0;
		if (YMin>Ft4->y1) YMin=Ft4->y1;
		if (YMin>Ft4->y2) YMin=Ft4->y2;
		if (YMin>Ft4->y3) YMin=Ft4->y3;
		YMax=Ft4->y0;
		if (YMax<Ft4->y1) YMax=Ft4->y1;
		if (YMax<Ft4->y2) YMax=Ft4->y2;
		if (YMax<Ft4->y3) YMax=Ft4->y3;

		BBox.XMin=XMin+BBOX_ADJ;
		BBox.XMax=XMax-BBOX_ADJ;
		BBox.YMin=YMin+BBOX_ADJ;
		BBox.YMax=YMax-BBOX_ADJ;

		Ft4->x0+=Pos.vx; Ft4->y0+=Pos.vy;
		Ft4->x1+=Pos.vx; Ft4->y1+=Pos.vy;
		Ft4->x2+=Pos.vx; Ft4->y2+=Pos.vy;
		Ft4->x3+=Pos.vx; Ft4->y3+=Pos.vy;

		return(Ft4);
}

/*****************************************************************************/
void	CActorGfx::getFrameOffsets(int _anim,int _frame,int *_x,int *_y)
{
	sSpriteAnimBank	*SpriteBank;
	sSpriteAnim		*ThisAnim;
	sSpriteFrame	*pFrame;

	SpriteBank=PoolEntry->ActorGfx;
	ThisAnim=SpriteBank->AnimList+_anim;
	pFrame=&ThisAnim->Anim[_frame];
	*_x=pFrame->XOfs;
	*_y=pFrame->YOfs;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
sModel	*CModelGfx::ModelTable;
sElem3d	*CModelGfx::ElemBank;
sTri	*CModelGfx::TriList;
sQuad	*CModelGfx::QuadList;
sVtx	*CModelGfx::VtxList;
u16		*CModelGfx::VtxIdxList;

/*****************************************************************************/
void	CModelGfx::SetData(sLevelHdr *LevelHdr)
{
		ModelTable=LevelHdr->ModelList;
		ElemBank=LevelHdr->ElemBank3d;
		TriList=LevelHdr->TriList;
		QuadList=LevelHdr->QuadList;
		VtxList=LevelHdr->VtxList;
		VtxIdxList=LevelHdr->VtxIdxList;
}

/*****************************************************************************/
void	CModelGfx::SetModel(int Type)
{
		Model=&CModelGfx::ModelTable[Type];
		Elem=&ElemBank[Model->ElemID];
}

/*****************************************************************************/
static const int	ElemXMin=-(16/2);
static const int	ElemXMax=+(16/2);
static const int	ElemYMin=-(16/2);
static const int	ElemYMax=+(16/2);
static const int	ElemZMin=-(16*4);
static const int	ElemZMax=+(16*4);

static VECTOR	VtxTable[8]=
{
	{ElemXMin,ElemYMin,ElemZMin},	// FLU
	{ElemXMax,ElemYMin,ElemZMin},	// FRU
	{ElemXMin,ElemYMax,ElemZMin},	// FLD
	{ElemXMax,ElemYMax,ElemZMin},	// FRD

	{ElemXMin,ElemYMin,ElemZMax},	// BLU
	{ElemXMax,ElemYMin,ElemZMax},	// BRU
	{ElemXMin,ElemYMax,ElemZMax},	// BLD
	{ElemXMax,ElemYMax,ElemZMax},	// BRD
};

/*****************************************************************************/
void	CModelGfx::RenderTile(DVECTOR &Pos,int TileID)
{
sElem3d	*ThisElem=&ElemBank[TileID];
u32		*XYList=(u32*)SCRATCH_RAM;
u32		*OutVtx=XYList;
VECTOR	*V0,*V1,*V2,*InVtx=VtxTable;
VECTOR	RenderPos;
MATRIX	Mtx;

		SetIdentNoTrans(&Mtx);
		RenderPos.vx=(INGAME_SCREENOFS_X)+Pos.vx;
		RenderPos.vy=(INGAME_SCREENOFS_Y)+Pos.vy;
		gte_SetRotMatrix(&Mtx);
		CMX_SetTransMtxXY(&RenderPos);

		V0=InVtx++; 
		V1=InVtx++; 
		V2=InVtx++;
		gte_ldv3(V0,V1,V2);
		for (int i=0; i<(int)((sizeof(VtxTable)/sizeof(VECTOR))+1); i++)
		{
			u32	*OutPtr;
			gte_rtpt(); // 22 cycles
			V0=InVtx++;
			V1=InVtx++;
			V2=InVtx++;
			gte_ldv3(V0,V1,V2);
			OutPtr=OutVtx;
			OutVtx+=3;
			gte_stsxy3c(OutPtr);	// read XY back
		}
		XYList+=8;
		RenderElem(ThisElem,Pos,0,0,0,XYList);
}

/*****************************************************************************/
void		CModelGfx::RenderElem(sElem3d *ThisElem,DVECTOR &Pos,SVECTOR *Angle,VECTOR *Scale,s32 ClipFlag,u32 *TransBuffer)
{
u8				*PrimPtr=GetPrimPtr();
u32				T0,T1,T2,T3;
s32				ClipZ;
sOT				*ThisOT;
VECTOR			RenderPos;
MATRIX			Mtx;
//u32	const		*XYList=(u32*)SCRATCH_RAM;
u8 const		*XYList=(u8*)SCRATCH_RAM;


// If has scale && angle then need to use PSX scale matrix, otherwise, force values
			if (Angle)
			{
				RotMatrix_gte(Angle,&Mtx);
				if (Scale)
				{
					ScaleMatrix(&Mtx,Scale);
				}
			}
			else
			{
				if (Scale)
				{
					SetIdentNoTrans(&Mtx,Scale);
				}
				else
				{
				SetIdentNoTrans(&Mtx);
				}
			}

		RenderPos.vx=(INGAME_SCREENOFS_X)+Pos.vx;
		RenderPos.vy=(INGAME_SCREENOFS_Y)+Pos.vy;
		gte_SetRotMatrix(&Mtx);
		CMX_SetTransMtxXY(&RenderPos);

// --- Cache Vtx ----------
		{
			int		Count=ThisElem->VtxTriCount;
			sVtx	*V0,*V1,*V2;
			u16		*IdxTable=&VtxIdxList[ThisElem->VtxIdxStart];

			V0=&VtxList[*IdxTable++];
			V1=&VtxList[*IdxTable++];
			V2=&VtxList[*IdxTable++];
			gte_ldv3(V0,V1,V2);

			while (Count--)
			{
				u32	*OutPtr;
				gte_rtpt_b(); // 22 cycles
		// Preload next (when able) - Must check this
				V0=&VtxList[*IdxTable++];
				V1=&VtxList[*IdxTable++];
				V2=&VtxList[*IdxTable++];
				OutPtr=TransBuffer;
				TransBuffer+=3;
				gte_ldv3(V0,V1,V2);
				gte_stsxy3c(OutPtr);	// read XY back
			}
		}

// --- Render Tri's -------------

		int		TriCount=ThisElem->TriCount;
		sTri	*TList=&TriList[ThisElem->TriStart];
		while (TriCount--)
		{
			POLY_GT3	*ThisPrim=(POLY_GT3*)PrimPtr;

			T0=*(u32*)(XYList+TList->P0); 
			T1=*(u32*)(XYList+TList->P1); 
			T2=*(u32*)(XYList+TList->P2);
			gte_ldsxy0(T0);
			gte_ldsxy1(T1);
			gte_ldsxy2(T2);

			setlen(ThisPrim, GPU_PolyGT3Tag);
			ThisPrim->code=TList->PolyCode;
			gte_nclip_b();	// 8 cycles

			setShadeTex(ThisPrim,1);

			*(u32*)&ThisPrim->x0=T0;	// Set XY0
			*(u32*)&ThisPrim->x1=T1;	// Set XY1
			*(u32*)&ThisPrim->x2=T2;	// Set XY2
			T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
			T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
			T2=*(u32*)&TList->uv2;		// Get UV2

			gte_stopz(&ClipZ);
			ThisOT=OtPtr+TList->OTOfs;
			ClipZ|=ClipFlag;			// <-- Evil!!
			TList++;
			if (ClipZ<0)
			{
				*(u32*)&ThisPrim->u0=T0;	// Set UV0
				*(u32*)&ThisPrim->u1=T1;	// Set UV1
				*(u32*)&ThisPrim->u2=T2;	// Set UV2
				addPrim(ThisOT,ThisPrim);
				PrimPtr+=sizeof(POLY_GT3);
			}
		}

// --- Render Quads -----------
		int		QuadCount=ThisElem->QuadCount;
		sQuad	*QList=&QuadList[ThisElem->QuadStart];
		while (QuadCount--)
		{
			POLY_GT4	*ThisPrim=(POLY_GT4*)PrimPtr;

			T0=*(u32*)(XYList+QList->P0); 
			T1=*(u32*)(XYList+QList->P1); 
			T2=*(u32*)(XYList+QList->P2);
			T3=*(u32*)(XYList+QList->P3);
			gte_ldsxy0(T0);
			gte_ldsxy1(T1);
			gte_ldsxy2(T2);
			
			setlen(ThisPrim, GPU_PolyGT4Tag);
			ThisPrim->code=QList->PolyCode;
			gte_nclip_b();	// 8 cycles

			setShadeTex(ThisPrim,1);

			*(u32*)&ThisPrim->x0=T0;	// Set XY0
			*(u32*)&ThisPrim->x1=T1;	// Set XY1
			*(u32*)&ThisPrim->x2=T2;	// Set XY2
			*(u32*)&ThisPrim->x3=T3;	// Set XY3
			T0=*(u32*)&QList->uv0;		// Get UV0 & TPage
			T1=*(u32*)&QList->uv1;		// Get UV1 & Clut
			T2=*(u32*)&QList->uv2;		// Get UV2
			T3=*(u32*)&QList->uv3;		// Get UV2
			gte_stopz(&ClipZ);
			ThisOT=OtPtr+QList->OTOfs;
			ClipZ|=ClipFlag;			// <-- Evil!!
			QList++;
			if (ClipZ<0)
			{
				*(u32*)&ThisPrim->u0=T0;	// Set UV0
				*(u32*)&ThisPrim->u1=T1;	// Set UV1
				*(u32*)&ThisPrim->u2=T2;	// Set UV2
				*(u32*)&ThisPrim->u3=T3;	// Set UV2
				addPrim(ThisOT,ThisPrim);
				PrimPtr+=sizeof(POLY_GT4);
			}
		}

		SetPrimPtr(PrimPtr);

}


/******************/
/*** Actor Bank ***/
/******************/

#include "system\global.h"
#include "mem\memory.h"
#include "fileio\fileio.h"
#include "utils\utils.h"
//#include "utils\pak.h"
#include "gfx\prim.h"
#include "gfx\actor.h"
#include "gfx\otpos.h"
#include "gfx\animtex.h"

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
//			DAVE_DBGMSG("Adjusted Slot Width from %i to %i\n",SlotList[Slot].Width,W);
			SlotList[Slot].Width=W; 
		}
		if (SlotList[Slot].Height<H) 
		{
//			DAVE_DBGMSG("Adjusted Slot Height from %i to %i\n",SlotList[Slot].Height,H);
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
		SlotList[Slot].RefCount++;

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
		SlotList[Slot].FrameCount+=FrameCount;
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
					int	V=Y*ThisSlot->Height;
					int	TexX=CACHE_X+CurrentTPX+(U>>2);
					int	TexY=CACHE_Y+V;

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
			SlotList[i].RefCount=0;
			SlotList[i].FrameCount=0;
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
			DrawSync(0);
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

// FixUp FrameList
		for (i=0; i<Spr->FrameCount; i++)
		{
			sSpriteFrameGfx	*ThisFrame=&Spr->FrameList[i];
			ThisFrame->PAKSpr=(u8*)				MakePtr(Spr,(int)ThisFrame->PAKSpr);
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

int		CountSlots(sPoolNode *Node)
{
int		Count=0;
			while (Node)
			{
				Count++;
				Node=Node->Next;
			}
	return(Count);

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
/*
		for (int i=0;i<CActorPool::Cache.GetSlotCount(); i++)
		{

			sPoolSlot	&Slot=CActorPool::Cache.GetSlot(i);
			int			Count=CountSlots(Slot.NodeList.Head);

			DAVE_DBGMSG("SC %i: %i %i\n",i,Slot.SlotCount,Count);
			ASSERT(Slot.SlotCount==Count);
		}
*/
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
		PoolEntry=ThisActor;
		ShadowXOfs=DEF_SHADOW_OFS;
		ShadowYOfs=DEF_SHADOW_OFS;
		ShadowFlag=false;
		OtPos=OTPOS__ACTOR_POS;
		
}

/*****************************************************************************/
CActorGfx::~CActorGfx()
{
}

/*****************************************************************************/
POLY_FT4	*CActorGfx::Render(DVECTOR &Pos,int Anim,int Frame,bool XFlip,bool YFlip)
{
sPoolNode		*ThisNode,*FindNode;
POLY_FT4		*Ft4;

// Calc Frame Ptrs
sSpriteAnimBank	*SpriteBank=PoolEntry->ActorGfx;
sSpriteAnim	*ThisAnim=SpriteBank->AnimList+Anim;

			CurrentFrame=&ThisAnim->Anim[Frame];
			CurrentFrameGfx=&SpriteBank->FrameList[CurrentFrame->FrameIdx];

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

			Ft4=GetPrimFT4();
			SetUpFT4(Ft4,ThisNode,Pos.vx,Pos.vy,XFlip,YFlip);
			setRGB0(Ft4,128,128,128);
			Ft4->tpage=ThisNode->TPage;
			Ft4->clut=PoolEntry->ActorGfx->Clut;
			addPrim(OtPtr+OtPos,Ft4);

			if (ShadowFlag)
			{
				POLY_FT4	*sFt4=GetPrimFT4();
				*sFt4=*Ft4;
				sFt4->x0-=ShadowXOfs;
				sFt4->x1-=ShadowXOfs;
				sFt4->y0+=ShadowYOfs;
				sFt4->y1+=ShadowYOfs;
				setSemiTrans(sFt4,1);
				setRGB0(sFt4,0,0,0);
				addPrim(OtPtr+OtPos,sFt4);
			}
// Set BBox
int			HalfW=CurrentFrameGfx->W>>1;

			BBox.XMin=-HalfW+BBOX_ADJ;
			BBox.XMax=+HalfW-BBOX_ADJ;
			BBox.YMin=-CurrentFrameGfx->H+BBOX_ADJ;
			BBox.YMax=0-BBOX_ADJ;

			return(Ft4);
}

/*****************************************************************************/
POLY_FT4	*CActorGfx::RotateScale(POLY_FT4 *Ft4,DVECTOR &Pos,int Angle,int XScale,int YScale)
{
int		dX,dY;
int		CosAngle,SinAngle;
int		CosX,CosY,SinX,SinY;
sBBox	SBox,CBox;


		Angle&=4095;	
	
		dX=(CurrentFrameGfx->W*XScale)>>(12+1);	// +1 for half
		dY=(CurrentFrameGfx->H*YScale)>>(12);	

		CosAngle=mcos(Angle);
		SinAngle=msin(Angle);

		SBox.XMin=(SinAngle*-dX)>>12;
		SBox.XMax=(SinAngle*+dX)>>12;
		SBox.YMin=(SinAngle*-dY)>>12;
		SBox.YMax=0;

		CBox.XMin=(CosAngle*-dX)>>12;
		CBox.XMax=(CosAngle*+dX)>>12;
		CBox.YMin=(CosAngle*-dY)>>12;
		CBox.YMax=0;

int		x0,x1,x2,x3;
int		y0,y1,y2,y3;
int		XMin,XMax;
int		YMin,YMax;

		x0=CBox.XMin-SBox.YMin; y0=SBox.XMin+CBox.YMin;
		x1=CBox.XMax-SBox.YMin; y1=SBox.XMax+CBox.YMin;
		x2=CBox.XMin+SBox.YMax; y2=SBox.XMin-CBox.YMax;
		x3=CBox.XMax+SBox.YMax; y3=SBox.XMax-CBox.YMax;

		XMin=x0;
		if (XMin>x1) XMin=x1;
		if (XMin>x2) XMin=x2;
		if (XMin>x3) XMin=x3;
		XMax=x0;
		if (XMax<x1) XMax=x1;
		if (XMax<x2) XMax=x2;
		if (XMax<x3) XMax=x3;
		YMin=y0;
		if (YMin>y1) YMin=y1;
		if (YMin>y2) YMin=y2;
		if (YMin>y3) YMin=y3;
		YMax=y0;
		if (YMax<y1) YMax=y1;
		if (YMax<y2) YMax=y2;
		if (YMax<y3) YMax=y3;

		BBox.XMin=XMin+BBOX_ADJ;
		BBox.XMax=XMax-BBOX_ADJ;
		BBox.YMin=YMin+BBOX_ADJ;
		BBox.YMax=YMax-BBOX_ADJ;

		Ft4->x0=Pos.vx+x0; Ft4->y0=Pos.vy+y0;
		Ft4->x1=Pos.vx+x1; Ft4->y1=Pos.vy+y1;
		Ft4->x2=Pos.vx+x2; Ft4->y2=Pos.vy+y2;
		Ft4->x3=Pos.vx+x3; Ft4->y3=Pos.vy+y3;

		return(Ft4);
}

/*****************************************************************************/
void	CActorGfx::SetUpFT4(POLY_FT4 *Ft4,sPoolNode *Node,int X,int Y,bool XFlip,bool YFlip)
{
u8		W=CurrentFrameGfx->W;
u8		H=CurrentFrameGfx->H;
u8		U=Node->U;
u8		V=Node->V;

		if (XFlip)
			{
			X-=CurrentFrame->XOfs;
			X-=W;
			Ft4->u0=U+W-1;
			Ft4->u1=U;//-1;
			Ft4->u2=U+W-1;
	 		Ft4->u3=U;//-1;

			}
		else
		{
			X+=CurrentFrame->XOfs;
			Ft4->u0=U;
			Ft4->u1=U+W-1;
			Ft4->u2=U;
 			Ft4->u3=U+W-1;

		}

		if (YFlip)
			{
			Y-=CurrentFrame->YOfs;
			Y-=H;
			Ft4->v0=V+H-1;
			Ft4->v1=V+H-1;
			Ft4->v2=V;//-1;
			Ft4->v3=V;//-1;
			}
		else
		{
			Y+=CurrentFrame->YOfs;
			Ft4->v0=V;
			Ft4->v1=V;
			Ft4->v2=V+H-1;
			Ft4->v3=V+H-1;
		}

		setXYWH(Ft4,X,Y,W,H);
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
sModel	*CModelGfx::ModelTable;
sTri	*CModelGfx::ModelTriList;
sQuad	*CModelGfx::ModelQuadList;
sVtx	*CModelGfx::ModelVtxList;

/*****************************************************************************/
void	CModelGfx::SetData(sModel *Table,sTri *TList,sQuad *QList,sVtx *VList)
{
		ModelTable=Table;
		ModelTriList=TList;
		ModelQuadList=QList;
		ModelVtxList=VList;

}

/*****************************************************************************/
void	CModelGfx::SetModel(int Type)
{
		Model=&CModelGfx::ModelTable[Type];
}

/*****************************************************************************/
const int	PXOfs=-16;
const int	PYOfs=-8;

void		CModelGfx::Render(DVECTOR &Pos,SVECTOR *Angle,VECTOR *Scale)
{
#define	BLOCK_MULT	16
u8				*PrimPtr=GetPrimPtr();
POLY_FT3		*TPrimPtr=(POLY_FT3*)PrimPtr;
sVtx			*P0,*P1,*P2;
u32				T0,T1,T2;
s32				ClipZ;
sOT				*ThisOT;
DVECTOR			MapXY;
VECTOR			RenderPos;
int				TriCount=Model->TriCount;				
sTri			*TList=&ModelTriList[Model->TriStart];
				MATRIX	Mtx;

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

			MapXY.vx=Pos.vx>>4;
			MapXY.vy=Pos.vy>>4;
			
int			ShiftX=(Pos.vx & 15);
int			ShiftY=(Pos.vy & 15);

			RenderPos.vx=(PXOfs*16)+((MapXY.vx*16)+ShiftX);
			RenderPos.vy=(PYOfs*16)+((MapXY.vy*16)+ShiftY);

		gte_SetRotMatrix(&Mtx);
		CMX_SetTransMtxXY(&RenderPos);

		while (TriCount--)
		{
			P0=&ModelVtxList[TList->P0]; P1=&ModelVtxList[TList->P1]; P2=&ModelVtxList[TList->P2];
			gte_ldv3(P0,P1,P2);
			setlen(TPrimPtr, GPU_PolyFT3Tag);
			TPrimPtr->code=TList->PolyCode;
			setRGB0(TPrimPtr,128,128,128);
			gte_rtpt_b();
	
			T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
			T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
			T2=*(u16*)&TList->uv2;		// Get UV2
			*(u32*)&TPrimPtr->u0=T0;	// Set UV0
			*(u32*)&TPrimPtr->u1=T1;	// Set UV1
			*(u16*)&TPrimPtr->u2=T2;	// Set UV2
/*
			if (TList->OTOfs>ActorOT)
			{
				ThisOT=OtPtr+(ActorOT+1);
			}
			else
			{
				ThisOT=OtPtr+(ActorOT-1);
			}
*/
			ThisOT=OtPtr+TList->OTOfs;


			TList++;
			addPrim(ThisOT,TPrimPtr);
			gte_stsxy3_ft3(TPrimPtr);
			TPrimPtr++;

/* Models are not clipped
			gte_nclip_b();
			gte_stsxy3_ft3(TPrimPtr);
			gte_stopz(&ClipZ);
			if (ClipZ<=0)
			{
				addPrim(ThisOT,TPrimPtr);
				TPrimPtr++;
			}
*/
		}

		SetPrimPtr((u8*)TPrimPtr);
}

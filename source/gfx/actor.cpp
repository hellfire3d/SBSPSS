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
CActorCache	CActorPool::Cache;
sActorPool	*CActorPool::ActorList,*CActorPool::LastActor;

u8			CActorGfx::UnpackBuffer[CActorCache::MAX_ACTOR_SIZE];

/*****************************************************************************/
/*** Cache *******************************************************************/
/*****************************************************************************/
CActorCache::CActorCache()
{
		for (int i=0;i<CACHE_TYPE_MAX; i++)
		{
			SlotList[i].NodeList.List=0;
		}
}

/*****************************************************************************/
CActorCache::~CActorCache()
{
}

/*****************************************************************************/
int		CActorCache::GetSizeType(int Size)
{
		if (Size<= 16) return(16);
		if (Size<= 32) return(32);
		if (Size<= 64) return(64);
		if (Size<=128) return(128);
		if (Size<=256) return(256);
		ASSERT(!"SPRITE SIZE NOT SUPPORTED");

		return(-1);
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
			ASSERT(SlotCount!=CACHE_TYPE_MAX);
			Slot=SlotCount;
			SlotList[Slot].Width=W;
			SlotList[Slot].Height=H;
			SlotCount++;
		}
		SlotList[Slot].RefCount++;

		return(Slot);
}

/*****************************************************************************/
sPoolNode *CActorCache::RemoveHeadNode(sNodeList *Root)
{
sPoolNode *Node=Root->List;
sPoolNode *Next=Node->Next;

		Root->List=Node->Next;
		Next->Prev=0;
		Node->Next=0;
		return(Node);
}

/*****************************************************************************/
void	CActorCache::RemoveNode(sPoolNode *Node,sNodeList *Root)
{
sPoolNode	*Prev=Node->Prev;
sPoolNode	*Next=Node->Next;

		if (Prev)
		{ // Not Head Node
			Prev->Next=Node->Next;
		}
		if (Next)
		{ // Not Tail Node
			Next->Prev=Node->Prev;
		}
}

/*****************************************************************************/
// Add node to end of list
void	CActorCache::AddNode(sPoolNode *Node,sNodeList *Root)
{
sPoolNode	*Prev=Root->LastNode;
sPoolNode	*Next=0;

		if (Prev)
		{ // Not Head Node
			Prev->Next=Node;
		}
		else
		{
			Root->List=Node;
		}

		Node->Prev=Prev;
		Node->Next=0;
		Root->LastNode=Node;
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
		
		for (int i=0; i<SlotCount; i++)
		{
//			printf("Slot %i: (%i) %i %i=%i\n",i,SlotList[i].RefCount,SlotList[i].Width,SlotList[i].Height,SlotList[i].FrameCount);
			InitCache(i,TPW);
		}
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
			
			List=(sPoolNode*)MemAlloc(Total*sizeof(sPoolNode),"CacheNodeList");

// Create List Entries
			for (int Y=0; Y<H; Y++)
			{
				for (int X=0; X<W; X++)
				{
					int	U=(X*ThisSlot->Width);
					int	V=Y*ThisSlot->Height;
					int	TexX=CACHE_X+CurrentTPX+(U>>2);
					int	TexY=CACHE_Y+V;

					List->Actor=0;
					List->TexX=TexX;
					List->TexY=TexY;
					List->U=U&255;
					List->V=V&255;
					List->TPage=getTPage(0,0,TexX,TexX);
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
			if (SlotList[i].NodeList.List) MemFree(SlotList[i].NodeList.List);
			SlotList[i].NodeList.List=0;
			SlotList[i].NodeList.LastNode=0;
			SlotList[i].RefCount=0;
			SlotList[i].FrameCount=0;
			SlotList[i].Width=0;
			SlotList[i].Height=0;
		}

// Init VRam Table
		for (int Y=0; Y<CACHE_TABLE_H; Y++)
		{
			for (int X=0; X<CACHE_TABLE_W; X++)
			{
				SlotTable[X][Y]=0;
			}
		}

		CurrentTPX=0;
		CurrentPalette=0;
		SlotCount=0;

// Clear VRam
RECT	R={512,256,512,250};
		ClearImage(&R,0,255,0);
}

/*****************************************************************************/
void	CActorCache::LoadPalette(sActorPool *Actor)
{
RECT	R;
int		X=CurrentPalette%(((CACHE_W*TPAGE_W)/CACHE_PALW));
int		Y=CurrentPalette/(((CACHE_W*TPAGE_W)/CACHE_PALW)-1);

// Cheap bodge at mo
		if (Actor->Filename==ACTORS_SPONGEBOB_SBK)
		{
			R.x=512;
			R.y=511;
		}
		else
		{
			R.x=CACHE_PALX+(CurrentPalette*CACHE_PALW);
			R.y=CACHE_PALY-Y;
		}

		R.w=CACHE_PALW;
		R.h=CACHE_PALH;
		DrawSync(0);
		LoadImage( &R, (u32*)Actor->ActorGfx->Palette);

		Actor->ActorGfx->Clut=getClut(R.x,R.y);
		CurrentPalette++;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CActorPool::Init()
{
		Cache.Reset();
		ActorList=0;
		LastActor=0;
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
				List->CachePool=Cache.GetSlotList(List->CacheSlot);
				List->ThisCache.List=0;
				List->ThisCache.LastNode=0;
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

sSpriteAnimBank	*Spr=(sSpriteAnimBank*)CFileIO::loadFile(Filename);

//		printf("Add Actor %i\n",(int)Filename);
		Spr->AnimList=(sSpriteAnim*)	MakePtr(Spr,(int)Spr->AnimList);
		Spr->FrameList=(sSpriteFrame*)	MakePtr(Spr,(int)Spr->FrameList);
		Spr->Palette=(u8*)				MakePtr(Spr,(int)Spr->Palette);

// FixUp AnimList
		for (i=0; i<Spr->AnimCount; i++)
		{
			sSpriteAnim	*ThisAnim=&Spr->AnimList[i];
			ThisAnim->Anim=(u16*)	MakePtr(Spr,(int)ThisAnim->Anim);
			TotalFrames+=ThisAnim->FrameCount;
		}

// FixUp FrameList
		for (i=0; i<Spr->FrameCount; i++)
		{
			sSpriteFrame	*ThisFrame=&Spr->FrameList[i];
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
}

/*****************************************************************************/
CActorGfx::~CActorGfx()
{
}

/*****************************************************************************/
sSpriteFrame	*CActorGfx::GetFrame(int Anim,int Frame)
{

sSpriteAnim		*ThisAnim=PoolEntry->ActorGfx->AnimList+Anim;
u16				ThisFrame=ThisAnim->Anim[Frame];
	
				return(PoolEntry->ActorGfx->FrameList+ThisFrame);
}

/*****************************************************************************/
int				CActorGfx::GetTotalFrameCount()
{
int		Total=0;
		for (int Anim=0; Anim<PoolEntry->ActorGfx->AnimCount; Anim++)
		{
			sSpriteAnim		*ThisAnim=PoolEntry->ActorGfx->AnimList+Anim;
			Total+=ThisAnim->FrameCount;
		}

		return(Total);
}

/*****************************************************************************/
int	ActorOT=10;

POLY_FT4	*CActorGfx::Render(DVECTOR &Pos,int Anim,int Frame,bool XFlip,bool YFlip,bool Shadow)
{
sSpriteFrame	*FrameGfx=GetFrame(Anim,Frame);
sPoolNode		*ThisNode;;
POLY_FT4		*Ft4;

// Is cached?
//			if(this->PoolEntry->Filename==ACTORS_SPONGEBOB_SBK) return(0);

			ThisNode=PoolEntry->ThisCache.List;
			while (ThisNode)
			{
				if (ThisNode->Anim==Anim && ThisNode->Frame==Frame) break;
				ThisNode=ThisNode->Next;
			}

			if (!ThisNode)
			{ // Not cached frame
				ThisNode=CActorCache::RemoveHeadNode(PoolEntry->CachePool);
				CActorCache::AddNode(ThisNode,&PoolEntry->ThisCache);
				RECT	R;

				ThisNode->Anim=Anim;
				ThisNode->Frame=Frame;

				PAK_doUnpak(UnpackBuffer,FrameGfx->PAKSpr);
				R.x=ThisNode->TexX;
				R.y=ThisNode->TexY;
				R.w=FrameGfx->W>>2;	// div 4 cos 16 color
				R.h=FrameGfx->H;
				LoadImage( &R, (u32*)UnpackBuffer);
			}

			Ft4=GetPrimFT4();
			SetUpFT4(Ft4,FrameGfx,ThisNode,Pos.vx,Pos.vy,XFlip,YFlip);
			setRGB0(Ft4,128,128,128);
			Ft4->tpage=ThisNode->TPage;
			Ft4->clut=PoolEntry->ActorGfx->Clut;
			AddPrimToList(Ft4,ActorOT);

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
				AddPrimToList(sFt4,ActorOT);
			}

			return(Ft4);
}

/*****************************************************************************/
void	CActorGfx::SetUpFT4(POLY_FT4 *Ft4,sSpriteFrame *Frame,sPoolNode *Node,int X,int Y,bool XFlip,bool YFlip)
{
u8		W=Frame->W;
u8		H=Frame->H;
u8		U=Node->U;
u8		V=Node->V;

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
			Ft4->u1=U+W-1;
			Ft4->u2=U;
 			Ft4->u3=U+W-1;

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
int	DX=1;
int	DY=1;
int	PXOfs=-16;
int	PYOfs=-6;
void		CModelGfx::Render(DVECTOR &Pos)
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

			MapXY.vx=Pos.vx>>4;
			MapXY.vy=Pos.vy>>4;
			
int			ShiftX=(Pos.vx & 15);
int			ShiftY=(Pos.vy & 15);

			RenderPos.vx=(PXOfs*16)+((MapXY.vx*16)+ShiftX);
			RenderPos.vy=(PYOfs*16)+((MapXY.vy*16)+ShiftY);

		CMX_SetTransMtxXY(&RenderPos);

		while (TriCount--)
		{
			P0=&ModelVtxList[TList->P0]; P1=&ModelVtxList[TList->P1]; P2=&ModelVtxList[TList->P2];
			gte_ldv3(P0,P1,P2);
			setPolyFT3(TPrimPtr);
			setShadeTex(TPrimPtr,1);
			setlen(TPrimPtr, GPU_PolyFT3Tag);
			gte_rtpt_b();
	
			T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
			T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
			T2=*(u16*)&TList->uv2;		// Get UV2
			*(u32*)&TPrimPtr->u0=T0;	// Set UV0
			*(u32*)&TPrimPtr->u1=T1;	// Set UV1
			*(u16*)&TPrimPtr->u2=T2;	// Set UV2
			if (TList->OTOfs>MAX_OT-1) TList->OTOfs=MAX_OT-1;
			ThisOT=OtPtr+TList->OTOfs;

			TList++;
			gte_nclip_b();
			gte_stsxy3_ft3(TPrimPtr);
			gte_stopz(&ClipZ);
			if (ClipZ<=0)
			{
				addPrim(ThisOT,TPrimPtr);
				TPrimPtr++;
			}
		}

		SetPrimPtr((u8*)TPrimPtr);
}

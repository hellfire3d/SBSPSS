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

u8			*CActorCache::UnpackBuffer;

/*****************************************************************************/
/*** Cache *******************************************************************/
/*****************************************************************************/
CActorCache::CActorCache()
{
		for (int i=0;i<CACHE_TYPE_MAX; i++)
		{
			SlotList[i].ListMem=0;
		}
		UnpackBuffer=0;

}

/*****************************************************************************/
CActorCache::~CActorCache()
{
}

/*****************************************************************************/
int		CActorCache::GetSizeType(int Size)
{
//		if (Size<= 16) return(16);
//		if (Size<= 32) return(32);
//		if (Size<= 64) return(64);
//		if (Size<=128) return(128);
//		if (Size<=256) return(256);
		return((Size+15)&-16);
//	Size>>=4;
//	Size<<=4;
//		return(Size);
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
sPoolNode *Node=Root->Head;
sPoolNode *Next=Node->Next;

		Root->Head=Node->Next;
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
sPoolNode	*Prev=Root->Tail;
sPoolNode	*Next=0;

		if (Prev)
		{ // Not Head Node
			Prev->Next=Node;
		}
		else
		{
			Root->Head=Node;
		}

		Node->Prev=Prev;
		Node->Next=0;
		Root->Tail=Node;
}

/*****************************************************************************/
// Add node list to end of list
void	CActorCache::AddNodeList(sNodeList *Src,sNodeList *Dst)
{
sPoolNode	*SrcHead=Src->Head;
sPoolNode	*SrcTail=Src->Tail;
sPoolNode	*DstHead=Dst->Head;
sPoolNode	*DstTail=Dst->Tail;

		if (!SrcHead) return;

		if (!DstTail)
		{
			Dst->Head=SrcHead;
			SrcHead->Prev=0;
		}
		else
		{
			DstTail->Next=SrcHead;
			SrcHead->Prev=DstTail;
		}

		SrcTail->Next=0;
		Dst->Tail=SrcTail;
		Src->Head=0;
		Src->Tail=0;
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

/*		if (TPW<1) */TPW=1;
		ASSERT(SlotCount<CACHE_W);

		for (int i=0; i<SlotCount; i++)
		{
			if (MaxW<SlotList[i].Width)		MaxW=SlotList[i].Width;
			if (MaxH<SlotList[i].Height)	MaxH=SlotList[i].Height;
			InitCache(i,TPW);
		}
		UnpackBuffer=(u8*)MemAlloc(MaxW*MaxH,"UnpackBuffer");
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

					List->Actor=0;
					List->TexX=TexX;
					List->TexY=TexY;
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
		if (UnpackBuffer) MemFree(UnpackBuffer);
		UnpackBuffer=0;

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
		{ // Cheap bodge at mo
			RECT	R;
			int		X=CurrentPalette%(((CACHE_W*TPAGE_W)/CACHE_PALW));
			int		Y=CurrentPalette/(((CACHE_W*TPAGE_W)/CACHE_PALW)-1);

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
				List->PoolCache=Cache.GetSlotList(List->CacheSlot);
				List->ActorCache.Head=0;
				List->ActorCache.Tail=0;
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
void	CActorPool::CleanUpCache()
{
sActorPool	*Actor=ActorList;
			
			while (Actor) 
			{
				CActorCache::AddNodeList(&Actor->ActorCache ,Actor->PoolCache);
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
		PoolEntry=ThisActor;
		ShadowXOfs=DEF_SHADOW_OFS;
		ShadowYOfs=DEF_SHADOW_OFS;
		ShadowFlag=false;
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
int	ActorOT=10;

POLY_FT4	*CActorGfx::Render(DVECTOR &Pos,int Anim,int Frame,bool XFlip,bool YFlip)
{

sPoolNode		*ThisNode;;
POLY_FT4		*Ft4;

			CurrentFrame=GetFrame(Anim,Frame);
// Is cached?
			ThisNode=PoolEntry->ActorCache.Head;
			while (ThisNode)
			{ // Try local Cache (From Head forward)
				if (ThisNode->Actor==PoolEntry->Filename && ThisNode->Anim==Anim && ThisNode->Frame==Frame) break;
				ThisNode=ThisNode->Next;
			}
/*			
			if (!ThisNode)
			{ // Try main cache ( from tail back)
			
				ThisNode=PoolEntry->PoolCache->Tail;
				while (ThisNode)
				{
					if (ThisNode->Actor==PoolEntry->Filename && ThisNode->Anim==Anim && ThisNode->Frame==Frame) break;
					if (ThisNode->Prev==PoolEntry->PoolCache->Tail) ThisNode->Prev=0;
					ThisNode=ThisNode->Prev;

				}
				if (ThisNode)
				{
					CActorCache::RemoveNode(ThisNode,PoolEntry->PoolCache);
					CActorCache::AddNode(ThisNode,&PoolEntry->ActorCache);
				}
			}
			else
			{
			}
*/
			if (!ThisNode)
			{ // Not cached frame
				ThisNode=CActorCache::RemoveHeadNode(PoolEntry->PoolCache);
				CActorCache::AddNode(ThisNode,&PoolEntry->ActorCache);
				RECT	R;
				ASSERT(ThisNode);

				ThisNode->Actor=PoolEntry->Filename;
				ThisNode->Anim=Anim;
				ThisNode->Frame=Frame;

				PAK_doUnpak(CActorCache::UnpackBuffer,CurrentFrame->PAKSpr);
				R.x=ThisNode->TexX;
				R.y=ThisNode->TexY;
				R.w=CurrentFrame->W>>2;	// div 4 cos 16 color
				R.h=CurrentFrame->H;
				LoadImage( &R, (u32*)CActorCache::UnpackBuffer);
			}

			Ft4=GetPrimFT4();
			SetUpFT4(Ft4,CurrentFrame,ThisNode,Pos.vx,Pos.vy,XFlip,YFlip);
			setRGB0(Ft4,128,128,128);
			Ft4->tpage=ThisNode->TPage;
			Ft4->clut=PoolEntry->ActorGfx->Clut;
			AddPrimToList(Ft4,ActorOT);

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
				AddPrimToList(sFt4,ActorOT);
			}
// Set BBox
int			HalfW=CurrentFrame->W>>1;

			BBox.XMin=-HalfW;
			BBox.XMax=+HalfW;
			BBox.YMin=-CurrentFrame->H;
			BBox.YMax=0;

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
	
		dX=(CurrentFrame->W*XScale)>>(12+1);	// +1 for half
		dY=(CurrentFrame->H*YScale)>>(12);	

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

		BBox.XMin=XMin;
		BBox.XMax=XMax;
		BBox.YMin=YMin;
		BBox.YMax=YMax;

		Ft4->x0=Pos.vx+x0; Ft4->y0=Pos.vy+y0;
		Ft4->x1=Pos.vx+x1; Ft4->y1=Pos.vy+y1;
		Ft4->x2=Pos.vx+x2; Ft4->y2=Pos.vy+y2;
		Ft4->x3=Pos.vx+x3; Ft4->y3=Pos.vy+y3;

//		Ft4->x0=Pos.vx+CBox.XMin-SBox.YMin; Ft4->y0=Pos.vy+SBox.XMin+CBox.YMin;
//		Ft4->x1=Pos.vx+CBox.XMax-SBox.YMin; Ft4->y1=Pos.vy+SBox.XMax+CBox.YMin;
//		Ft4->x2=Pos.vx+CBox.XMin+SBox.YMax; Ft4->y2=Pos.vy+SBox.XMin-CBox.YMax;
//		Ft4->x3=Pos.vx+CBox.XMax+SBox.YMax; Ft4->y3=Pos.vy+SBox.XMax-CBox.YMax;

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
//int	DX=1;
//int	DY=1;
const int	PXOfs=-16;
const int	PYOfs=-6;

void		CModelGfx::Render(DVECTOR &Pos,SVECTOR *Angle=0,VECTOR *Scale=0)
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

			SetIdentNoTrans(&Mtx);
			if (Scale || Angle)
			{
				if (Angle) RotMatrix(Angle,&Mtx);
				if (Scale) ScaleMatrix(&Mtx,Scale);
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

/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"game\game.h"

#include	"LayerTile.h"
#include	"LayerTile3d.h"


#if		defined(__USER_sbart__) || defined(__USER_daveo__)
#define	_SHOW_POLYZ_	1
#include	"gfx\font.h"	
static		FontBank		*Font;
#endif


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile3d::CLayerTile3d(sLevelHdr *LevelHdr,sLayerHdr *Hdr) : CLayerTile(LevelHdr,Hdr)
{
		TileBank3d=LevelHdr->TileBank3d;
		TriList=LevelHdr->TriList;
		QuadList=LevelHdr->QuadList;
		VtxList=LevelHdr->VtxList;

#if		defined(_SHOW_POLYZ_)
		Font=new ("PrimFont") FontBank;
		Font->initialise( &standardFont );
		Font->setOt( 0 );
		Font->setTrans(1);
#endif
}

/*****************************************************************************/
CLayerTile3d::~CLayerTile3d()
{
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

void	CLayerTile3d::init(DVECTOR &MapPos,int Shift)
{
		CLayerTile::init(MapPos,Shift);
}

/*****************************************************************************/
void	CLayerTile3d::shutdown()
{
#if		defined(_SHOW_POLYZ_)
		Font->dump();
		delete Font;
#endif
}

/*****************************************************************************/
void	CLayerTile3d::think(DVECTOR &MapPos)
{
			MapXY.vx=MapPos.vx>>4;
			MapXY.vy=MapPos.vy>>4;
			
//			MapXY.vx-=SCREEN_TILE_ADJ_LEFT;
//			MapXY.vy-=SCREEN_TILE_ADJ_UP;

			ShiftX=(MapPos.vx & 15);
			ShiftY=(MapPos.vy & 15);

			RenderOfs.vx=RenderOfs.vy=0;
			if (MapXY.vx<0) 
			{
				RenderOfs.vx=-MapXY.vx*BLOCK_MULT;
				MapXY.vx=0;
			}
			if (MapXY.vy<0) 
			{
				RenderOfs.vy=-MapXY.vy*BLOCK_MULT;
				MapXY.vy=0;
			}

			if (MapXY.vx+SCREEN_TILE_WIDTH3D<=MapWidth)
				RenderW=SCREEN_TILE_WIDTH3D;
			else
				RenderW=MapWidth-MapXY.vx;

			if (MapXY.vy+SCREEN_TILE_HEIGHT3D<=MapHeight)
				RenderH=SCREEN_TILE_HEIGHT3D;
			else
				RenderH=MapHeight-MapXY.vy;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if	1
VECTOR	asd={0,0,0};
int	ZP=0;
sVtx	VO={0,0,0};
void	CLayerTile3d::render()
{
const int	XOfs=-(BLOCK_MULT*15)-(SCREEN_TILE_ADJ_LEFT*BLOCK_MULT)-SCREEN_TILE_ADJ_LEFT;
const int	YOfs=-(BLOCK_MULT*7)-(SCREEN_TILE_ADJ_UP*BLOCK_MULT)-SCREEN_TILE_ADJ_UP;

sTileMapElem	*MapPtr=GetMapPos();
u8				*PrimPtr=GetPrimPtr();
POLY_FT3		*TPrimPtr=(POLY_FT3*)PrimPtr;
sVtx			*P0,*P1,*P2;
u32				T0,T1,T2;
s32				ClipZ;
sOT				*ThisOT;
MATRIX			&CamMtx=CGameScene::GetCamMtx();
VECTOR			BlkPos;

		CamMtx.t[0]=0;
		CamMtx.t[1]=0;
		CamMtx.t[2]=ZP;
		SetIdentNoTrans(&CamMtx);
		SetRotMatrix(&CamMtx);
		SetTransMatrix(&CamMtx);

// Setup Trans Matrix
		BlkPos.vx=XOfs-(ShiftX);
		BlkPos.vy=YOfs-(ShiftY);
		BlkPos.vx-=asd.vx;
		BlkPos.vy-=asd.vy;


		for (int Y=0; Y<RenderH; Y++)
		{
			sTileMapElem	*MapRow=MapPtr;
			s32				BlkXOld=BlkPos.vx;

			for (int X=0; X<RenderW; X++)
			{
				sTile3d		*Tile=&TileBank3d[MapRow->Tile];
				int			TriCount=Tile->TriCount;				
				sTri		*TList=&TriList[Tile->TriStart];

				while (TriCount--)	// Blank tiles rejected here (as no tri-count)
				{
					sVtx	_P0,_P1,_P2;
					P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
					_P0.vx=VtxList[TList->P0].vx+VO.vx; _P0.vy=VtxList[TList->P0].vy+VO.vy; _P0.vz=VtxList[TList->P0].vz+VO.vz; 
					_P1.vx=VtxList[TList->P1].vx+VO.vx; _P1.vy=VtxList[TList->P1].vy+VO.vy; _P1.vz=VtxList[TList->P1].vz+VO.vz; 
					_P2.vx=VtxList[TList->P2].vx+VO.vx; _P2.vy=VtxList[TList->P2].vy+VO.vy; _P2.vz=VtxList[TList->P2].vz+VO.vz; 
					P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
					P0=&_P0;
					P1=&_P1;
					P2=&_P2;
					CMX_SetTransMtxXY(&BlkPos);
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
//					gte_nclip_b();
					gte_stsxy3_ft3(TPrimPtr);
//					gte_stopz(&ClipZ);
//					if (ClipZ<=0)
					{
						addPrim(ThisOT,TPrimPtr);
//						addPrimNoCheck(OtPtr,TPrimPtr);
						TPrimPtr++;
					}
				}
				MapRow++;
				BlkPos.vx+=BLOCK_MULT;
			}
			MapPtr+=MapWidth;
			BlkPos.vx=BlkXOld;
			BlkPos.vy+=BLOCK_MULT;
		}

		SetPrimPtr((u8*)TPrimPtr);

#if		defined(_SHOW_POLYZ_)
char	Txt[256];
int		TCount=((u8*)TPrimPtr-PrimPtr)/sizeof(POLY_FT3);
int		QCount=0;
		sprintf(Txt,"TC %i\nQC %i",TCount,QCount);
		Font->print( 128, 32, Txt);
#endif

}

#else
void	CLayerTile3d::render()
{
/*
const int	XOfs=-(BLOCK_MULT*15)-(SCREEN_TILE_ADJ_LEFT*BLOCK_MULT);
const int	YOfs=-(BLOCK_MULT*7)-(SCREEN_TILE_ADJ_UP*BLOCK_MULT);

sTileMapElem	*MapPtr=GetMapPos();
u8				*PrimPtr=GetPrimPtr();
POLY_FT3		*TPrimPtr=(POLY_FT3*)PrimPtr;
VECTOR			BlkPos;
//sOT				*ThisOT=OtPtr+LayerOT;
sVtx			*P0,*P1,*P2;
u32				T0,T1,T2;
s32				ClipZ;


// Setup Trans Matrix
		BlkPos.vx=XOfs-((MapXY.vx*BLOCK_MULT)+ShiftX);
		BlkPos.vy=YOfs-((MapXY.vy*BLOCK_MULT)+ShiftY);
		BlkPos.vx+=RenderOfs.vx;
		BlkPos.vy+=RenderOfs.vy;
		CMX_SetTransMtxXY(&BlkPos);

		for (int Y=0; Y<RenderH; Y++)
		{
			// Get strip render size
			int		TriStart=MapPtr[0];
			int		TriEnd=MapPtr[RenderW];
			int		TriCount=TriEnd-TriStart;	

			sTri	*TList=&TriList[TriStart];
			while (TriCount>0)
			{
				P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
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

				int	ZP=P0->vz>>5;
				if (ZP<-4) ZP=-4;
				if (ZP>8) ZP=8;
sOT				*ThisOT=OtPtr+(LayerOT+ZP);

				TList++;
				gte_nclip_b();
				gte_stsxy3_ft3(TPrimPtr);
				gte_stopz(&ClipZ);
				if (ClipZ<=0)
				{
					addPrimNoCheck(ThisOT,TPrimPtr);
//					addPrim(ThisOT,TPrimPtr);
					TPrimPtr++;
				}

				TriCount--;
			}
			MapPtr+=MapWidth;
		}

int		PolyCount=((u8*)TPrimPtr-PrimPtr)/sizeof(POLY_FT3);

		SetPrimPtr((u8*)TPrimPtr);

#if		defined(_SHOW_POLYZ_)
char	Txt[256];
sprintf(Txt,"Poly Count=%i",PolyCount);
		Font->print( 32, 32, Txt);
#endif
*/
}

#endif
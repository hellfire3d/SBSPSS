/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#include 	"system\global.h"
#include	"system\vid.h"
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

static const int	BLOCK_SIZE				=16;
static const int	SCREEN_TILE_ADJ_U		=2;
static const int	SCREEN_TILE_ADJ_D		=1;
static const int	SCREEN_TILE_ADJ_L		=2;
static const int	SCREEN_TILE_ADJ_R		=3;

static const int	SCREEN_TILE3D_WIDTH		=(INGAME_SCREENW/BLOCK_SIZE)+SCREEN_TILE_ADJ_L+SCREEN_TILE_ADJ_R;
static const int	SCREEN_TILE3D_HEIGHT	=(INGAME_SCREENH/BLOCK_SIZE)+SCREEN_TILE_ADJ_U+SCREEN_TILE_ADJ_D;

static const int	RENDER_X_OFS			=INGAME_SCREENOFS_X-(SCREEN_TILE_ADJ_L*BLOCK_SIZE)+INGAME_RENDER_OFS_X;
static const int	RENDER_Y_OFS			=INGAME_SCREENOFS_Y-(SCREEN_TILE_ADJ_U*BLOCK_SIZE)+INGAME_RENDER_OFS_Y;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile3d::CLayerTile3d(sLevelHdr *LevelHdr,sLayerHdr *Hdr) : CLayerTile(LevelHdr,Hdr)
{
		ElemBank3d=LevelHdr->ElemBank3d;
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
			
			MapXY.vx-=SCREEN_TILE_ADJ_L;
			MapXY.vy-=SCREEN_TILE_ADJ_U;

			ShiftX=(MapPos.vx & 15);
			ShiftY=(MapPos.vy & 15);

			RenderOfs.vx=RenderOfs.vy=0;
			if (MapXY.vx<0) 
			{
				RenderOfs.vx=-MapXY.vx*BLOCK_SIZE;
				MapXY.vx=0;
			}
			if (MapXY.vy<0) 
			{
				RenderOfs.vy=-MapXY.vy*BLOCK_SIZE;
				MapXY.vy=0;
			}

			if (MapXY.vx+SCREEN_TILE3D_WIDTH<=MapWidth)
				RenderW=SCREEN_TILE3D_WIDTH;
			else
				RenderW=MapWidth-MapXY.vx;

			if (MapXY.vy+SCREEN_TILE3D_HEIGHT<=MapHeight)
				RenderH=SCREEN_TILE3D_HEIGHT;
			else
				RenderH=MapHeight-MapXY.vy;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

sFlipTable	FlipTable[4]=
{
	{{+4096,0,+4096,0},0<<31},	//00 <0
	{{-4096,0,+4096,0},1<<31},	//01 >0
	{{+4096,0,-4096,0},1<<31},	//10 >0
	{{-4096,0,-4096,0},0<<31}	//11 <0
};

void	CLayerTile3d::render()
{
sTileMapElem	*MapPtr=GetMapPos();
u8				*PrimPtr=GetPrimPtr();
POLY_FT3		*TPrimPtr=(POLY_FT3*)PrimPtr;
sVtx			*P0,*P1,*P2;
u32				T0,T1,T2;
s32				ClipZ;
sOT				*ThisOT;
VECTOR			BlkPos;

// Setup Trans Matrix
		BlkPos.vx=RENDER_X_OFS-(ShiftX)+RenderOfs.vx;
		BlkPos.vy=RENDER_Y_OFS-(ShiftY)+RenderOfs.vy;

		for (int Y=0; Y<RenderH; Y++)
		{
			sTileMapElem	*MapRow=MapPtr;
			s32				BlkXOld=BlkPos.vx;

			for (int X=0; X<RenderW; X++)
			{
				u16			Tile=MapRow->Tile;
				u16			TileIdx=Tile>>2;
				u16			Flip=Tile&3;
				sFlipTable	*FTab=&FlipTable[Flip];
				sElem3d		*Elem=&ElemBank3d[TileIdx];
				int			TriCount=Elem->TriCount;				
				sTri		*TList=&TriList[Elem->TriStart];

				P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
				CMX_SetTransMtxXY(&BlkPos);
				CMX_SetRotMatrixXY(&FTab->Mtx);
				while (TriCount--)	// Blank tiles rejected here (as no tri-count)
				{
					gte_ldv3(P0,P1,P2);
					setlen(TPrimPtr, GPU_PolyFT3Tag);
					TPrimPtr->code=TList->PolyCode;
					gte_rtpt_b();
//					setShadeTex(TPrimPtr,1);
			
					T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
					T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
					T2=*(u16*)&TList->uv2;		// Get UV2
					*(u32*)&TPrimPtr->u0=T0;	// Set UV0
					*(u32*)&TPrimPtr->u1=T1;	// Set UV1
					*(u16*)&TPrimPtr->u2=T2;	// Set UV2
extern u8 GlobalRGB[];
					setRGB0(TPrimPtr,GlobalRGB[1],GlobalRGB[1],GlobalRGB[1]);

					ThisOT=OtPtr+TList->OTOfs;
					TList++;
					P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];	// Pre-fetch next Tri
					gte_nclip_b();
					gte_stsxy3_ft3(TPrimPtr);
					gte_stopz(&ClipZ);
					ClipZ^=FTab->ClipCode;
					if (ClipZ<0)
					{
						addPrim(ThisOT,TPrimPtr);
						TPrimPtr++;
					}
				}
				MapRow++;
				BlkPos.vx+=BLOCK_SIZE;
			}
			MapPtr+=MapWidth;
			BlkPos.vx=BlkXOld;
			BlkPos.vy+=BLOCK_SIZE;
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

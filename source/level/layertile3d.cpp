/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"


#include	"LayerTile.h"
#include	"LayerTile3d.h"

#include "gfx\font.h"	

#if		defined(__USER_art__) || defined(__USER_daveo__)
#define	_SHOW_POLYZ_	1
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile3d::CLayerTile3d(sLayerHdr *Hdr,sTile *TileBank) : CLayerTile(Hdr,TileBank)
{
	
	LayerInfo=(sLayer3d*)MakePtr(Hdr,sizeof(sLayerHdr));

	TriList=(sTri*)		MakePtr(Hdr,LayerInfo->TriList);
	QuadList=(sQuad*)	MakePtr(Hdr,LayerInfo->QuadList);
	VtxList=(sVtx*)		MakePtr(Hdr,LayerInfo->VtxList);

	Map=(sTileMapElem*)MakePtr(Hdr,sizeof(sLayerHdr)+sizeof(sLayer3d));

	Font=0;
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
	if (Font) 
	{
		Font->dump();
		delete Font;
	}
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
}

int	GHV=256;

/*****************************************************************************/

void	CLayerTile3d::think(DVECTOR &MapPos)
{
int			XPos=MapPos.vx>>MapXYShift;
int			YPos=MapPos.vy>>MapXYShift;

			MapXY.vx=XPos>>4;
			MapXY.vy=YPos>>4;
			
			MapXY.vx-=SCREEN_TILE_ADJ_LEFT;
			MapXY.vy-=SCREEN_TILE_ADJ_UP;

			ShiftX=XPos & 15;
			ShiftY=YPos & 15;

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

		SetGeomScreen(GHV);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile3d::render()
{
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

}

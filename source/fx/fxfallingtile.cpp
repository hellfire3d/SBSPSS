/********************/
/*** Falling Tile ***/
/********************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"

#include	"FX\FXfallingTile.h"


/*****************************************************************************/
int		TT=1;
int		FallingTile_DefVY=-2;
int		FallingTile_DefLife=32;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXFallingTile::init(DVECTOR const &_Pos)
{
		CFX::init();
sLevelHdr	*LevelHdr=CLevel::getLevelHdr();

		TileBank3d=LevelHdr->TileBank3d;
		TriList=LevelHdr->TriList;
		QuadList=LevelHdr->QuadList;
		VtxList=LevelHdr->VtxList;
		Pos=_Pos;

		Velocity.vx=getRndRange(7)-4;
		Velocity.vy=FallingTile_DefVY;
		Life=FallingTile_DefLife;
		Tile=TT++;

}

/*****************************************************************************/
void	CFXFallingTile::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXFallingTile::think(int _frames)
{
		CFX::think(_frames);
		Pos.vx+=Velocity.vx;
		Pos.vy+=Velocity.vy;
		Velocity.vy++;
		Life--;
		if (!Life) setToShutdown();
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
const int	PXOfs=-16*16;	// Must sort these out to be single global!
const int	PYOfs=-8*16;

void	CFXFallingTile::render()
{
		CFX::render();

		if (!canRender() && Life) return;

u8			*PrimPtr=GetPrimPtr();
POLY_FT3	*TPrimPtr=(POLY_FT3*)PrimPtr;
sVtx		*P0,*P1,*P2;
u32			T0,T1,T2;
s32			ClipZ;
sOT			*ThisOT;
MATRIX		Mtx;
DVECTOR		&RenderPos=getRenderPos();
VECTOR		ThisRenderPos;

			SetIdentNoTrans(&Mtx);

			ThisRenderPos.vx=PXOfs+RenderPos.vx;
			ThisRenderPos.vy=PYOfs+RenderPos.vy;

			gte_SetRotMatrix(&Mtx);
			CMX_SetTransMtxXY(&ThisRenderPos);

			sTile3d		*ThisTile=&TileBank3d[Tile];
			int			TriCount=ThisTile->TriCount;
			sTri		*TList=&TriList[ThisTile->TriStart];

			while (TriCount--)
			{
				P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
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

				ThisOT=OtPtr;
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

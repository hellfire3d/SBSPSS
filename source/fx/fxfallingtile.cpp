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
#include	"level\layertile3d.h"

#include	"FX\FXfallingTile.h"


/*****************************************************************************/
const int		FallingTile_DefVY=-2;
const int		FallingTile_DefLife=64;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXFallingTile::init(DVECTOR const &_Pos)
{
		CFX::init();
sLevelHdr	*LevelHdr=CLevel::getLevelHdr();

		ElemBank3d=LevelHdr->ElemBank3d;
		TriList=LevelHdr->TriList;
		QuadList=LevelHdr->QuadList;
		VtxList=LevelHdr->VtxList;
		Pos=_Pos;

		Velocity.vx=getRndRange(7)-4;
		Velocity.vy=FallingTile_DefVY;
		Life=FallingTile_DefLife;
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
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
const int	PXOfs=-16*16;	// Must sort these out to be single global!
const int	PYOfs=-8*16;

void	CFXFallingTile::render()
{
		CFX::render();

		if (!canRender()) return;

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

			u16			TileIdx=Tile>>2;
			u16			Flip=Tile&3;
			sFlipTable	*FTab=&FlipTable[Flip];
			sElem3d		*Elem=&ElemBank3d[TileIdx];
			int			TriCount=Elem->TriCount;				
			sTri		*TList=&TriList[Elem->TriStart];

			CMX_SetTransMtxXY(&ThisRenderPos);
			CMX_SetRotMatrixXY(&FTab->Mtx);
			while (TriCount--)
			{
				P0=&VtxList[TList->P0]; P1=&VtxList[TList->P1]; P2=&VtxList[TList->P2];
				gte_ldv3(P0,P1,P2);
				setlen(TPrimPtr, GPU_PolyFT3Tag);
				TPrimPtr->code=TList->PolyCode;
				gte_rtpt_b();
				setShadeTex(TPrimPtr,1);
			
				T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
				T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
				T2=*(u16*)&TList->uv2;		// Get UV2
				*(u32*)&TPrimPtr->u0=T0;	// Set UV0
				*(u32*)&TPrimPtr->u1=T1;	// Set UV1
				*(u16*)&TPrimPtr->u2=T2;	// Set UV2

				ThisOT=OtPtr+TList->OTOfs;
				TList++;
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

		SetPrimPtr((u8*)TPrimPtr);

}

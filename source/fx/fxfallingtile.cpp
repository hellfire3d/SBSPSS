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
#include	"system\vid.h"
#include	"gfx\actor.h"

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
		CSoundMediator::playSfx(CSoundMediator::SFX_ANY_OBJECT_FALLING,false);
		Flags |=FX_FLAG_NO_THINK_KILL;
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
void	CFXFallingTile::render()
{
		CFX::render();

		if (!canRender()) return;

DVECTOR		&RenderPos=getRenderPos();

			u16			TileIdx=Tile>>2;
			CModelGfx::RenderTile(RenderPos,TileIdx);
/*			u16			Flip=Tile&3;
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
*/
}

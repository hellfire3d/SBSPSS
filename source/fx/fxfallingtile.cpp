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
		CSoundMediator::playSfx(CSoundMediator::SFX_ANY_OBJECT_FALLING,false,true);
		if (!isOnScreen(_Pos)) setToShutdown();
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXFallingTile::think(int _frames)
{
		if (Tile==0) setToShutdown();
		CFX::think(_frames);
		Pos.vx+=Velocity.vx;
		Pos.vy+=Velocity.vy;
		Velocity.vy++;
		Flags |=FX_FLAG_NO_THINK_KILL;

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
}

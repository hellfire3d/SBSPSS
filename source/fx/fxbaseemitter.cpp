/**************************/
/*** Emitter Base Class ***/
/**************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"
#include	"FX\FXBaseEmitter.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXBaseEmitter::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		Width=Height=16;
		CurrentCount=DataPtr->Rate;
		Life=DataPtr->Life;
}

/*****************************************************************************/
void	CFXBaseEmitter::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXBaseEmitter::think(int _frames)
{
		CFX::think(_frames);

		CurrentCount-=_frames;
		if (CurrentCount<=0)
		{
			CurrentCount=DataPtr->Rate;
			createParticle(_frames);
		}
			
}

/*****************************************************************************/
void	CFXBaseEmitter::createParticle(int _frames)
{
		CFX::Create(DataPtr->ParticleType,getPos());
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXBaseEmitter::render()
{
/*
DVECTOR	renderPos;
		if (MapEmitter)
		{
		CFX::render();
		if (!canRender()) return;
		renderPos=getRenderPos();
		}
		else
		{
			renderPos=getPos();
		}

POLY_F4		*f4;
		f4=GetPrimF4();
		setPolyF4(f4);
		setXYWH(f4,renderPos.vx,renderPos.vy,Width,Height);
		setSemiTrans(f4,1);
		setShadeTex(f4,0);
		setRGB0(f4,30,40,50);
		AddPrimToList(f4,0);
*/
}

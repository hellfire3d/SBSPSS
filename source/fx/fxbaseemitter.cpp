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
#include	"FX\FXEmitter.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXEmitter::init(DVECTOR const &_Pos)
{
		CFX::init();
		Pos=_Pos;
		SetFrame(0,0);
		Scale=ONE;
		BirthCount=0;
}

/*****************************************************************************/
void	CFXEmitter::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
void	CFXEmitter::setUp(int W,int H,int Rate,int Amount,int Life)
{
		SetSize(W,H);
		SetRate(Rate,Amount);
		SetLife(L);
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXEmitter::think(int _frames)
{
		CFX::think(_frames);

		BirthCount-=_frames;
		if (BirthCount<=0)
		{
			BirthCount=BirthRate;
			Create(_frames);
		}
		Life--;
		if (Life

}

/*****************************************************************************/
void	CFXEmitter::Create(int _frames)
{

}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXEmitter::render()
{
DVECTOR	renderPos;
		if (MapEmitter)
		{
		CFX::render();
		if (!canRender()) return;
		renderPos=getRenderPos();
		}
		else
		{
			renderPos=getPos()
		}

POLY_F4		*f4;
		f4=GetPrimF4();
		setPolyF4(f4);
		setXYWH(f4,renderPos.vx,renderPos.vy,Width.Height);
		setSemiTrans(f4,1);
		setShadeTex(f4,0);
		setRGB0(f4,30,40,50);
		AddPrimToList(f4,0);
}

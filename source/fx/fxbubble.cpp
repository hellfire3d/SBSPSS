/*****************/
/*** Bubble FX ***/
/*****************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"
#include	"level\layercollision.h"
#include	"FX\FXBaseAnim.h"
#include	"FX\FXBubble.h"


/*****************************************************************************/
void	CFXBubble::init(DVECTOR const &_Pos)
{
		CFXBaseAnim::init(_Pos);
		Life=32+getRndRange(63);
		Velocity.vy=-(getRndRange(4)+1);
		CurrentScaleX=CurrentScaleY=getRndRange(ONE/2)+(ONE/2);
		XIdx=getRnd()&15;
		if (!isOnScreen(_Pos)) setToShutdown();
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
static const s16	XT[16]={ 0,+1,+0,+1,+0, 0,-1,+0,-1,+0,0,+1,+0,+1,+0,};

void	CFXBubble::think(int _frames)
{
		Life-=_frames;
		if (Life<=0)
		{
			if (renderFrame!=FRM__BUBBLEPOP)
			{
				renderFrame=FRM__BUBBLEPOP;
			}
			else
			{
				killFX();
			}
		}
		else
		{
			CFXBaseAnim::think(_frames);
			Pos.vx+=XT[XIdx++];
			XIdx&=15;
		}
}

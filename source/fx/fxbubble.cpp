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
		VelY=-getRndRange(4)-1;
		CurrentScaleX=CurrentScaleY=getRndRange(DataPtr->Scale/2)+(DataPtr->Scale/2);
		Die=0;
		XIdx=getRnd()&15;

}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
int	XT[16]={ 0,+1,+0,+1,+0, 0,-1,+0,-1,+0,0,+1,+0,+1,+0,};
void	CFXBubble::think(int _frames)
{
		if (Die)
		{
			if (renderFrame!=FRM__BUBBLEPOP)
			{
				renderFrame=FRM__BUBBLEPOP;
			}
			else
			{
				setToShutdown();
			}
		}
		else
		{
			CFXBaseAnim::think(_frames);
			Pos.vx+=XT[XIdx++];
			XIdx&=15;
		}
}

/*****************************************************************************/
void	CFXBubble::killFX()
{

	Die=1;
}


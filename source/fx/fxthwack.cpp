/***********************/
/*** Anim Base Class ***/
/***********************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"

#include	"FX\FXBaseAnim.h"
#include	"FX\FXAttachAnim.h"


/*****************************************************************************/
void	CFXAttachAnim::init(DVECTOR const &_Pos)
{
		CFXBaseAnim::init(_Pos);
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXAttachAnim::render()
{
		CFXBaseAnim::render();
		if (!canRender() || !IsVisible) return;

CThing	*Parent=getParent();
		ASSERT(Parent);

DVECTOR	&ParentPos=Parent->getRenderPos();
int		FrameW=Frame->x1-Frame->x0;
int		HalfW=FrameW>>1;

		Frame->x0=ParentPos.vx-HalfW;
		Frame->y0=ParentPos.vy;
		Frame->x1=ParentPos.vx+HalfW;
		Frame->y1=ParentPos.vy;

		int	BY=Frame->y2-Frame->y0;
//		setCollisionCentreOffset(0,BY>>1);
		setCollisionSize(FrameW,BY*2);

}


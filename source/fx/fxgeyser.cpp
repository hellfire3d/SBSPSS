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

int		MaxHeight=128;

/*****************************************************************************/
void	CFXAttachAnim::init(DVECTOR const &_Pos)
{
		CFXBaseAnim::init(_Pos);
		Height=TargetHeight=8;
}

/*****************************************************************************/
void	CFXAttachAnim::thing(int Frames)
{
		CFXBaseAnim::think(Frames);
CThing	*Parent=getParent();

int		HDiff=TargetHeight-Height;
		Height+=(HDiff+1)/2;
				
		if (Parent) 
		{
			Pos2=Parent->getRenderPos();
		}
		else
		{

		}

}
/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXAttachAnim::render()
{
		CFXBaseAnim::render();
		if (!canRender() || Flags & FX_FLAG_HIDDEN) return;

int		FrameW=Frame->x1-Frame->x0;
int		HalfW=FrameW>>1;

		Frame->x0=Pos2.vx-HalfW;
		Frame->y0=Pos2.vy;
		Frame->x1=Pos2.vx+HalfW;
		Frame->y1=Pos2.vy;

		int	BY=Frame->y2-Frame->y0;
//		setCollisionCentreOffset(0,BY>>1);
		setCollisionSize(FrameW,BY*2);

}


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

#include	"FX\FXAnim.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXAnim::init(DVECTOR const &_Pos)
{
		CFX::init();
		Pos=_Pos;
		SetFrame(0,0);
		Scale=ONE;
}

/*****************************************************************************/
void	CFXAnim::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
void	CFXAnim::SetFrame(int Base,int Count,int Shift=0)
{
		Frame=0;
		BaseFrame=Base;
		FrameShift=Shift;
		MaxFrame=(Count<<FrameShift)-1;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXAnim::think(int _frames)
{
		CFX::think(_frames);

		Frame+=_frames;
		if (Frame>=MaxFrame) setToShutdown();
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXAnim::render()
{
		CFX::render();

		if (!canRender() || isSetToShutdown()) return;

DVECTOR		&RenderPos=getRenderPos();
POLY_FT4	*Ft4;
int			ThisFrame=(MaxFrame-Frame)>>FrameShift;

			printf("%i\n",ThisFrame);

			Ft4=m_spriteBank->printRotatedScaledSprite(BaseFrame+ThisFrame,RenderPos.vx,RenderPos.vy,Scale,Scale,0,OtPos*0);
			setRGB0(Ft4,R,G,B);

}

/************************/
/*** Trail Base Class ***/
/************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"game\game.h"

#include	"FX\FXBaseTrail.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXBaseTrail::init(DVECTOR const &_Pos)
{
		CFX::init();
		Pos=_Pos;
		HeadIdx=0;
		ListCount=0;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXBaseTrail::think(int _frames)
{
DVECTOR	Pos;
bool	hasParent=getFXParentPos(Pos);

		if (hasParent)
		{
			setPos(Pos);
		}
}

/*****************************************************************************/
CFXBaseTrail::sList	&CFXBaseTrail::moveHead()
{
		HeadIdx--;
		if (HeadIdx<0) HeadIdx+=LIST_SIZE;
		if (ListCount<LIST_SIZE) ListCount++;

		return(List[HeadIdx]);
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/

void	CFXBaseTrail::render()
{
DVECTOR	RenderPos;

		getFXRenderPos(RenderPos);

		if (canRender())
		{
			SpriteBank	*SprBank=CGameScene::getSpriteBank();
			int		ThisIdx=HeadIdx;
			int		StartY=RenderPos.vy;
			
			for (int i=0; i<ListCount; i++)
			{
				sList		&ThisElem=List[ThisIdx];
				POLY_FT4	*Ft4;

				RenderPos.vx+=ThisElem.Ofs.vx>>2;
				RenderPos.vy+=ThisElem.Ofs.vy>>2;
				if (ThisElem.Shade)
				{
					Ft4=SprBank->printRotatedScaledSprite(ThisElem.Frame,RenderPos.vx,RenderPos.vy,ThisElem.Scale,ThisElem.Scale,ThisElem.Angle,OtPos);
					setShadeTex(Ft4,0);
					setRGB0(Ft4,ThisElem.Shade,ThisElem.Shade,ThisElem.Shade);
					setSemiTrans(Ft4,1);
					Ft4->tpage|=Trans<<5;
				}
				ThisIdx++;
				ThisIdx&=LIST_SIZE-1;
			}
			int	BY=(RenderPos.vy-StartY)-32;
			setCollisionCentreOffset(0,BY>>1);
			setCollisionSize(64,-BY);
		}
}


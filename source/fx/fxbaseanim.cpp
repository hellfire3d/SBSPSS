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
#include	"level\layercollision.h"
#include	"FX\FXBaseAnim.h"

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


/*****************************************************************************/
void	CFXBaseAnim::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		CurrentFrame=0;
		CurrentScaleX=CurrentScaleY=DataPtr->Scale;
		CurrentHeading = 0;
		MaxFrame=((DataPtr->EndFrame-DataPtr->StartFrame)<<DataPtr->FrameShift)-1;
		if (DataPtr->Flags & FXANIM_FLAG_LOOP)
		{ // Looping Anim, so let it live forever!
			Life=-1;
		}
		else
		{
			Life=MaxFrame;
		}

		VelY=0;
		renderFrame=DataPtr->StartFrame;
}

/*****************************************************************************/
void	CFXBaseAnim::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXBaseAnim::think(int _frames)
{
		CFX::think(_frames);
		CurrentFrame+=_frames;

		if (CurrentFrame>=MaxFrame)
		{
			CurrentFrame=0;
		}
		Pos.vx+=DataPtr->Velocity.vx;
		Pos.vy+=DataPtr->Velocity.vy+VelY;

int		ThisFrame=CurrentFrame>>DataPtr->FrameShift;
		renderFrame=DataPtr->StartFrame+ThisFrame;

		if (DataPtr->Flags & FXANIM_FLAG_HAS_GRAVITY)
		{ 
			VelY++;
		}

		if (DataPtr->Flags & FXANIM_FLAG_COLLIDE_KILL)
		{
			CLayerCollision	*ColLayer=CGameScene::getCollision();
			int	DistY = ColLayer->getHeightFromGround( Pos.vx, Pos.vy, 16 );
			
			if (DistY<=0) 
			{
				Pos.vy-=DistY;
				killFX();
			}
		}
}

/*****************************************************************************/
void	CFXBaseAnim::killFX()
{
	setToShutdown();
// If has follow on effect, create it now
	if (DataPtr->EndFX)
	{
		CFX::Create((CFX::FX_TYPE)DataPtr->EndFX,getPos());
	}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXBaseAnim::render()
{
DVECTOR	RenderPos;

		getFXRenderPos(RenderPos);
		if (!canRender() || !IsVisible) return;

SpriteBank	*SprBank=CGameScene::getSpriteBank();
POLY_FT4	*Ft4=SprBank->printRotatedScaledSprite(renderFrame,RenderPos.vx,RenderPos.vy,CurrentScaleX,CurrentScaleY,CurrentHeading,OtPos);
			setShadeTex(Ft4,0);
			setRGB0(Ft4,DataPtr->R,DataPtr->G,DataPtr->B);
			setSemiTrans(Ft4,DataPtr->Flags & FXANIM_FLAG_TRANS);
}

/*****************************************************************************/
void	CFXBaseAnim::collidedWith(CThing *_thisThing)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player = (CPlayer *) _thisThing;

			if ( !player->isRecoveringFromHit() )
			{
				if ( DataPtr->Flags & FXANIM_FLAG_KILL_PLAYER )
				{
					player->takeDamage( DAMAGE__KILL_OUTRIGHT );
				}
				else if ( DataPtr->Flags & FXANIM_FLAG_INJURE_PLAYER )
				{
					player->takeDamage( DAMAGE__HIT_ENEMY );
				}
			}

			break;
		}

		default:
			break;
	}
}

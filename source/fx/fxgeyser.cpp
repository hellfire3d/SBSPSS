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

#include	"FX\FXGeyser.h"

static const int	GeyserSpeed=4;

/*****************************************************************************/
void	CFXGeyser::init(DVECTOR const &_Pos)
{
		CFX::init(_Pos);
		Height=TargetHeight=8;
		currentFrame=FRM__GUSH000;
		SoundId = NOT_PLAYING;
		BasePos=_Pos;
		OtPos+=3;

}

/*****************************************************************************/
void	CFXGeyser::shutdown()
{
	if ( SoundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) SoundId );
	}

	CFX::shutdown();
}

/*****************************************************************************/
void	CFXGeyser::think(int Frames)
{
		if (getParent())
		{ // make huge box
			setCollisionCentreOffset(0,-512/2);
			setCollisionSize(32,512);
		}

		CFX::think(Frames);
		Pos=BasePos;
		currentFrame++;
		if (currentFrame>FRM__GUSH003)
		{
			currentFrame=FRM__GUSH000;
		}
//		TargetHeight=MaxHeight;
}

/*****************************************************************************/
/*** Render ******************************************************************/
/*****************************************************************************/
void	CFXGeyser::render()
{
DVECTOR		RenderPos;
POLY_FT4	*Ft4;
SpriteBank	*SprBank=CGameScene::getSpriteBank();
CThing		*Parent=getParent();
int			W;

		getFXRenderPos(RenderPos);
		if (!canRender() || Flags & FX_FLAG_HIDDEN) return;

		if( SoundId != NOT_PLAYING && !CSoundMediator::isSfxStillPlaying( (xmPlayingId) SoundId ) )
		{
			// unlock sound if it has finished

			CSoundMediator::stopAndUnlockSfx( (xmPlayingId) SoundId );
			SoundId = NOT_PLAYING;
		}

// is it attached to a platform?
		if (Parent) 
		{ // yes, so get pos, and return
			Height=RenderPos.vy-Parent->getRenderPos().vy;
		}
		else
		{
			int		HDiff=TargetHeight-Height;
			Height+=(HDiff+(GeyserSpeed-1))/GeyserSpeed;

			if ( Height > 10 )
			{
				Flags |= FX_FLAG_INJURE_PLAYER;

				if ( SoundId == NOT_PLAYING && !GameScene.getIsPaused() )
				{
					SoundId = (int) CSoundMediator::playSfx(CSoundMediator::SFX_OIL_GEYSER,true);
				}
			}
			else
			{
				Flags &= ~FX_FLAG_INJURE_PLAYER;
			}
		}

// top
		Ft4=SprBank->printFT4(currentFrame,RenderPos.vx-8,RenderPos.vy-Height,0,0,OtPos);
		setShadeTex(Ft4,0);
		setRGB0(Ft4,RGB.R,RGB.G,RGB.B);
		setSemiTrans(Ft4,Flags & FX_FLAG_TRANS);
		W=(Ft4->x1-Ft4->x0)/2;
		Ft4->x0-=W;	Ft4->x1+=W;
		Ft4->x2-=W;	Ft4->x3+=W;
		if (Height<8)
		{
			Ft4->x0+=2;	Ft4->x1-=2;
			Ft4->x2+=2;	Ft4->x3-=2;
		}
		

		if (!Parent)
		{
			int		FrameW=Ft4->x1-Ft4->x0;
			setCollisionCentreOffset(FrameW>>1,-Height/2);
			setCollisionCentreOffset(0,-Height/2);
			setCollisionSize(FrameW,Height);
		}
// Base
		Ft4=SprBank->printFT4(FRM__GUSHBASE,RenderPos.vx-8,RenderPos.vy,currentFrame&2,0/*currentFrame&2*/,OtPos);
		setShadeTex(Ft4,0);
		setRGB0(Ft4,RGB.R,RGB.G,RGB.B);
		setSemiTrans(Ft4,Flags & FX_FLAG_TRANS);
		W=(Ft4->x1-Ft4->x0)/2;
		Ft4->x0-=W+2;	Ft4->x1+=W+2;
		Ft4->x2-=W-2;	Ft4->x3+=W-2;
		Ft4->y0=Ft4->y2-Height;
		Ft4->y1=Ft4->y3-Height;
		Ft4->v0++; Ft4->v1++;
		Ft4->v2--; Ft4->v3--;
		

}


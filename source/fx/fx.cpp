/*********************/
/*** FX Base Class ***/
/*********************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"
#include	"gfx\otpos.h"
#include	"player\player.h"


#include	"FX\FX.h"
#include	"FX\FXjfish.h"
#include	"FX\FXBaseTrail.h"
#include	"FX\FXBaseAnim.h"
//#include	"FX\FXBaseEmitter.h"


#include	"FX\FXLaser.h"
#include	"FX\FXThwack.h"
#include	"FX\FXBubble.h"
#include	"FX\FXFire.h"
#include	"FX\FXfallingTile.h"
#include	"FX\FXSteam.h"
#include	"FX\FXGeyser.h"
#include	"FX\FXSmoke.h"

#include	"FX\FXNrgBar.h"
#include	"FX\FXTVExplode.h"
#include	"FX\FXZzzz.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Anim Data 

CFX::sFXRGB	CFX::FXRGBTable[CFX::FX_RGB_MAX]=
{
	{127,127,255},		// Water
	{0,255,0},		// Acid
	{255,  0,  0},		// Lava
	{ 64, 64, 64},		// Oil
};

CFX::sFXRGB	FXRgbWater={127,127,255};

/*****************************************************************************/
CFXBaseAnim::sFXBaseData	FXDropBaseData=
{
		FRM__DRIP,FRM__DRIP,1,
		FX_FLAG_LOOP | FX_FLAG_COLLIDE_KILL | FX_FLAG_HAS_GRAVITY | FX_FLAG_NO_THINK_KILL, 
		0,0,CSoundMediator::SFX_HAZARD__ACID_DROP,
};

CFXBaseAnim::sFXBaseData	FXSplashBaseData=
{
		FRM__SPLASH001,FRM__SPLASH006,1,
		FX_FLAG_NO_THINK_KILL,
		0,0,0,
};

CFXBaseAnim::sFXBaseData	FXExplodeBaseData=
{
		FRM__EXPLOSION0001,FRM__EXPLOSION0008,1,
		FX_FLAG_NO_THINK_KILL,
		CSoundMediator::SFX_HAZARD__FIREBALL_LAND,0,0,
};

CFXBaseAnim::sFXBaseData	FXFireBaseData=
{
		FRM__FIRE01,FRM__FIRE08,1,
		FX_FLAG_LOOP | FX_FLAG_TRANS,
		0,CSoundMediator::SFX_FIRE_CRACKLING,0,
};

CFXBaseAnim::sFXBaseData	FXBubbleBaseData=
{
		FRM__BUBBLE_2,FRM__BUBBLE_2,1,
		FX_FLAG_LOOP | FX_FLAG_COLLIDE_KILL | FX_FLAG_NO_THINK_KILL,
		0,0,0,
};

/*****************************************************************************/
/*
CFXLaser	*TestFXPtr=0;
void	TestFX(DVECTOR Pos,CThing *Parent)
{
		if (!TestFXPtr)
		{
			TestFXPtr=(CFXLaser*)CFX::Create(CFX::FX_TYPE_LASER,Parent);
//			TestFXPtr->setLife(32);
//			TestFXPtr=0;
		}
		else
		{
			TestFXPtr->killFX();
			TestFXPtr=0;
		}
}
*/
/*****************************************************************************/

int	FXType=(CFX::FX_TYPE)CFX::FX_TYPE_JELLYFISH_LEGS;

#include	"game\game.h"
CFXZZZZ	*TFX=0;
void	TestFX(DVECTOR Pos,CThing *Parent)
{
/*
		if (!TFX)
		{
			TFX=(CFXZZZZ*)CFX::Create((CFX::FX_TYPE)FXType,Pos);
		}
		else
		{
			TFX->killFX();
			TFX=0;
		}
*/
		CFX::Create((CFX::FX_TYPE)FXType,Parent);

}

/*****************************************************************************/
CFX		*CFX::Create(const FX_TYPE Type)
{
CFX		*NewFX;

		NewFX = (CFX*)CThingManager::GetThing(CThing::TYPE_FX,Type);
		if (!NewFX)
		switch(Type)
		{
		case FX_TYPE_NONE:
			ASSERT(!"FX NONE CANT BE CREATED!");
			break;

		case FX_TYPE_BASE_ANIM:
			NewFX=new ("FXBaseAnim") CFXBaseAnim();
			break;

		case FX_TYPE_DROP_WATER:
			NewFX=new ("FXWaterDrip") CFXBaseAnim();
			NewFX->setBaseData(&FXDropBaseData);
			NewFX->setRGB(FX_RGB_WATER);
			NewFX->setAfterEffect(CFX::FX_TYPE_SPLASH_WATER);
			break;
		case FX_TYPE_DROP_ACID:
			NewFX=new ("FXAcidDrip") CFXBaseAnim();
			NewFX->setBaseData(&FXDropBaseData);
			NewFX->setRGB(FX_RGB_ACID);
			NewFX->setAfterEffect(CFX::FX_TYPE_SPLASH_ACID);
			break;
		case FX_TYPE_DROP_LAVA:
			NewFX=new ("FXLavaDrip") CFXBaseAnim();
			NewFX->setBaseData(&FXDropBaseData);
			NewFX->setRGB(FX_RGB_LAVA);
			NewFX->setAfterEffect(CFX::FX_TYPE_SPLASH_LAVA);
			break;
		case FX_TYPE_DROP_OIL:
			NewFX=new ("FXOilDrip") CFXBaseAnim();
			NewFX->setBaseData(&FXDropBaseData);
			NewFX->setRGB(FX_RGB_OIL);
			NewFX->setAfterEffect(CFX::FX_TYPE_SPLASH_OIL);
			break;
		
		case FX_TYPE_SPLASH_WATER:
			NewFX=new ("FXWaterSplash") CFXBaseAnim();
			NewFX->setBaseData(&FXSplashBaseData);
			NewFX->setRGB(FX_RGB_WATER);
			break;
		case FX_TYPE_SPLASH_ACID:
			NewFX=new ("FXAcidSplash") CFXBaseAnim();
			NewFX->setBaseData(&FXSplashBaseData);
			NewFX->setRGB(FX_RGB_ACID);
			break;
		case FX_TYPE_SPLASH_LAVA:
			NewFX=new ("FXLavaSplash") CFXBaseAnim();
			NewFX->setBaseData(&FXSplashBaseData);
			NewFX->setRGB(FX_RGB_LAVA);
			break;
		case FX_TYPE_SPLASH_OIL:
			NewFX=new ("FXOilSplash") CFXBaseAnim();
			NewFX->setBaseData(&FXSplashBaseData);
			NewFX->setRGB(FX_RGB_OIL);
			NewFX->setAfterEffect(CFX::FX_TYPE_EXPLODE);
			break;

		case FX_TYPE_BUBBLE_WATER:
			NewFX=new ("FXBubbleWater") CFXBubble();
			NewFX->setBaseData(&FXBubbleBaseData);
			NewFX->setRGB(FX_RGB_WATER);
			break;
		case FX_TYPE_BUBBLE_ACID:
			NewFX=new ("FXBubbleAcid") CFXBubble();
			NewFX->setBaseData(&FXBubbleBaseData);
			NewFX->setRGB(FX_RGB_ACID);
			break;
		case FX_TYPE_BUBBLE_LAVA:
			NewFX=new ("FXBubbleLava") CFXBubble();
			NewFX->setBaseData(&FXBubbleBaseData);
			NewFX->setRGB(FX_RGB_LAVA);
			break;
		case FX_TYPE_BUBBLE_OIL:
			NewFX=new ("FXBubbleOil") CFXBubble();
			NewFX->setBaseData(&FXBubbleBaseData);
			NewFX->setRGB(FX_RGB_OIL);
			break;

		case FX_TYPE_GEYSER_WATER:
			NewFX=new ("FXWaterGEYSER") CFXGeyser();
			NewFX->setRGB(FX_RGB_WATER);
			break;
		case FX_TYPE_GEYSER_ACID:
			NewFX=new ("FXAcidGEYSER") CFXGeyser();
			NewFX->setRGB(FX_RGB_ACID);
			break;
		case FX_TYPE_GEYSER_LAVA:
			NewFX=new ("FXLavaGEYSER") CFXGeyser();
			NewFX->setRGB(FX_RGB_LAVA);
			break;
		case FX_TYPE_GEYSER_OIL:
			NewFX=new ("FXOilGEYSER") CFXGeyser();
			NewFX->setRGB(FX_RGB_OIL);
			break;

		case FX_TYPE_THWACK:
			NewFX=new ("FXThwack") CFXThwack();
			break;

		case FX_TYPE_LIGHTNING_BOLT:
			ASSERT(!"FX_TYPE_LIGHTNING_BOLT");
			break;

		case FX_TYPE_STEAM:
			NewFX=new ("FXSteam") CFXSteam();
			break;
		case FX_TYPE_GAS:
			ASSERT(!"FX_TYPE_GAS");
			break;
		case FX_TYPE_FLAMES:
			NewFX=new ("FXFlames") CFXFire();
			NewFX->setBaseData(&FXFireBaseData);
			break;

		case FX_TYPE_SMOKE:
			NewFX=new ("FXSmoke") CFXSmoke();
			break;
		case FX_TYPE_SMOKE_PUFF:
			NewFX=new ("FXSmokePuff") CFXSmokePuff();
			break;

		case FX_TYPE_JELLYFISH_LEGS:
			NewFX=new ("JellyFish Legs") CFXJellyFishLegs();
			break;
		case FX_TYPE_FALLINGTILE:
			NewFX=new ("FXFalling Tile") CFXFallingTile();
			break;
		case FX_TYPE_EXPLODE:
			NewFX=new ("FXExplode") CFXBaseAnim();
			NewFX->setBaseData(&FXExplodeBaseData);
			break;
		case FX_TYPE_NRG_BAR:
			NewFX=new ("NRG Bar") CFXNRGBar();
			break;
		case FX_TYPE_LASER:
			NewFX=new ("FX Laser") CFXLaser();
			break;
		case FX_TYPE_TV_EXPLODE:
			NewFX=new ("FX TVExplode") CFXTVExplode();
			break;
		case FX_TYPE_ZZZZ:
			NewFX=new ("FX ZZZZ") CFXZZZZ();
			break;

		default:
			ASSERT(!"UNKNOWN FX TYPE");
			return NULL;
		}
		NewFX->setThingSubType(Type);
		return NewFX;
}

/*****************************************************************************/
CFX		*CFX::Create(const FX_TYPE Type,CThing *Parent)
{
CFX		*NewFX=CFX::Create(Type);

		NewFX->init(Parent->getPos());
		ASSERT(Parent);
		Parent->addChild(NewFX);

		return NewFX;
}

/*****************************************************************************/
CFX		*CFX::Create(const FX_TYPE Type,DVECTOR const &Pos)
{
CFX		*NewFX=CFX::Create(Type);

		NewFX->init(Pos);

		return NewFX;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFX::leftThinkZone(int _frames)
{
	if(Flags & FX_FLAG_NO_THINK_KILL)
	{
		killFX();
	}

	if( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx(m_soundId);
		m_soundId=NOT_PLAYING;
	}
}

/*****************************************************************************/
void	CFX::init()
{
		CFXThing::init();
		OtPos=OTPOS__ACTOR_POS;
		Flags=0;
		Velocity.vx=Velocity.vy=0;
		m_soundId = NOT_PLAYING;
		EndSnd=0;

}

/*****************************************************************************/
void	CFX::init(DVECTOR const &_Pos)
{
		init();
		Pos=_Pos;
}

/*****************************************************************************/
void	CFX::setBaseData(void *Data)
{
		setRGB(127,127,127);
		AfterEffect=FX_TYPE_NONE;
}

/*****************************************************************************/
void	CFX::shutdown()
{
		CFXThing::shutdown();
		if( m_soundId != NOT_PLAYING )
		{
			CSoundMediator::stopAndUnlockSfx(m_soundId);
			m_soundId=NOT_PLAYING;
		}
}

/*****************************************************************************/
void	CFX::think(int _frames)
{
CThing	*Parent=getParent();

		if (Parent)
		{
			Pos=Parent->getPos();
		}

		CFXThing::think(_frames);

		if (Flags & FX_FLAG_HAS_LIFE)
		{
			Life-=_frames;
			if (Life<=0)
			{
				killFX();
			}
		}

		if (Flags & FX_FLAG_HAS_GRAVITY)
		{ 
			Velocity.vy++;
		}

		Pos.vx+=Velocity.vx;
		Pos.vy+=Velocity.vy;
		
		if (Velocity.vy)
		if (Flags & FX_FLAG_COLLIDE_KILL || Flags )
		{
			CLayerCollision	*ColLayer=CGameScene::getCollision();
			int	DistY = ColLayer->getHeightFromGround( Pos.vx, Pos.vy, 16 );
			
			if (DistY<=0) 
			{
				Pos.vy+=DistY;
				if (Flags & FX_FLAG_COLLIDE_KILL)
				{
					killFX();
				}
				else
				{
					Velocity.vx/=2;
					Velocity.vy=-(Velocity.vy/2);
					if (Velocity.vy)
					{
						CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_BLINK,false);
					}
				}
			}
		}


}

/*****************************************************************************/
void	CFX::killFX()
{
	setToShutdown();
// If has follow on effect, create it now
	if (AfterEffect!=CFX::FX_TYPE_NONE && canThink())
	{
		CFX::Create((CFX::FX_TYPE)AfterEffect,getPos());
	}
	if( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx(m_soundId);
		m_soundId=NOT_PLAYING;
	}
	if (EndSnd)
	{
		CSoundMediator::playSfx( (CSoundMediator::SFXID)EndSnd,false,true);
	}
}


/*****************************************************************************/
void	CFX::render()
{
		CFXThing::render();
}

/*****************************************************************************/
bool	CFX::getFXParentPos(DVECTOR &Pos)
{
CThing	*Parent=getParent();
		if (!Parent) return(false);

		if (Flags & FX_FLAG_SCREEN_FX)
		{
			Pos=getPos();
		}
		else
		{
			Pos=Parent->getPos();
		}
		return(true);
}

/*****************************************************************************/
void	CFX::getFXRenderPos(DVECTOR &Pos)
{
		if (Flags & FX_FLAG_SCREEN_FX)
		{
			Pos=getPos();
		}
		else
		{
			CFX::render();
			Pos=getRenderPos();
		}
}

/*****************************************************************************/
void	CFX::collidedWith(CThing *_thisThing)
{
	ASSERT(canCollide());
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player = (CPlayer *) _thisThing;

			if ( !player->isRecoveringFromHit() )
			{
				player->takeDamage( DAMAGE__HIT_ENEMY );
			}

			break;
		}

		default:
			break;
	}
}

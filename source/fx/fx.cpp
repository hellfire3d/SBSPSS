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

#include	"FX\FX.h"
#include	"FX\FXjfish.h"
#include	"FX\FXBaseTrail.h"
#include	"FX\FXBaseAnim.h"
#include	"FX\FXBaseEmitter.h"

#include	"FX\FXfallingTile.h"
#include	"FX\FXSteam.h"

#include	"FX\FXNrgBar.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Anim Data 
CFXBaseAnim::sFXAnim	FXWaterSplashData=
{
		ONE,
		FRM__SPLASH001,FRM__SPLASH006,
		1,
		127,127,255,
		0,
		{0,0},
		0,
};

CFXBaseAnim::sFXAnim	FXAcidSplashData=
{
		ONE,
		FRM__SPLASH001,FRM__SPLASH006,
		1,
		0,255,0,
		0,
		{0,0},
		0
};
CFXBaseAnim::sFXAnim	FXLavaSplashData=
{
		ONE,
		FRM__SPLASH001,FRM__SPLASH006,
		1,
		255,0,0,
		0,
		{0,0},
		0
};
CFXBaseAnim::sFXAnim	FXOilSplashData=
{
		ONE,
		FRM__SPLASH001,FRM__SPLASH006,
		1,
		0,0,0,
		0,
		{0,0},
		CFX::FX_TYPE_EXPLODE,
};

CFXBaseAnim::sFXAnim	FXExplodeData=
{
		ONE,
		FRM__EXPLOSION0001,FRM__EXPLOSION0006,
		1,
		127,127,127,
		0,
		{0,0},
		0
};

CFXBaseAnim::sFXAnim	FXFireData=
{
		ONE,
		FRM__FIRE01,FRM__FIRE06,
		1,
		127,127,127,
		CFXBaseAnim::FXANIM_FLAG_LOOP,
		{0,0},
		0
};

CFXBaseAnim::sFXAnim	FXWaterDripData=
{
		ONE,
		FRM__DRIP,FRM__DRIP,
		1,
		127,127,255,
		CFXBaseAnim::FXANIM_FLAG_LOOP | CFXBaseAnim::FXANIM_FLAG_COLLIDE_KILL | CFXBaseAnim::FXANIM_FLAG_HAS_GRAVITY,
		{0,1},
		CFX::FX_TYPE_SPLASH_WATER
};

CFXBaseAnim::sFXAnim	FXAcidDripData=
{
		ONE,
		FRM__DRIP,FRM__DRIP,
		1,
		127,127,255,
		CFXBaseAnim::FXANIM_FLAG_LOOP | CFXBaseAnim::FXANIM_FLAG_COLLIDE_KILL,
		{0,1},
		CFX::FX_TYPE_SPLASH_WATER
};
CFXBaseAnim::sFXAnim	FXLavaDripData=
{
		ONE,
		FRM__DRIP,FRM__DRIP,
		1,
		127,127,255,
		CFXBaseAnim::FXANIM_FLAG_LOOP | CFXBaseAnim::FXANIM_FLAG_COLLIDE_KILL,
		{0,1},
		CFX::FX_TYPE_SPLASH_WATER
};
CFXBaseAnim::sFXAnim	FXOilDripData=
{
		ONE,
		FRM__DRIP,FRM__DRIP,
		1,
		127,127,255,
		CFXBaseAnim::FXANIM_FLAG_LOOP | CFXBaseAnim::FXANIM_FLAG_COLLIDE_KILL,
		{0,1},
		CFX::FX_TYPE_SPLASH_WATER
};

/*****************************************************************************/
/*
CFXSteam	*TestFXPtr=0;
void	TestFX(DVECTOR Pos)
{
		if (!TestFXPtr)
		{
			Pos.vx=256;
			Pos.vy=128;
			TestFXPtr=(CFXSteam*)CFX::Create(CFX::FX_TYPE_STEAM,Pos);
			TestFXPtr->setLife(32);
			TestFXPtr->setRelativeToMap(false);
			TestFXPtr=0;
		}
		else
		{
			TestFXPtr->killFX();
			TestFXPtr=0;
		}
}
*/
/*****************************************************************************/
int	FXType=(CFX::FX_TYPE)CFX::FX_TYPE_DROP_WATER;
void	TestFX(DVECTOR Pos)
{
		CFX::Create((CFX::FX_TYPE)FXType,Pos);
//		TestFXPtr->setLife(32);
}

/*****************************************************************************/
CFX		*CFX::Create(const FX_TYPE Type)
{
CFX		*NewFX;

		NewFX = (CFX*)CThingManager::GetThing(CThing::TYPE_FX,Type);
		if (!NewFX)
		switch(Type)
		{
		case FX_TYPE_FALLINGTILE:
			NewFX=new ("FXFalling Tile") CFXFallingTile();
			break;
		case FX_TYPE_STEAM:
			NewFX=new ("FXSteam") CFXSteam();
			break;
		case FX_TYPE_SPLASH_WATER:
			NewFX=new ("FXWaterSplash") CFXBaseAnim();
			NewFX->setData(&FXWaterSplashData);
			break;
		case FX_TYPE_SPLASH_ACID:
			NewFX=new ("FXAcidSplash") CFXBaseAnim();
			NewFX->setData(&FXAcidSplashData);
			break;
		case FX_TYPE_SPLASH_LAVA:
			NewFX=new ("FXLavaSplash") CFXBaseAnim();
			NewFX->setData(&FXLavaSplashData);
			break;
		case FX_TYPE_SPLASH_OIL:
			NewFX=new ("FXOilSplash") CFXBaseAnim();
			NewFX->setData(&FXOilSplashData);
			break;
		case FX_TYPE_EXPLODE:
			NewFX=new ("FXExplode") CFXBaseAnim();
			NewFX->setData(&FXExplodeData);
			break;
		case FX_TYPE_FLAMES:
			NewFX=new ("FXFlames") CFXBaseAnim();
			NewFX->setData(&FXFireData);
			break;

		case FX_TYPE_DROP_WATER:
			NewFX=new ("FXWaterDrip") CFXBaseAnim();
			NewFX->setData(&FXWaterDripData);
			break;
		case FX_TYPE_DROP_ACID:
			NewFX=new ("FXAcidDrip") CFXBaseAnim();
			NewFX->setData(&FXAcidDripData);
			break;
		case FX_TYPE_DROP_LAVA:
			NewFX=new ("FXLavaDrip") CFXBaseAnim();
			NewFX->setData(&FXLavaDripData);
			break;
		case FX_TYPE_DROP_OIL:
			NewFX=new ("FXOilDrip") CFXBaseAnim();
			NewFX->setData(&FXOilDripData);
			break;

		case FX_TYPE_NRG_BAR:
			NewFX=new ("NRG Bar") CFXNRGBar();
			break;

		case FX_TYPE_JELLYFISH_LEGS:
			NewFX=new ("JellyFish Legs") CFXJellyFishLegs();
			break;

		case FX_TYPE_BUBBLE_WATER:
		case FX_TYPE_BUBBLE_ACID:
		case FX_TYPE_BUBBLE_LAVA:
		case FX_TYPE_BUBBLE_OIL:
		case FX_TYPE_LIGHTNING_BOLT:
		case FX_TYPE_LIGHTNING_SHEET:
		case FX_TYPE_LIGHTNING_BLAST:
		case FX_TYPE_LIGHTNING_RADIAL:
		case FX_TYPE_LIGHTNING_PROJECTILE:
		case FX_TYPE_SHOCKWAVE:
		case FX_TYPE_DAZE:

		case FX_TYPE_CASCADE:		
		case FX_TYPE_CASCADE_SPLASH:	
		case FX_TYPE_FIREBALL:

		case FX_TYPE_SMOKE:
		case FX_TYPE_GAS:

	
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
void	CFX::init()
{
		CFXThing::init();
		OtPos=OTPOS__ACTOR_POS;
		RelativeToMap=true;
		Life=-1;	// Set to immortal
}

/*****************************************************************************/
void	CFX::init(DVECTOR const &_Pos)
{
		init();
		Pos=_Pos;
}

/*****************************************************************************/
void	CFX::shutdown()
{
		CFXThing::shutdown();
}

/*****************************************************************************/
void	CFX::think(int _frames)
{
		CFXThing::think(_frames);

		if (Life>0)
		{
			Life-=_frames;
			if (Life<=0)
			{
				killFX();
			}
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

		if (RelativeToMap)
		{
			Pos=Parent->getPos();
		}
		else
		{
			Pos=getPos();
		}
		return(true);
}

/*****************************************************************************/
void	CFX::getFXRenderPos(DVECTOR &Pos)
{
		if (RelativeToMap)
		{
			CFX::render();
			Pos=getRenderPos();
		}
		else
		{
			Pos=getPos();
		}
}

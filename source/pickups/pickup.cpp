/*=========================================================================

	pickup.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"	// Damnit.. include order! :( (pkg)
#endif

#include "pickups\pickup.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

// For the factory..

#ifndef	__PICKUPS_PHEALTH_H__
#include "pickups\phealth.h"
#endif

#ifndef	__PICKUPS_PLIFE_H__
#include "pickups\plife.h"
#endif

#ifndef	__PICKUPS_PSPATULA_H__
#include "pickups\pspatula.h"
#endif

#ifndef	__PICKUPS_PJLAMMO_H__
#include "pickups\pjlammo.h"
#endif

#ifndef	__PICKUPS_PBUBMIX_H__
#include "pickups\pbubmix.h"
#endif

#ifndef	__PICKUPS_PBLOWER_H__
#include "pickups\pblower.h"
#endif

#ifndef	__PICKUPS_PGLASSES_H__
#include "pickups\pglasses.h"
#endif

#ifndef	__PICKUPS_PSHOES_H__
#include "pickups\pshoes.h"
#endif

#ifndef	__PICKUPS_PBALLOON_H__
#include "pickups\pballoon.h"
#endif

#ifndef	__PICKUPS_PHELMET_H__
#include "pickups\phelmet.h"
#endif

#ifndef	__PICKUPS_PNET_H__
#include "pickups\pnet.h"
#endif

#ifndef	__PICKUPS_PQUEST_H__
#include "pickups\pquest.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::init()
{
	CPickupThing::init();

	m_spriteBank=new ("pickup sprite") SpriteBank();
	m_spriteBank->load(INGAMEFX_INGAMEFX_SPR);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::shutdown()
{
	m_spriteBank->dump();	delete m_spriteBank;
	CPickupThing::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include "pad\pads.h"
void	CBasePickup::think(int _frames)
{
	CPickupThing::think(_frames);
	thinkPickup(_frames);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::render()
{
	DVECTOR	ofs,pos;
	int		visibilityRadius;
	
	CPickupThing::render();
	
	ofs=CLevel::getCameraPos();
	pos.vx=Pos.vx-ofs.vx;
	pos.vy=Pos.vy-ofs.vy;
	visibilityRadius=getVisibilityRadius();
	if(pos.vx>0-visibilityRadius&&pos.vx<512+visibilityRadius&&
	   pos.vy>0-visibilityRadius&&pos.vy<256+visibilityRadius)
	{
		renderPickup(&pos);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::collidedWith(CThing *_thisThing)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
			collect((CPlayer*)_thisThing);
			break;

		default:
			ASSERT(0);
			break;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::setPos(const DVECTOR *_pos)
{
	Pos=*_pos;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBasePickup::collect(class CPlayer *_player)
{
	shutdown();
	delete this;
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseRespawningPickup::init()
{
	CBasePickup::init();
	m_respawnTime=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int respawnFlashFrames=50;
void	CBaseRespawningPickup::think(int _frames)
{
	if(m_respawnTime<=respawnFlashFrames)
	{
		CBasePickup::think(_frames);
	}

	if(m_respawnTime)
	{
		m_respawnTime-=_frames;
		if(m_respawnTime<0)
		{
			m_respawnTime=0;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseRespawningPickup::render()
{
	if(m_respawnTime==0||
	   m_respawnTime<respawnFlashFrames&&!(m_respawnTime&4))
	{
		CBasePickup::render();
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseRespawningPickup::collect(class CPlayer *_player)
{
	m_respawnTime=getRespawnTime();
}




/*----------------------------------------------------------------------
	Function:
	Purpose:	This is basically a factory method for making pickups :)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CBasePickup	*createPickup(const PICKUP_TYPE _type,const DVECTOR *_pos)
{
	CBasePickup	*pickup;
	DVECTOR		pickupPos;

	switch(_type)
	{
		case PICKUP__BIG_HEALTH:
			pickup=new ("LargeHealthPickup") CLargeHealthPickup();
			break;
		case PICKUP__MEDIUM_HEALTH:
			pickup=new ("MediumHealthPickup") CMediumHealthPickup();
			break;
		case PICKUP__SMALL_HEALTH:
			pickup=new ("SmallHealthPickup") CSmallHealthPickup();
			break;

		case PICKUP__LIFE:
			pickup=new ("LifePickup") CLifePickup();
			break;

		case PICKUP__SPATULA:
			pickup=new ("SpatulaPickup") CSpatulaPickup();
			break;

		case PICKUP__JELLY_LAUNCHER_AMMO:
			pickup=new ("JellyLauncherAmmoPickup") CJellyLauncherAmmoPickup();
			break;

		case PICKUP__BUBBLE_MIXTURE:
			pickup=new ("BubbleAmmoPickup") CBubbleMixturePickup();
			break;

		case PICKUP__BUBBLE_WAND:
			pickup=new ("BubbleWandPickup") CBubbleWandPickup();
			break;

		case PICKUP__NET:
			pickup=new ("NetPickup") CNetPickup();
			break;

		case PICKUP__GLASSES:
			pickup=new ("GlassesPickup") CGlassesPickup();
			break;

		case PICKUP__SQUEAKY_SHOES:
			pickup=new ("ShoesPickup") CShoesPickup();
			break;

		case PICKUP__BALLOON:
			pickup=new ("BalloonPickup") CBalloonPickup();
			break;

		case PICKUP__HELMET:
			pickup=new ("HelmetPickup") CHelmetPickup();
			break;

		case PICKUP__CORAL_BLOWER:
			pickup=new ("CoralBlowerPickup") CCoralBlowerPickup();
			break;

		case PICKUP__QUEST_ITEM__TEST:
			pickup=new ("QuestItemPickup") CTestQuestItemPickup();
			break;

		default:
			ASSERT(!"UNKNOWN PICKUP TYPE");
			pickup=NULL;
			break;
	}

	pickup->init();
	pickupPos=pickup->getSizeForPlacement();
	pickupPos.vx=_pos->vx+(pickupPos.vx/2);
	pickupPos.vy=_pos->vy+(pickupPos.vy/2)-16;
	pickup->setPos(&pickupPos);

	return pickup;
}

/*===========================================================================
end */

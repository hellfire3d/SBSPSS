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

if(PadGetDown(0)&PAD_L2)
{
	collect(NULL);
}
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
	/*
	DrawLine(pos.vx-15,pos.vy-15,pos.vx+15,pos.vy+15,255,255,255,0);
	DrawLine(pos.vx+15,pos.vy-15,pos.vx-15,pos.vy+15,255,255,255,0);
	*/
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
void	CBaseRespawningPickup::think(int _frames)
{
	if(m_respawnTime==0)
	{
		CBasePickup::think(_frames);
	}
	else
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
	   m_respawnTime<50&&!(m_respawnTime&3))
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

	switch(_type)
	{
		case PICKUP__100_PERCENT_LIFE:
			pickup=new ("100HealthPickup") C100HealthPickup();
			break;
		case PICKUP__50_PERCENT_LIFE:
			pickup=new ("15HealthPickup") C50HealthPickup();
			break;
		case PICKUP__25_PERCENT_LIFE:
			pickup=new ("25HealthPickup") C25HealthPickup();
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

		case PICKUP__QUEST_ITEM__TEST:
			pickup=new ("QuestItemPickup") CTestQuestItemPickup();
			break;

		default:
			ASSERT(!"UNKNOWN PICKUP TYPE");
			pickup=NULL;
			break;
	}

	pickup->init();
	pickup->setPos(_pos);

	return pickup;
}

/*===========================================================================
end */

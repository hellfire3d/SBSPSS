/*=========================================================================

	tplatfrm.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "triggers\trigger.h"
#include "triggers\tplatfrm.h"

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

	
/*	Std Lib
	------- */

/*	Data
	---- */

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
#if defined (__USER_art__) || defined (__USER_sbart__)
#include "gfx\prim.h"
void	CPlatformTrigger::render()
{
	DVECTOR	ofs;
	CRECT	area;

	CTriggerThing::render();

	ofs=CLevel::getCameraPos();
	area=getCollisionArea();
	area.x1-=ofs.vx;
	area.y1-=ofs.vy;
	area.x2-=ofs.vx;
	area.y2-=ofs.vy;

	if(area.x1<=511&&area.x2>=0&&
	   area.y1<=255&&area.y2>=0)
	{
		POLY_F4	*f4;
		f4=GetPrimF4();
		setXY4(f4,area.x1,area.y1,
				  area.x2,area.y1,
				  area.x1,area.y2,
				  area.x2,area.y2);
		setRGB0(f4,0,255,0);
		setSemiTrans(f4,true);
		AddPrimToList(f4,0);
		DrawLine(area.x1,area.y1,area.x2,area.y1,0,255,0,0);
		DrawLine(area.x2,area.y1,area.x2,area.y2,0,255,0,0);
		DrawLine(area.x2,area.y2,area.x1,area.y2,0,255,0,0);
		DrawLine(area.x1,area.y2,area.x1,area.y1,0,255,0,0);
	}
}
#endif

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlatformTrigger::collidedWith(CThing *_thisThing)
{
	ASSERT(_thisThing->getThingType()==TYPE_PLAYER);

	CPlayer *player = (CPlayer *) _thisThing;
	ATTACK_STATE playerState = player->getAttackState();

	if ( playerState == ATTACK_STATE__BUTT_BOUNCE )
	{
		m_platform->trigger();
	}
}

/*===========================================================================
end */

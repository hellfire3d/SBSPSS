/*=========================================================================

	pmdead.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmdead.h"


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
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
void	CPlayerModeDead::enter()
{
	m_deadTime=0;
	m_player->setAnimNo(ANIM_SPONGEBOB_DEATHSPIN);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeDead::think()
{
	m_deadTime++;
	if(m_deadTime<m_player->getAnimFrameCount())
	{
		m_player->setAnimFrame(m_deadTime);
	}

m_player->moveVertical(5);

	if((m_deadTime>DEATH_DELAY&&m_player->getPadInputDown()&PI_ACTION)||
	   m_deadTime>DEATH_TIMEOUT)
	{
		respawn();
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
/*
void	CPlayerModeDead::render(DVECTOR *_pos)
{
	DVECTOR	deadSbPos;

	deadSbPos=*_pos;
	deadSbPos.vy-=m_deadTime;

	m_player->renderSb(&deadSbPos,ANIM_SPONGEBOB_DEATHANGLE,0);
}
*/



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeDead::fall()
{
	/*
	m_yVelocity+=getPlayerMetrics()->m_metric[DEFAULT_PLAYER_PLAYER_GRAVITY];
	if(m_yVelocity>=metrics->m_metric[DEFAULT_PLAYER_TERMINAL_VELOCITY]<<VELOCITY_SHIFT)
	{
		m_yVelocity=metrics->m_metric[DEFAULT_PLAYER_TERMINAL_VELOCITY]<<VELOCITY_SHIFT;
	}
	*/
}

/*===========================================================================
end */

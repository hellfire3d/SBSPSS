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

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif


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
	int	deathAnim;

	m_deadTime=0;
	
	switch(m_player->getDeathType())
	{
		default:
		case DEATHTYPE__NORMAL:
			deathAnim=ANIM_SPONGEBOB_DEATHFORWARDS;
			break;
		case DEATHTYPE__DRYUP:
			deathAnim=ANIM_SPONGEBOB_DEATHDRY;
			break;
		case DEATHTYPE__SQUASH:
			deathAnim=ANIM_SPONGEBOB_DEATHSQUASH;
			break;
		case DEATHTYPE__LIQUID:
			deathAnim=ANIM_SPONGEBOB_DEATHFORWARDS;
			break;
		case DEATHTYPE__FALL_TO_DEATH:
			deathAnim=-1;
			break;
	}

	if(deathAnim!=-1)
	{
		m_player->setAnimNo(deathAnim);
	}

	CSoundMediator::stopSpeech();


	m_killed=false;
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
	if(m_player->getDeathType()!=DEATHTYPE__FALL_TO_DEATH)
	{
		if(m_deadTime<m_player->getAnimFrameCount())
		{
			m_player->setAnimFrame(m_deadTime);
		}
	}

	m_player->moveVertical(5);

	if(!m_killed)
	if((m_deadTime>DEATH_DELAY&&m_player->getPadInputDown()&(PI_JUMP|PI_FIRE))||
	   m_deadTime>DEATH_TIMEOUT)
	{
		// Take a life off..
		CGameSlotManager::getSlotData()->m_lives--;

		CGameScene::restartlevel();
		m_killed=true;
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

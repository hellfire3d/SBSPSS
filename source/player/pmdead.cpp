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

static int anim=0;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeDead::enter()
{
	m_deadTime=0;
	
	switch(m_player->getDeathType())
	{
		default:
		case DEATHTYPE__NORMAL:
			m_deathAnim=ANIM_SPONGEBOB_DEATHFORWARDS;
			break;
		case DEATHTYPE__DRYUP:
			m_deathAnim=ANIM_SPONGEBOB_DEATHDRY;
			break;
		case DEATHTYPE__SQUASH:
			m_deathAnim=ANIM_SPONGEBOB_DEATHSQUASH;
			break;
		case DEATHTYPE__LIQUID:
			m_deathAnim=ANIM_SPONGEBOB_DEATHFORWARDS;
			break;
		case DEATHTYPE__FALL_TO_DEATH:
			m_deathAnim=-1;
			break;
	}

	if(m_deathAnim!=-1)
	{
		m_player->setAnimNo(m_deathAnim);
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
		int frameCount,frame;

		if(m_deathAnim!=-1)
		{
			m_player->setAnimNo(m_deathAnim);
		}
		frameCount=m_player->getAnimFrameCount()-1;
		if(m_deadTime<=frameCount)
		{
			frame=m_deadTime;
		}
		else
		{
			frame=frameCount;
		}
		m_player->setAnimFrame(frame);
	}

	m_player->moveVertical(5);

	if(!m_killed)
	{
		if((m_deadTime>DEATH_DELAY&&m_player->getPadInputDown()&(PI_JUMP|PI_FIRE))||
		   m_deadTime>DEATH_TIMEOUT)
		{
			if(GameScene.getLevelNumber()!=5)
			{
				// Take a life off..
				CGameSlotManager::getSlotData()->m_lives--;
			}

			CGameScene::restartlevel();
			m_killed=true;
		}
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

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
	m_deadTime=0;
	
	switch(m_player->getDeathType())
	{
		default:
		case DEATHTYPE__NORMAL:
		case DEATHTYPE__DRYUP:
		case DEATHTYPE__SQUASH:
			m_deathAnim=ANIM_SPONGEBOB_DEATHBOUNCE;
			m_deathMovementType=BOUNCE_OFF_SCREEN;
			m_bounceVelocity.vx=BOUNCE_INITIALX*m_player->getFacing();
			m_bounceVelocity.vy=BOUNCE_INITIALY;
			m_player->setSBOTPosToFront();
			break;
		case DEATHTYPE__LIQUID:
			m_deathAnim=ANIM_SPONGEBOB_DEATHTAR;
			m_deathMovementType=STAY_STILL;
			break;
		case DEATHTYPE__FALL_TO_DEATH:
			m_deathAnim=-1;
			m_deathMovementType=FALL_TO_DEATH;
			break;
	}

	if(m_deathAnim!=-1)
	{
		m_player->setAnimNo(m_deathAnim);
	}

	CSoundMediator::stopSpeech();

	m_killed=false;

	m_player->clearPlatform();
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

	if(m_player->getPos().vy<GameScene.GetLevel().getMapHeight16()+(4*16))
	{
		if(m_deathMovementType==FALL_TO_DEATH)
		{
			m_player->moveVertical(5);
		}
		else if(m_deathMovementType==BOUNCE_OFF_SCREEN)
		{
			DVECTOR	pos=m_player->getPos();
			pos.vx+=m_bounceVelocity.vx>>BOUNCE_VELOCITY_SHIFT;
			pos.vy+=m_bounceVelocity.vy>>BOUNCE_VELOCITY_SHIFT;
			if(pos.vy<0)pos.vy=0;
			m_player->setPos(pos);

			if(m_bounceVelocity.vy<BOUNCE_MAXY)
			{
				m_bounceVelocity.vy+=BOUNCE_MOVEY;
				if(m_bounceVelocity.vy>BOUNCE_MAXY)
				{
					m_bounceVelocity.vy=BOUNCE_MAXY;
				}
			}

			if(m_bounceVelocity.vx<0)
			{
				m_bounceVelocity.vx+=BOUNCE_MOVEX;
				if(m_bounceVelocity.vx>0)
				{
					m_bounceVelocity.vx=0;
				}
			}
			else if(m_bounceVelocity.vx>0)
			{
				m_bounceVelocity.vx-=BOUNCE_MOVEX;
				if(m_bounceVelocity.vx<0)
				{
					m_bounceVelocity.vx=0;
				}
			}
		}
	}

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
/*===========================================================================
end */

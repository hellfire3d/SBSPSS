/*=========================================================================

	pmnet.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmnet.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef	__ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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

// Net pulse
int	npspeed=150;
int npsize=40;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeNet::enter()
{
	m_netting=false;
	m_netState=NET_STATE__EMPTY;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	netLaunchPos={-10,-70};
void	CPlayerModeNet::think()
{
	// If we're netting then restore the 'real' anim number/frame before
	// doing the think so that the rest of the code doesn't know what
	// is going on ;)
	if(m_netting)
	{
		setAnimNo(m_savedAnimNo);
		setAnimFrame(m_savedAnimFrame);
	}
	CPlayerModeBase::think();

	// Start to net?
	if(!m_netting&&getPadInputDown()&PI_ACTION&&canSwingNetFromThisState())
	{
		m_netFrame=0;
		m_netting=true;
	}

	// Netting?
	if(m_netting)
	{
		switch(m_netState)
		{
			case NET_STATE__EMPTY:
				{
					DVECTOR	playerPos;
					int		playerFacing;
					CRECT	netRect;
					CThing	*thing;

					playerPos=m_player->getPos();
					playerFacing=m_player->getFacing();

					netRect.x1=playerPos.vx-20;
					netRect.y1=playerPos.vy-20-40;
					netRect.x2=playerPos.vx+20;
					netRect.y2=playerPos.vy+20-40;

					thing=CThingManager::checkCollisionAreaAgainstThings(&netRect,CThing::TYPE_ENEMY,false);
					while(thing)
					{
						if(((CNpcEnemy*)thing)->canBeCaughtByNet())
						{
							((CNpcEnemy*)thing)->caughtWithNet();
							m_netState=NET_STATE__JUST_CAUGHT_SOMETHING;
							thing=NULL;
							m_netSin=0;
						}
						else
						{
							thing=CThingManager::checkCollisionAreaAgainstThings(&netRect,CThing::TYPE_ENEMY,true);
						}
					}
				}
				break;

			case NET_STATE__JUST_CAUGHT_SOMETHING:
				break;

			case NET_STATE__FULL:
				if(m_netFrame==0)
				{
					// Launch projectile at halfway through the swing..
					CPlayerProjectile *projectile;

					int		playerFacing;
					int		fireHeading;
					DVECTOR	launchPos;

					playerFacing=m_player->getFacing();
					fireHeading=1024+(1024*playerFacing);
					launchPos=m_player->getPos();
					launchPos.vx+=netLaunchPos.vx*playerFacing;
					launchPos.vy+=netLaunchPos.vy;


					projectile = new( "user projectile" ) CPlayerProjectile;
					projectile->init(	launchPos,
										fireHeading,
										CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
										CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
										5*60);
					projectile->setLayerCollision( m_player->getLayerCollision() );

					m_netState=NET_STATE__JUST_LAUNCHED_SOMETHING;
				}
				break;

			case NET_STATE__JUST_LAUNCHED_SOMETHING:
				break;
		}

		m_player->setAnimNo(ANIM_SPONGEBOB_SWIPE);
		m_player->setAnimFrame(m_netFrame);
		m_netFrame++;
		if(m_netFrame>=m_player->getAnimFrameCount())
		{
			m_player->setAnimNo(m_savedAnimNo);
			m_player->setAnimFrame(m_savedAnimFrame);
			m_netting=false;

			if(m_netState==NET_STATE__JUST_CAUGHT_SOMETHING)
			{
				m_netState=NET_STATE__FULL;
			}
			else if(m_netState==NET_STATE__JUST_LAUNCHED_SOMETHING)
			{
				m_netState=NET_STATE__EMPTY;
			}
		}
	}

	if(m_netState==NET_STATE__FULL)
	{
		m_netSin=(m_netSin+npspeed)&4095;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeNet::renderModeUi()
{
	SpriteBank	*sb;
	sFrameHdr	*fh;

	sb=m_player->getSpriteBank();
	fh=sb->getFrameHeader(FRM__NET);
	if(m_netState==NET_STATE__FULL)
	{
		// Net has a jellyfish inside
		int size=256+128+((msin(m_netSin)*npsize)>>12);
		sb->printFT4Scaled(fh,CPlayer::POWERUPUI_ICONX-(fh->W/2),CPlayer::POWERUPUI_ICONY-(fh->H/2),0,0,CPlayer::POWERUPUI_OT,size);
	}
	else
	{
		sb->printFT4(fh,CPlayer::POWERUPUI_ICONX-(fh->W/2),CPlayer::POWERUPUI_ICONY-(fh->H/2),0,0,CPlayer::POWERUPUI_OT);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeNet::setAnimNo(int _animNo)
{
	CPlayerModeBase::setAnimNo(_animNo);
	m_savedAnimNo=_animNo;
}

void	CPlayerModeNet::setAnimFrame(int _animFrame)
{
	CPlayerModeBase::setAnimFrame(_animFrame);
	m_savedAnimFrame=_animFrame;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeNet::canSwingNetFromThisState()
{
	int	ret=false;

	switch(getState())
	{
		case STATE_IDLE:
		case STATE_IDLETEETER:
		case STATE_JUMP:
		case STATE_RUN:
		case STATE_FALL:
			ret=true;
			break;

		case STATE_SPRINGUP:
		case STATE_FALLFAR:
		case STATE_BUTTBOUNCE:
		case STATE_BUTTFALL:
		case STATE_BUTTLAND:
		case STATE_DUCK:
		case STATE_SOAKUP:
		case STATE_GETUP:
			break;
	}

	return ret;
}

/*===========================================================================
end */

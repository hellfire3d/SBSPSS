/*=========================================================================

	pmjelly.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmjelly.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef	__PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeJellyLauncher::enter()
{
	CPlayerModeBase::enter();
	m_firingState=FIRING_STATE__NONE;
	m_player->giveJellyAmmo();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	jellyLaunchPos={-20,-25};
void	CPlayerModeJellyLauncher::think()
{
	CPlayerModeBase::think();

	// Start to fire?
	switch(m_firingState)
	{
		case FIRING_STATE__NONE:
			if(getPadInputDown()&PI_FIRE&&m_player->getJellyAmmo()&&canFireFromThisState())
			{
				m_firingFrame=0;
				m_firingTime=0;
				m_firingState=FIRING_STATE__POWERINGUP;
			}
			break;
		case FIRING_STATE__POWERINGUP:
			if(getPadInputHeld()&PI_FIRE)
			{
				int	fc,frame;
				if(m_firingTime<TIMEOUT_FOR_BIG_SHOT)
				{
					m_firingTime++;
				}
				m_player->setAnimNo(ANIM_SPONGEBOB_FIRESTART);
				fc=m_player->getAnimFrameCount();
				if(m_firingTime>=fc)
				{
					frame=fc;
				}
				else
				{
					frame=m_firingTime;
				}
				m_player->setAnimFrame(frame);
			}
			else
			{
				m_firingState=FIRING_STATE__FIRING;
				m_player->setAnimNo(ANIM_SPONGEBOB_FIREEND);
				launchProjectile();
			}
			break;
		case FIRING_STATE__FIRING:
			m_player->setAnimFrame(m_firingFrame++);
			if(m_firingFrame>=m_player->getAnimFrameCount())
			{
				// Finished recoil
				setState(STATE_IDLE);
				m_firingState=FIRING_STATE__NONE;
			}
			break;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeJellyLauncher::renderModeUi()
{
	SpriteBank	*sb;
	sFrameHdr	*fh;
	FontBank	*fb;
	char		buf[4];

	sb=m_player->getSpriteBank();
	fh=sb->getFrameHeader(FRM__LAUNCHER);
	switch(m_firingState)
	{
		case FIRING_STATE__NONE:
		case FIRING_STATE__FIRING:
			sb->printFT4(fh,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT);
			break;
		case FIRING_STATE__POWERINGUP:
			{
				int	rotRange,scaleBase,scaleRange;
				int	xs,ys,rot;
				rotRange=m_firingTime;
				scaleBase=4500+(m_firingTime*10);
				scaleRange=m_firingTime*2;
				rot=(getRndRange(rotRange*2)-rotRange)&4095;
				xs=scaleBase+getRndRange(scaleRange);
				ys=scaleBase+getRndRange(scaleRange);
				sb->printRotatedScaledSprite(fh,CPlayer::POWERUPUI_ICONX+(fh->W/2),CPlayer::POWERUPUI_ICONY+(fh->H/2),xs,ys,rot,CPlayer::POWERUPUI_OT);
			}
			break;
	}

	fb=m_player->getFontBank();
	sprintf(buf,"x%d",m_player->getJellyAmmo());
	fb->print(CPlayer::POWERUPUI_TEXTX,CPlayer::POWERUPUI_TEXTY,buf);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeJellyLauncher::setState(int _state)
{
	int	allowChange;

	allowChange=true;
	if(m_firingState!=FIRING_STATE__NONE)
	{
		switch(_state)
		{
			case STATE_FALL:
			case STATE_JUMPBACK:
				// Break out of firing state!
				m_firingState=FIRING_STATE__NONE;
				break;

			case STATE_IDLE:
			case STATE_IDLETEETER:
			case STATE_JUMP:
			case STATE_SPRINGUP:
			case STATE_RUN:
			case STATE_FALLFAR:
			case STATE_BUTTBOUNCE:
			case STATE_BUTTFALL:
			case STATE_BUTTLAND:
			case STATE_BUTTBOUNCEUP:
			case STATE_DUCK:
			case STATE_SOAKUP:
			case STATE_GETUP:
			case STATE_CROUCHDOWN:
			case STATE_CROUCHUP:
				allowChange=false;
				break;
		}
	}

	if(allowChange)
	{
		return CPlayerModeBase::setState(_state);
	}
	else
	{
		return false;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeJellyLauncher::canFireFromThisState()
{
	int	ret=false;

	switch(getState())
	{
		case STATE_IDLE:
		case STATE_IDLETEETER:
			ret=true;
			break;

		case STATE_JUMP:
		case STATE_SPRINGUP:
		case STATE_RUN:
		case STATE_FALL:
		case STATE_FALLFAR:
		case STATE_BUTTBOUNCE:
		case STATE_BUTTFALL:
		case STATE_BUTTLAND:
		case STATE_BUTTBOUNCEUP:
		case STATE_DUCK:
		case STATE_SOAKUP:
		case STATE_GETUP:
		case STATE_CROUCHDOWN:
		case STATE_CROUCHUP:
		case STATE_JUMPBACK:
			break;
	}

	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeJellyLauncher::launchProjectile()
{
	int					playerFacing;
	DVECTOR				launchPos;
	int					fireHeading;
	CPlayerProjectile	*projectile;

	playerFacing=m_player->getFacing();
	launchPos=getPlayerPos();
	launchPos.vx+=playerFacing*jellyLaunchPos.vx;
	launchPos.vy+=jellyLaunchPos.vy;
	if(m_firingTime==TIMEOUT_FOR_BIG_SHOT&&m_player->getJellyAmmo()>=AMMO_AMOUNT_FOR_BIG_SHOT)
	{
		// Powered up, big shot
		int	i;

		fireHeading=1024+(1024*playerFacing)-512;
		for(i=0;i<3;i++)
		{
			projectile=new("JellyProjectile") CPlayerProjectile;
			projectile->init(launchPos,
							fireHeading,
							 CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
							CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
							5*60);
			projectile->setLayerCollision( m_player->getLayerCollision() );
			fireHeading+=512;
			m_player->useOneJelly();
		}
	}
	else
	{
		// Normal, small shot
		fireHeading=1024+(1024*m_player->getFacing());
		projectile=new("JellyProjectile") CPlayerProjectile;
		projectile->init(launchPos,
						 fireHeading,
						 CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
						 CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
						 5*60);
		projectile->setLayerCollision( m_player->getLayerCollision() );
		m_player->useOneJelly();
	}
}

/*===========================================================================
end */

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

#ifndef	__PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __GAME_GAME_H__
#include "game/game.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

// States
#ifndef __PLAYER__PSLOOK_H__
#include "player\pslook.h"
#endif

#ifndef __PLAYER__PSJUMP_H__
#include "player\psjump.h"
#endif

#ifndef __PLAYER__PSJMPBCK_H__
#include "player\psjmpbck.h"
#endif

#ifndef __PLAYER__PSRUN_H__
#include "player\psrun.h"
#endif

#ifndef __PLAYER__PSFALL_H__
#include "player\psfall.h"
#endif

#ifndef __PLAYER__PSHITGND_H__
#include "player\pshitgnd.h"
#endif

#ifndef __PLAYER__PSIDLE_H__
#include "player\psidle.h"
#endif

#ifndef __PLAYER__PSBUTT_H__
#include "player\psbutt.h"
#endif

#ifndef __PLAYER__PSDUCK_H__
#include "player\psduck.h"
#endif

#ifndef __PLAYER__PSSPRING_H__
#include "player\psspring.h"
#endif

#ifndef __PLAYER_PSCART_H__
#include "player\pscart.h"
#endif

#ifndef __PLAYER_PSFLOAT_H__
#include "player\psfloat.h"
#endif

#ifndef __PLAYER_PSDANCE_H__
#include "player\psdance.h"
#endif

#ifndef __PLAYER_PSSWAL_H__
#include "player\psswal.h"
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

static	CPlayerState	*s_stateTable[]=
{
	&s_stateWeaponIdle,						// STATE_IDLE
	&s_stateTeeterIdle,						// STATE_IDLETEETER
	&s_stateJump,							// STATE_JUMP
	&s_stateSpring,							// STATE_SPRINGUP
	&s_stateRun,							// STATE_RUN
	&s_stateFall,							// STATE_FALL
	&s_stateFallFar,						// STATE_FALLFAR
	&s_stateHitGround,						// STATE_HITGROUND
	&s_stateButtBounce,						// STATE_BUTTBOUNCE
	&s_stateButtBounceFall,					// STATE_BUTTFALL
	&s_stateButtBounceLand,					// STATE_BUTTLAND
	&s_stateButtBounceUp,					// STATE_BUTTBOUNCEUP
	&s_stateSoakUp,							// STATE_SOAKUP
	&s_stateGetUp,							// STATE_GETUP
	&s_stateLookDown,						// STATE_LOOKDOWN
	&s_stateLookDownRelax,					// STATE_LOOKDOWNRELAX
	&s_stateLookUp,							// STATE_LOOKUP
	&s_stateLookUpRelax,					// STATE_LOOKUPRELAX
	&s_stateJumpBack,						// STATE_JUMPBACK
	&s_stateCart,							// STATE_CART
	&s_stateFloat,							// STATE_FLOAT
	&s_stateDance,							// STATE_CELEBRATE
	&s_stateSwallow,						// STATE_SWALLOW
};


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
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeJellyLauncher::think()
{
	// If we're firing then restore the 'real' anim number/frame before
	// doing the think so that the rest of the code doesn't know what
	// is going on ;)
	if(m_firingState==FIRING_STATE__FIRING||m_firingState==FIRING_STATE__RECOILING)
	{
		setAnimNo(m_savedAnimNo);
		setAnimFrame(m_savedAnimFrame);
	}
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
				m_colourFlash=0;
			}
			break;
		case FIRING_STATE__POWERINGUP:
			if(getPadInputHeld()&PI_FIRE)
			{
				if(m_firingTime<TIMEOUT_FOR_BIG_SHOT)
				{
					m_firingTime++;
				}
				else
				{
					m_colourFlash=(m_colourFlash+FLASHSPEED)&4095;
				}
				CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_MEDIUM,(m_firingTime*32)/TIMEOUT_FOR_BIG_SHOT);
			}
			else
			{
				CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_MEDIUM);
				launchProjectile();
				m_firingState=FIRING_STATE__RECOILING;
				m_firingFrame=0;
//				m_firingState=FIRING_STATE__FIRING;
//				m_firingFrame=0;
			}
			break;
		case FIRING_STATE__FIRING:
//			m_player->setAnimNo(ANIM_SPONGEBOB_FIRESTART);
//			m_player->setAnimFrame(m_firingFrame++);
//			if(m_firingFrame>=m_player->getAnimFrameCount())
//			{
//				CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_MEDIUM);
//				launchProjectile();
//				m_firingState=FIRING_STATE__RECOILING;
//				m_firingFrame=0;
//			}
			break;
		case FIRING_STATE__RECOILING:
			m_player->setAnimNo(ANIM_SPONGEBOB_FIREEND);
			m_player->setAnimFrame(m_firingFrame++);
			if(m_firingFrame>=m_player->getAnimFrameCount())
			{
				m_firingState=FIRING_STATE__NONE;
				m_player->setAnimNo(m_savedAnimNo);
				m_player->setAnimFrame(m_savedAnimFrame);
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
void	CPlayerModeJellyLauncher::setAnimNo(int _animNo)
{
	CPlayerModeBase::setAnimNo(_animNo);
	m_savedAnimNo=_animNo;
}

void	CPlayerModeJellyLauncher::setAnimFrame(int _animFrame)
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
void	CPlayerModeJellyLauncher::renderModeUi()
{
	SpriteBank	*sb;
	sFrameHdr	*fh;
	FontBank	*fb;
	char		buf[4];

	sb=CGameScene::getSpriteBank();
	fh=sb->getFrameHeader(FRM__SMALL_LAUNCHER);
	switch(m_firingState)
	{
		case FIRING_STATE__NONE:
		case FIRING_STATE__FIRING:
		case FIRING_STATE__RECOILING:
			sb->printFT4(fh,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT);
			break;
		case FIRING_STATE__POWERINGUP:
			{
				int			rotRange,scaleBase,scaleRange;
				int			xs,ys,rot;
				POLY_FT4	*ft4;
				int			colour;

				rotRange=m_firingTime/2;
				scaleBase=4500+(m_firingTime*10);
				scaleRange=m_firingTime*2;
				rot=(getRndRange(rotRange*2)-rotRange)&4095;
				xs=scaleBase+getRndRange(scaleRange);
				ys=scaleBase+getRndRange(scaleRange);
				ft4=sb->printRotatedScaledSprite(fh,CPlayer::POWERUPUI_ICONX+(fh->W/2),CPlayer::POWERUPUI_ICONY+(fh->H/2),xs,ys,rot,CPlayer::POWERUPUI_OT);
				colour=abs((msin(m_colourFlash)*FLASHSCALE)>>12);
				setRGB0(ft4,128+colour,128-colour,128-colour);
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
int		CPlayerModeJellyLauncher::canFireFromThisState()
{
	int	ret=false;

	switch(getState())
	{
		case STATE_IDLE:
		case STATE_IDLETEETER:
		case STATE_JUMP:
		case STATE_RUN:
		case STATE_FALL:
		case STATE_SPRINGUP:
			ret=true;
			break;

		case STATE_FALLFAR:
		case STATE_BUTTBOUNCE:
		case STATE_BUTTFALL:
		case STATE_BUTTLAND:
		case STATE_BUTTBOUNCEUP:
		case STATE_SOAKUP:
		case STATE_GETUP:
		case STATE_LOOKDOWN:
		case STATE_LOOKDOWNRELAX:
		case STATE_LOOKUP:
		case STATE_LOOKUPRELAX:
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
DVECTOR	jellyLaunchPos={-20,-25};
void	CPlayerModeJellyLauncher::launchProjectile()
{
	int					playerFacing;
	DVECTOR				launchPos;
	int					fireHeading;
	CPlayerProjectile	*projectile;

	CSoundMediator::playSfx(CSoundMediator::SFX_JELLY_LAUNCHER);

	playerFacing=m_player->getFacing();
	launchPos=getPlayerPos();
	launchPos.vx+=playerFacing*jellyLaunchPos.vx;
	launchPos.vy+=jellyLaunchPos.vy;
	if(m_firingTime==TIMEOUT_FOR_BIG_SHOT&&m_player->getJellyAmmo()>1)
	{
		// Powered up, big shot
		int	i;

		fireHeading=1024+(1024*playerFacing)-POWER_UP_SHOT_ANGLE;
		for(i=0;i<3&&m_player->getJellyAmmo();i++)
		{
			projectile=CPlayerProjectile::Create();
			projectile->init(launchPos,
							fireHeading,
							 CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
							CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
							(2*GameState::getOneSecondInFrames()));

			projectile->setRGB( 255 + ( 128 << 8 ) + ( 255 << 16 ) );
			fireHeading+=POWER_UP_SHOT_ANGLE;
			m_player->useOneJelly();
		}
	}
	else
	{
		// Normal, small shot
		fireHeading=1024+(1024*m_player->getFacing());
		projectile=CPlayerProjectile::Create();
		projectile->init(launchPos,
						 fireHeading,
						 CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
						 CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
						 (2*GameState::getOneSecondInFrames()));

		projectile->setRGB( 255 + ( 128 << 8 ) + ( 255 << 16 ) );
		m_player->useOneJelly();
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CPlayerState	**CPlayerModeJellyLauncher::getStateTable()
{
	return s_stateTable;
}

/*===========================================================================
end */

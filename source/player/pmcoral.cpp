/*=========================================================================

	pmcoral.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmcoral.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

// States
#ifndef __PLAYER__PSCROUCH_H__
#include "player\pscrouch.h"
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

#ifndef __PLAYER__PSDUCK_H__
#include "player\psduck.h"
#endif

#ifndef __PLAYER__PSSPRING_H__
#include "player\psspring.h"
#endif

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PROJECTL_PRNPCSPR_H__
#include "projectl\prnpcspr.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
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
	&s_stateWeaponIdle,						// STATE_IDLETEETER
	&s_stateJump,							// STATE_JUMP
	&s_stateSpring,							// STATE_SPRINGUP
	&s_stateWalk,							// STATE_RUN
	&s_stateFall,							// STATE_FALL
	&s_stateFallFar,						// STATE_FALLFAR
	&s_stateHitGround,						// STATE_HITGROUND
	NULL,									// STATE_BUTTBOUNCE
	NULL,									// STATE_BUTTFALL
	NULL,									// STATE_BUTTLAND
	NULL,									// STATE_BUTTBOUNCEUP
	NULL,									// STATE_DUCK
	NULL,									// STATE_SOAKUP
	&s_stateGetUp,							// STATE_GETUP
	&s_stateCrouchDown,						// STATE_CROUCHDOWN
	&s_stateCrouchUp,						// STATE_CROUCHUP
	&s_stateJumpBack,						// STATE_JUMPBACK
};



static	PlayerMetrics	s_playerMetrics=
{	{
	DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
	DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
	DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
	DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
	DEFAULT_PLAYER_RUN_SPEEDUP/3,			// PM__RUN_SPEEDUP
	DEFAULT_PLAYER_RUN_REVERSESLOWDOWN/3,	// PM__RUN_REVERSESLOWDOWN
	DEFAULT_PLAYER_RUN_SLOWDOWN/3,			// PM__RUN_SLOWDOWN
	DEFAULT_PLAYER_PLAYER_GRAVITY,			// PM__GRAVITY
	DEFAULT_PLAYER_TERMINAL_VELOCITY,		// PM__TERMINAL_VELOCITY
	DEFAULT_BUTT_FALL_VELOCITY,				// PM__BUTT_FALL_VELOCITY
	DEFAULT_HITREACT_XVELOCITY,				// PM__HITREACT_XVELOCITY
	DEFAULT_HITREACT_YVELOCITY,				// PM__HITREACT_YVELOCITY
	DEFAULT_HITREACT_FRAMES,				// PM__HITREACT_FRAMES
}	};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeCoralBlower::enter()
{
	CPlayerModeBase::enter();
	m_blowerState=BLOWER_STATE__EMPTY;
	CSoundMediator::playSfx(CSoundMediator::SFX_ITEM__CORAL_BLOWER);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	blowerCatchPos={-110,-20};
DVECTOR blowerCatchSize={180,150};
DVECTOR blowerSuckUpPoint={-20,-20};
DVECTOR blowerLaunchPoint={-20,-20};
void	CPlayerModeCoralBlower::think()
{
	CPlayerModeBase::think();

	switch(m_blowerState)
	{
		case BLOWER_STATE__EMPTY:
			if(getPadInputDown()&PI_CATCH&&getState()==STATE_IDLE)
			{
				m_blowerState=BLOWER_STATE__SUCKING;
				m_enemy=NULL;
			}
			break;
		case BLOWER_STATE__SUCKING:
			if(!(getPadInputHeld()&PI_CATCH&&getState()==STATE_IDLE))
			{
				m_blowerState=BLOWER_STATE__EMPTY;
			}
			else if(m_enemy==NULL)
			{
				// Search for an enemy..
				DVECTOR	playerPos;
				int		playerFacing;
				CRECT	suckRect;
				CThing	*thing;

				playerPos=m_player->getPos();
				playerFacing=m_player->getFacing();

				suckRect.x1=playerPos.vx+(blowerCatchPos.vx*playerFacing)-(blowerCatchSize.vx/2);
				suckRect.y1=playerPos.vy+blowerCatchPos.vy-(blowerCatchSize.vy/2);
				suckRect.x2=suckRect.x1+blowerCatchSize.vx;
				suckRect.y2=suckRect.y1+blowerCatchSize.vy;

				#ifdef __USER_paul__
				{
				CRECT	area=suckRect;
				DVECTOR	ofs=CLevel::getCameraPos();
				area.x1-=ofs.vx;
				area.y1-=ofs.vy;
				area.x2-=ofs.vx;
				area.y2-=ofs.vy;
				DrawLine(area.x1,area.y1,area.x2,area.y1,255,255,255,0);
				DrawLine(area.x2,area.y1,area.x2,area.y2,255,255,255,0);
				DrawLine(area.x2,area.y2,area.x1,area.y2,255,255,255,0);
				DrawLine(area.x1,area.y2,area.x1,area.y1,255,255,255,0);
				}
				#endif


				thing=CThingManager::checkCollisionAreaAgainstThings(&suckRect,CThing::TYPE_ENEMY,false);
				while(thing)
				{
					if(((CNpcEnemy*)thing)->canBeSuckedUp())
					{
						m_enemy=(CNpcEnemy*)thing;
						thing=NULL;
					}
					else
					{
						thing=CThingManager::checkCollisionAreaAgainstThings(&suckRect,CThing::TYPE_ENEMY,true);
					}
				}
			}
			else
			{
				// Got an enemy.. suck him up
				if(m_enemy->suckUp(getSuckUpPoint(),1))
				{
					m_enemyFrame = m_enemy->getFrame();
					m_blowerState=BLOWER_STATE__FULL;
				}
			}
			break;
		case BLOWER_STATE__FULL:
			if(getPadInputDown()&PI_FIRE&&getState()==STATE_IDLE)
			{
				m_blowerState=BLOWER_STATE__AIMING;
				m_launchHeading=0;
				m_launchHeadingChangeSpeed=0;
			}
			break;
		case BLOWER_STATE__AIMING:
			if(getState()!=STATE_IDLE)
			{
				m_blowerState=BLOWER_STATE__FULL;
			}
			else if(!(getPadInputHeld()&PI_FIRE))
			{
				// Fire!
				m_blowerState=BLOWER_STATE__EMPTY;

				DVECTOR launchPos=m_player->getPos();
				int		facing=m_player->getFacing();
				int		launchHeading;

				launchPos.vx+=blowerLaunchPoint.vx*facing;
				launchPos.vy+=blowerLaunchPoint.vy;

				launchHeading=(-((m_launchHeading+1024)*facing)-1024)&4095;

				CEnemyAsSpriteProjectile *projectile;
				projectile = new( "blower projectile" ) CEnemyAsSpriteProjectile;
				projectile->init(	launchPos,
									launchHeading,//1024+(1024*facing),
									CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
									CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
									5*60);
				projectile->setLayerCollision( m_player->getLayerCollision() );

				//CActorGfx *projectileGfx;
				//projectileGfx=CActorPool::GetActor((FileEquate)ACTORS_SHELL_SBK);
				//projectile->setGraphic( projectileGfx );
				projectile->setGraphic( m_enemyFrame );
				projectile->setHasRGB( false );
			}
			else
			{
				int	padHeld;
				padHeld=getPadInputHeld();
				if(padHeld&PI_UP)
				{
					if(m_launchHeadingChangeSpeed>-BLOWER_AIM_SPEED_INITIAL)
					{
						m_launchHeadingChangeSpeed=-BLOWER_AIM_SPEED_INITIAL;
					}
					else
					{
						if(m_launchHeadingChangeSpeed>-BLOWER_AIM_SPEED_MAXIMUM)
						{
							m_launchHeadingChangeSpeed--;
						}
					}
					m_launchHeading+=m_launchHeadingChangeSpeed;
					if(m_launchHeading<BLOWER_MINIMUM_AIM_ANGLE)
					{
						m_launchHeading=BLOWER_MINIMUM_AIM_ANGLE;
					}
				}
				else if(padHeld&PI_DOWN)
				{
					if(m_launchHeadingChangeSpeed<BLOWER_AIM_SPEED_INITIAL)
					{
						m_launchHeadingChangeSpeed=BLOWER_AIM_SPEED_INITIAL;
					}
					else
					{
						if(m_launchHeadingChangeSpeed<BLOWER_AIM_SPEED_MAXIMUM)
						{
							m_launchHeadingChangeSpeed++;
						}
					}
					m_launchHeading+=m_launchHeadingChangeSpeed;
					if(m_launchHeading>BLOWER_MAXIMUM_AIM_ANGLE)
					{
						m_launchHeading=BLOWER_MAXIMUM_AIM_ANGLE;
					}
				}
				else
				{
					m_launchHeadingChangeSpeed=0;
				}
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
void	CPlayerModeCoralBlower::renderModeUi()
{
	SpriteBank	*sb;
	sFrameHdr	*fh;
//	char		buf[4];

	sb=m_player->getSpriteBank();
	fh=sb->getFrameHeader(FRM__BLOWER);
	if(m_blowerState==BLOWER_STATE__FULL||m_blowerState==BLOWER_STATE__AIMING)
	{
		// Blower has a creature/object inside
		sb->printFT4Scaled(FRM__BLOWER,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT,256+128);

		if(m_blowerState==BLOWER_STATE__AIMING)
		{
			// Draw aiming cursor
			int		facing,heading;
			DVECTOR	screenOfs,launchPos,targetPos;

			facing=m_player->getFacing();
			heading=((m_launchHeading+1024)*facing)&4095;
			screenOfs=CLevel::getCameraPos();
			launchPos=m_player->getPlayerPos();
			launchPos.vx+=(blowerLaunchPoint.vx*facing)-screenOfs.vx;
			launchPos.vy+=blowerLaunchPoint.vy-screenOfs.vy;
			targetPos.vx=launchPos.vx-((msin(heading)*BLOWER_TARGET_DISTANCE)>>12);
			targetPos.vy=launchPos.vy-((mcos(heading)*BLOWER_TARGET_DISTANCE)>>12);
			heading=((m_launchHeading+1024)*-facing+1024)&4095;

			sb->printRotatedScaledSprite(FRM__AIM_ARROW,targetPos.vx,targetPos.vy,8192,8192,heading,0);
			PAUL_DBGMSG("%d",heading);
		}
	}
	else
	{
		// Blower is empty
		sb->printFT4(fh,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT);
	}

//	sprintf(buf,"x%d",0);
//	m_player->getFontBank()->print(CPlayer::POWERUPUI_TEXTX,CPlayer::POWERUPUI_TEXTY,buf);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayerModeCoralBlower::canDoLookAround()
{
	return m_blowerState!=BLOWER_STATE__AIMING;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const struct PlayerMetrics	*CPlayerModeCoralBlower::getPlayerMetrics()
{
	return &s_playerMetrics;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CPlayerState	**CPlayerModeCoralBlower::getStateTable()
{
	return s_stateTable;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR			*CPlayerModeCoralBlower::getSuckUpPoint()
{
	static DVECTOR	suckUpPoint;

	suckUpPoint=getPlayerPos();
	suckUpPoint.vx+=blowerSuckUpPoint.vx*m_player->getFacing();
	suckUpPoint.vy+=blowerSuckUpPoint.vy;

	return &suckUpPoint;
}


/*===========================================================================
end */

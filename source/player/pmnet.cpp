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
#include "utils\utils.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

// States
#ifndef __PLAYER__PSBUTT_H__
#include "player\psbutt.h"
#endif

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

#ifndef __PLAYER__PSDUCK_H__
#include "player\psduck.h"
#endif

#ifndef __PLAYER__PSSPRING_H__
#include "player\psspring.h"
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
	&s_stateNetIdle,						// STATE_IDLE
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
	NULL,									// STATE_CART
	NULL,									// STATE_FLOAT
	&s_stateDance,							// STATE_CELEBRATE
	&s_stateSwallow,						// STATE_SWALLOW
};


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
	CPlayerModeBase::enter();
	m_netState=NET_STATE__INERT;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	netCatchPos[17]=		// Oh.. why not, eh?
{
	{	64-42,	-128+102	},
	{	64-15,	-128+86		},
	{	64-42,	-128+33		},
	{	64-42,	-128+33		},
	{	64-42,	-128+33		},
	{	64-80,	-128+35		},
	{	64-80,	-128+35		},
	{	64-95,	-128+64		},
	{	64-95,	-128+64		},
	{	64-114,	-128+110	},
	{	64-114,	-128+110	},
	{	64-114,	-128+110	},
	{	64-95,	-128+64		},
	{	64-80,	-128+35		},
	{	64-42,	-128+33		},
	{	64-15,	-128+86		},
	{	64-42,	-128+102	},
};
DVECTOR netCatchSize={50,30};
DVECTOR	netLaunchPos={-15,-90};
void	CPlayerModeNet::think()
{
	// If we're netting then restore the 'real' anim number/frame before
	// doing the think so that the rest of the code doesn't know what
	// is going on ;)
	if(m_netState!=NET_STATE__INERT)
	{
		setAnimNo(m_savedAnimNo);
		setAnimFrame(m_savedAnimFrame);
	}
	CPlayerModeBase::think();

	// Netting?
	switch(m_netState)
	{
		case NET_STATE__INERT:
			{
				int	padDown;
				padDown=getPadInputDown();
				if(padDown&(PI_CATCH|PI_FIRE)&&canSwingNetFromThisState())
				{
					if(padDown&PI_CATCH&&!m_player->isJellyFishAmmoFull())
					{
						m_netState=NET_STATE__CATCHING;
					}
					else if(padDown&PI_FIRE&&m_player->getJellyFishAmmo())
					{
						m_netState=NET_STATE__LAUNCHING;
					}
					m_netFrame=0;
				}
			}
			break;

		case NET_STATE__CATCHING:
			{
//				DVECTOR	catchPos;
//				DVECTOR	playerPos;
				int		playerFacing;
				CRECT	netRect;
				CThing	*thing;

				ASSERT(m_netFrame<(int)(sizeof(netCatchPos)/sizeof(DVECTOR)));
				DVECTOR const &catchPos=netCatchPos[m_netFrame];
				DVECTOR const &playerPos=m_player->getPos();
				playerFacing=m_player->getFacing();

				netRect.x1=playerPos.vx+(catchPos.vx*playerFacing)-(netCatchSize.vx/2);
				netRect.y1=playerPos.vy+catchPos.vy-(netCatchSize.vy/2);
				netRect.x2=netRect.x1+netCatchSize.vx;
				netRect.y2=netRect.y1+netCatchSize.vy;

/*
				#ifdef __USER_paul__
				{
				CRECT	area=netRect;
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
*/

				thing=CThingManager::checkCollisionAreaAgainstThings(&netRect,CThing::TYPE_ENEMY,false);
				while(thing)
				{
					if(((CNpcEnemy*)thing)->canBeCaughtByNet())
					{
						m_netState=NET_STATE__JUST_CAUGHT_SOMETHING;
						if(m_player->getJellyFishAmmo()==0)
						{
							m_netSin=0;
						}
						m_player->giveJellyFishAmmo();
						((CNpcEnemy*)thing)->caughtWithNet();
						thing=NULL;
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

		case NET_STATE__LAUNCHING:
			// Launch projectile at halfway through the swing..
			if(m_netFrame==4)
			{
				CPlayerProjectile *projectile;

				int		playerFacing;
				int		fireHeading;
				DVECTOR	launchPos;

				playerFacing=m_player->getFacing();
				fireHeading=1024+(512*playerFacing);
				launchPos=m_player->getPos();
				launchPos.vx+=netLaunchPos.vx*playerFacing;
				launchPos.vy+=netLaunchPos.vy;


				projectile = CPlayerProjectile::Create();
				
				projectile->init(	launchPos,
									fireHeading,
									CPlayerProjectile::PLAYER_PROJECTILE_BOUNCING,
									CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
									5*60);

				projectile->updateCollisionArea();
				projectile->setRGB(255+(128<<8)+(255<<16));

				m_netState=NET_STATE__JUST_LAUNCHED_SOMETHING;
				m_player->useOneJellyFishAmmo();
			}
			break;

		case NET_STATE__JUST_LAUNCHED_SOMETHING:
			break;
	}

	if(m_netState!=NET_STATE__INERT)
	{
		m_player->setAnimNo(ANIM_SPONGEBOB_SWIPE);
		m_player->setAnimFrame(m_netFrame);
		m_netFrame++;
		if(m_netFrame>=m_player->getAnimFrameCount())
		{
			m_player->setAnimNo(m_savedAnimNo);
			m_player->setAnimFrame(m_savedAnimFrame);

			m_netState=NET_STATE__INERT;
		}
	}

	if(m_player->getJellyFishAmmo())
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
	char		buf[4];

	sb=CGameScene::getSpriteBank();
	fh=sb->getFrameHeader(FRM__SMALL_NET);
	if(m_player->getJellyFishAmmo())
	{
		POLY_FT4	*ft4;

		// Net has a jellyfish inside
		int	size=256+128+((msin(m_netSin)*npsize)>>12);
		u32	colour;
		sb->printFT4Scaled(fh,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT,size);
		ft4=sb->printFT4Scaled(FRM__NETBLOB,CPlayer::POWERUPUI_ICONX+17,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT,size);
		setShadeTex(ft4,0);
		setRGB0(ft4,255,128,255);
	}
	else
	{
		sb->printFT4(fh,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT);
	}

	sprintf(buf,"x%d",m_player->getJellyFishAmmo());
	m_player->getFontBank()->print(CPlayer::POWERUPUI_TEXTX,CPlayer::POWERUPUI_TEXTY,buf);
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
CPlayerState	**CPlayerModeNet::getStateTable()
{
	return s_stateTable;
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

/*===========================================================================
end */

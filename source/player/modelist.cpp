/*=========================================================================

	modelist.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\player.h"


#ifndef __PLAYER_PMODES_H__
#include "player/pmodes.h"
#endif

#ifndef __PLAYER_PMBOOTS_H__
#include "player/pmboots.h"
#endif

#ifndef __PLAYER_PMBALLOON_H__
#include "player/pmballoon.h"
#endif


#ifndef __PLAYER__PSJUMP_H__
#include "player\psjump.h"
#endif

#ifndef __PLAYER__PSRUN_H__
#include "player\psrun.h"
#endif

#ifndef __PLAYER__PSFALL_H__
#include "player\psfall.h"
#endif

#ifndef __PLAYER__PSIDLE_H__
#include "player\psidle.h"
#endif

#ifndef __PLAYER__PSBUTT_H__
#include "player\psbutt.h"
#endif

#ifndef __PLAYER__PSCHOP_H__
#include "player\pschop.h"
#endif

#ifndef __PLAYER__PSDUCK_H__
#include "player\psduck.h"
#endif

#ifndef __PLAYER__PSDEAD_H__
#include "player\psdead.h"
#endif

#ifndef __PLAYER__PSFLY_H__
#include "player\psfly.h"
#endif

#ifndef __PLAYER__PSBALLOON_H__
#include "player\psballoon.h"
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

CPlayerMode					modeEmpty;
CPlayerModeBoots			modeBoots;
CPlayerModeBalloon			modeBalloon;

CPlayerStateUnarmedIdle		stateUnarmedIdle;
CPlayerStateCoralBlowerIdle	stateCoralBlowerIdle;
CPlayerStateTeeterIdle		stateTeeterIdle;
CPlayerStateJump			stateJump;
CPlayerStateRun				stateRun;
CPlayerStateFall			stateFall;
CPlayerStateFallFar			stateFallFar;
CPlayerStateButtBounce		stateButtBounce;
CPlayerStateButtBounceFall	stateButtBounceFall;
CPlayerStateButtBounceLand	stateButtBounceLand;
CPlayerStateChop			stateChop;
CPlayerStateRunChop			stateRunChop;
CPlayerStateAirChop			stateAirChop;
CPlayerStateDuck			stateDuck;
CPlayerStateSoakUp			stateSoackUp;
CPlayerStateGetUp			stateGetup;
CPlayerStateDead			stateDead;
CPlayerStateFly				stateFly;
CPlayerStateBalloon			stateBalloon;









CPlayer::PlayerMode CPlayer::s_modes[NUM_PLAYERMODES]=
{
	//
	// PLAYER_MODE_BASICUNARMED
	// Basic player mode. No attack, only butt bounce
	//
	{
		{	{
			DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
			DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
			DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
			DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
			DEFAULT_PLAYER_RUN_SPEEDUP,				// PM__RUN_SPEEDUP
			DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
			DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
		}	},
		&modeEmpty,
		{
			&stateUnarmedIdle,						// STATE_IDLE
			&stateTeeterIdle,						// STATE_IDLETEETER
			&stateJump,								// STATE_JUMP
			&stateRun,								// STATE_RUN
			&stateFall,								// STATE_FALL
			&stateFallFar,							// STATE_FALLFAR
			&stateButtBounce,						// STATE_BUTTBOUNCE
			&stateButtBounceFall,					// STATE_BUTTFALL
			&stateButtBounceLand,					// STATE_BUTTLAND
			NULL,									// STATE_ATTACK
			NULL,									// STATE_RUNATTACK
			NULL,									// STATE_AIRATTACK
			&stateDuck,								// STATE_DUCK
			&stateSoackUp,							// STATE_SOAKUP
			&stateGetup,							// STATE_GETUP
			&stateDead,								// STATE_DEAD
		}
	},

	//
	// PLAYER_MODE_FULLUNARMED
	// Full unarmed player mode. With butt bounce and karate chop
	//
	{
		{	{
			DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
			DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
			DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
			DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
			DEFAULT_PLAYER_RUN_SPEEDUP,				// PM__RUN_SPEEDUP
			DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
			DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
		}	},
		&modeEmpty,
		{
			&stateUnarmedIdle,						// STATE_IDLE
			&stateTeeterIdle,						// STATE_IDLETEETER
			&stateJump,								// STATE_JUMP
			&stateRun,								// STATE_RUN
			&stateFall,								// STATE_FALL
			&stateFallFar,							// STATE_FALLFAR
			&stateButtBounce,						// STATE_BUTTBOUNCE
			&stateButtBounceFall,					// STATE_BUTTFALL
			&stateButtBounceLand,					// STATE_BUTTLAND
			&stateChop,								// STATE_ATTACK
			&stateRunChop,							// STATE_RUNATTACK
			&stateAirChop,							// STATE_AIRATTACK
			&stateDuck,								// STATE_DUCK
			&stateSoackUp,							// STATE_SOAKUP
			&stateGetup,							// STATE_GETUP
			&stateDead,								// STATE_DEAD
		}
	},

	//
	// PLAYER_MODE_SQUEAKYBOOTS
	// Squeaky boots. Also has butt bounce and karate chop
	//
	{
		{	{
			DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
			DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
			DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
			DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
			DEFAULT_PLAYER_RUN_SPEEDUP,				// PM__RUN_SPEEDUP
			DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
			DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
		}	},
		&modeBoots,
		{
			&stateUnarmedIdle,						// STATE_IDLE
			&stateTeeterIdle,						// STATE_IDLETEETER
			&stateJump,								// STATE_JUMP
			&stateRun,								// STATE_RUN
			&stateFall,								// STATE_FALL
			&stateFallFar,							// STATE_FALLFAR
			&stateButtBounce,						// STATE_BUTTBOUNCE
			&stateButtBounceFall,					// STATE_BUTTFALL
			&stateButtBounceLand,					// STATE_BUTTLAND
			&stateChop,								// STATE_ATTACK
			&stateRunChop,							// STATE_RUNATTACK
			&stateAirChop,							// STATE_AIRATTACK
			&stateDuck,								// STATE_DUCK
			&stateSoackUp,							// STATE_SOAKUP
			&stateGetup,							// STATE_GETUP
			&stateDead,								// STATE_DEAD
		}
	},

	//
	// PLAYER_MODE_BALLOON
	// Holding a balloon
	//
	{
		{	{
			0,										// PM__JUMP_VELOCITY
			0,										// PM__MAX_JUMP_FRAMES
			0,										// PM__MAX_SAFE_FALL_FRAMES
			DEFAULT_PLAYER_MAX_RUN_VELOCITY/2,		// PM__MAX_RUN_VELOCITY
			DEFAULT_PLAYER_RUN_SPEEDUP/4,			// PM__RUN_SPEEDUP
			DEFAULT_PLAYER_RUN_REVERSESLOWDOWN/5,	// PM__RUN_REVERSESLOWDOWN
			DEFAULT_PLAYER_RUN_SLOWDOWN/5,			// PM__RUN_SLOWDOWN
		}	},
		&modeBalloon,
		{
			&stateBalloon,							// STATE_IDLE
			NULL,									// STATE_IDLETEETER
			NULL,									// STATE_JUMP
			NULL,									// STATE_RUN
			NULL,									// STATE_FALL
			NULL,									// STATE_FALLFAR
			NULL,									// STATE_BUTTBOUNCE
			NULL,									// STATE_BUTTFALL
			NULL,									// STATE_BUTTLAND
			NULL,									// STATE_ATTACK
			NULL,									// STATE_RUNATTACK
			NULL,									// STATE_AIRATTACK
			NULL,									// STATE_DUCK
			NULL,									// STATE_SOAKUP
			NULL,									// STATE_GETUP
			&stateDead,								// STATE_DEAD
		}
	},

	//
	// PLAYER_MODE_NET
	// Armed with net
	//
	{
		{	{
			DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
			DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
			DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
			DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
			DEFAULT_PLAYER_RUN_SPEEDUP,				// PM__RUN_SPEEDUP
			DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
			DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
		}	},
		&modeEmpty,
		{
			&stateUnarmedIdle,						// STATE_IDLE
			&stateTeeterIdle,						// STATE_IDLETEETER
			&stateJump,								// STATE_JUMP
			&stateRun,								// STATE_RUN
			&stateFall,								// STATE_FALL
			&stateFallFar,							// STATE_FALLFAR
			NULL,									// STATE_BUTTBOUNCE
			NULL,									// STATE_BUTTFALL
			NULL,									// STATE_BUTTLAND
			NULL,									// STATE_ATTACK
			NULL,									// STATE_RUNATTACK
			NULL,									// STATE_AIRATTACK
			NULL,									// STATE_DUCK
			NULL,									// STATE_SOAKUP
			NULL,									// STATE_GETUP
			&stateDead,								// STATE_DEAD
		}
	},

	//
	// PLAYER_MODE_CORALBLOWER
	// Armed with coral blower ( Heavy weapon so slows SB down )
	//
	{
		{	{
			DEFAULT_PLAYER_JUMP_VELOCITY/3,			// PM__JUMP_VELOCITY
			DEFAULT_PLAYER_MAX_JUMP_FRAMES/2,		// PM__MAX_JUMP_FRAMES
			DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
			DEFAULT_PLAYER_MAX_RUN_VELOCITY/2,		// PM__MAX_RUN_VELOCITY
			DEFAULT_PLAYER_RUN_SPEEDUP/2,			// PM__RUN_SPEEDUP
			DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
			DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
		}	},
		&modeEmpty,
		{
			&stateCoralBlowerIdle,					// STATE_IDLE
			&stateTeeterIdle,						// STATE_IDLETEETER
			&stateJump,								// STATE_JUMP
			&stateRun,								// STATE_RUN
			&stateFall,								// STATE_FALL
			&stateFallFar,							// STATE_FALLFAR
			NULL,									// STATE_BUTTBOUNCE
			NULL,									// STATE_BUTTFALL
			NULL,									// STATE_BUTTLAND
			NULL,									// STATE_ATTACK
			NULL,									// STATE_RUNATTACK
			NULL,									// STATE_AIRATTACK
			NULL,									// STATE_DUCK
			NULL,									// STATE_SOAKUP
			NULL,									// STATE_GETUP
			&stateDead,								// STATE_DEAD
		}
	},

	//
	// PLAYER_MODE_FLY
	// Fly mode ( A useful debugging mode.. )
	//
	{
		{	{
			DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
			DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
			DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
			DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
			DEFAULT_PLAYER_RUN_SPEEDUP,				// PM__RUN_SPEEDUP
			DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
			DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
		}	},
		&modeEmpty,
		{
			&stateFly,								// STATE_IDLE
			&stateFly,								// STATE_IDLETEETER
			&stateFly,								// STATE_JUMP
			&stateFly,								// STATE_RUN
			&stateFly,								// STATE_FALL
			&stateFly,								// STATE_FALLFAR
			&stateFly,								// STATE_BUTTBOUNCE
			&stateFly,								// STATE_BUTTFALL
			&stateFly,								// STATE_BUTTLAND
			&stateFly,								// STATE_ATTACK
			&stateFly,								// STATE_RUNATTACK
			&stateFly,								// STATE_AIRATTACK
			&stateFly,								// STATE_DUCK
			&stateFly,								// STATE_SOAKUP
			&stateFly,								// STATE_GETUP
			&stateFly,								// STATE_DEAD
		}
	},
};


/*===========================================================================
end */

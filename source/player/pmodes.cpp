/*=========================================================================

	pmodes.cpp

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

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
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

CPlayerStateUnarmedIdle			stateUnarmedIdle;
CPlayerStateCoralBlowerIdle		stateCoralBlowerIdle;
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









CPlayer::PlayerMode CPlayer::s_modes[NUM_PLAYERMODES]=
{
	//
	// Basic player mode. No attack, only butt bounce
	//
	{
		{	{
			8,						// PM__JUMP_VELOCITY
			10,						// PM__MAX_JUMP_FRAMES
			20,						// PM__MAX_SAFE_FALL_FRAMES
			4,						// PM__GRAVITY_VALUE
			8,						// PM__TERMINAL_VELOCITY
			8,						// PM__MAX_RUN_VELOCITY
			4,						// PM__RUN_SPEEDUP
			2,						// PM__RUN_REVERSESLOWDOWN
			1,						// PM__RUN_SLOWDOWN
		}	},
		{
			&stateUnarmedIdle,		// STATE_IDLE
			&stateJump,				// STATE_JUMP
			&stateRun,				// STATE_RUN
			&stateFall,				// STATE_FALL
			&stateFallFar,			// STATE_FALLFAR
			&stateButtBounce,		// STATE_BUTTBOUNCE
			&stateButtBounceFall,	// STATE_BUTTFALL
			&stateButtBounceLand,	// STATE_BUTTLAND
			NULL,					// STATE_ATTACK
			NULL,					// STATE_RUNATTACK
			NULL,					// STATE_AIRATTACK
			&stateDuck,				// STATE_DUCK
			&stateSoackUp,			// STATE_SOAKUP
			&stateGetup,			// STATE_GETUP
		}
	},

	//
	// Full unarmed player mode. With butt bounce and karate chop
	//
	{
		{	{
			8,						// PM__JUMP_VELOCITY
			10,						// PM__MAX_JUMP_FRAMES
			20,						// PM__MAX_SAFE_FALL_FRAMES
			4,						// PM__GRAVITY_VALUE
			8,						// PM__TERMINAL_VELOCITY
			8,						// PM__MAX_RUN_VELOCITY
			4,						// PM__RUN_SPEEDUP
			2,						// PM__RUN_REVERSESLOWDOWN
			1,						// PM__RUN_SLOWDOWN
		}	},
		{
			&stateUnarmedIdle,		// STATE_IDLE
			&stateJump,				// STATE_JUMP
			&stateRun,				// STATE_RUN
			&stateFall,				// STATE_FALL
			&stateFallFar,			// STATE_FALLFAR
			&stateButtBounce,		// STATE_BUTTBOUNCE
			&stateButtBounceFall,	// STATE_BUTTFALL
			&stateButtBounceLand,	// STATE_BUTTLAND
			&stateChop,				// STATE_ATTACK
			&stateRunChop,			// STATE_RUNATTACK
			&stateAirChop,			// STATE_AIRATTACK
			&stateDuck,				// STATE_DUCK
			&stateSoackUp,			// STATE_SOAKUP
			&stateGetup,			// STATE_GETUP
		}
	},

	//
	// Armed with coral blower
	//
	{
		{	{
			8,						// PM__JUMP_VELOCITY
			10,						// PM__MAX_JUMP_FRAMES
			20,						// PM__MAX_SAFE_FALL_FRAMES
			4,						// PM__GRAVITY_VALUE
			8,						// PM__TERMINAL_VELOCITY
			4,//8,						// PM__MAX_RUN_VELOCITY
			2,//4,						// PM__RUN_SPEEDUP
			2,						// PM__RUN_REVERSESLOWDOWN
			2,//1,						// PM__RUN_SLOWDOWN
		}	},
		{
			&stateCoralBlowerIdle,	// STATE_IDLE
			NULL,					// STATE_JUMP
			&stateRun,				// STATE_RUN
			&stateFall,				// STATE_FALL
			&stateFallFar,			// STATE_FALLFAR
			NULL,					// STATE_BUTTBOUNCE
			NULL,					// STATE_BUTTFALL
			NULL,					// STATE_BUTTLAND
			NULL,					// STATE_ATTACK
			NULL,					// STATE_RUNATTACK
			NULL,					// STATE_AIRATTACK
			NULL,					// STATE_DUCK
			NULL,					// STATE_SOAKUP
			NULL,					// STATE_GETUP
		}
	},
};


/*===========================================================================
end */

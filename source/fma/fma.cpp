/*=========================================================================

	gameover.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/
/*
Sprite/object control - Thrown objects, so need curved path, :o(
FX

*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "fma\fma.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __THING_THING_H__
#include "thing\thing.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif

#ifndef __GFX_FADER_H__
#include "gfx\fader.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include "actor_spongebob_anim.h"
#endif

#ifndef	__ANIM_MERMAIDMAN_HEADER__
#include "actor_mermaidman_anim.h"
#endif

#ifndef	__ANIM_BARNACLEBOY_HEADER__
#include "actor_barnacleboy_anim.h"
#endif

#ifndef	__ANIM_GARY_HEADER__
#include "actor_gary_anim.h"
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// Available actors
enum
{
	FMA_ACTOR_SPONGEBOB,
	FMA_ACTOR_MM,
	FMA_ACTOR_BB,
	FMA_ACTOR_GARY,

	FMA_NUM_ACTORS
};

// Actor animation types
enum
{
	FMA_ANIM_IDLE,
	FMA_ANIM_WALK,

	FMA_NUM_ANIMS,
};

// Available script commands
typedef enum
{
	SC_SNAP_CAMERA_TO,			// x,y
	SC_MOVE_CAMERA_TO,			// x,y,frames

	SC_REGISTER_CONVERSATION,	// scriptId

	SC_WAIT_ON_TIMER,			// frames						(nonblocking)
	SC_WAIT_ON_ACTOR_STOP,		// actor						(nonblocking)
	SC_WAIT_ON_CAMERA_STOP,		//								(nonblocking)
	SC_WAIT_ON_CONVERSATION,	// scriptId						(nonblocking)
	
	SC_SET_ACTOR_VISIBILITY,	// actor,on/off
	SC_SET_ACTOR_POSITION,		// actor,x,y
	SC_SET_ACTOR_FACING,		// actor,facing
	SC_SET_ACTOR_ANIM_STATE,	// actor,state
	SC_WALK_ACTOR_TO_POSITION,	// actor,x,y,frames

	SC_STOP,					//
} SCRIPT_COMMAND;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Data for an actors graphics
typedef struct
{
	FileEquate		m_file;
	int				m_anims[FMA_NUM_ANIMS];

} ACTOR_GRAPHICS_DATA;

// Actor data
typedef struct
{
	CActorGfx	*m_gfx;
	u8			m_active;

	u8			m_animState,m_animFrame;
	
	DVECTOR		m_pos;
	
	u16			m_startMoveFrame;
	DVECTOR		m_startMovePos;
	u16			m_endMoveFrame;
	DVECTOR		m_endMovePos;
	u8			m_moving;

	u8			m_facing;
} ACTOR_DATA;


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

CFmaScene	FmaScene;



/*****************************************************************************/
/*** Intro FMA ***************************************************************/
/*****************************************************************************/
static const int s_FMAIntroScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH1_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH1_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH1_02_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH1_03_DAT,

// Scene 1 - SB & Gary outside house
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,80*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_GARY,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_GARY,84*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_GARY,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_GARY,true,

	SC_SNAP_CAMERA_TO,			67*16,16*16,

	SC_WAIT_ON_TIMER,			60,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH1_00_DAT,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,60*16,30*16,60*2,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_GARY,FMA_ANIM_WALK,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_GARY,64*16,30*16,60*3,

	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
//	SC_WAIT_ON_TIMER,			60*2,

// Scene 2 - Shady shoals with BB & MM
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,11*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,60*16,30*16,60*2,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,17*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SNAP_CAMERA_TO,			4*16,16*16,
	SC_WAIT_ON_TIMER,			60,

// Scene 3 - SB Arives
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,40*16,30*16,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,24*16,30*16,60*2,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_GARY,44*16,30*16,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_GARY,28*16,30*16,60*3,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_GARY,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_GARY,FMA_ANIM_IDLE,

// Scene 4 - SB talks to em
// Scene 5 - MM gives task
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH1_01_DAT,

// Scene 6 - SB readies to run
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_TIMER,			15,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH1_02_DAT,
	SC_WAIT_ON_TIMER,			15,

// Scene 7 - SB Leaves
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,40*16,30*16,30,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

// Scene 8 - Back to Gary, for non funny end
	SC_SET_ACTOR_FACING,		FMA_ACTOR_GARY,0,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_GARY,30*16,30*16,60,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_GARY,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH1_03_DAT,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_GARY,44*16,30*16,60*2,

	SC_WAIT_ON_TIMER,			60*5,
	SC_STOP
};

/*****************************************************************************/
/*** C1 End FMA **************************************************************/
/*****************************************************************************/
static const int s_FMAC1EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH2_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH2_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH2_02_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH2_03_DAT,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,16*16,
	SC_WAIT_ON_TIMER,			60*2,
// Scene 2 - inside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,207*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,212*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SNAP_CAMERA_TO,			197*16,16*16,
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH2_00_DAT,
	SC_WAIT_ON_TIMER,			30,

// Scene 3 - SB enter by door
	SC_MOVE_CAMERA_TO,			235*16,16*16,60,
	SC_WAIT_ON_CAMERA_STOP,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,270*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,260*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,230*16,30*16,120,
	SC_MOVE_CAMERA_TO,			210*16,16*16,120,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH2_01_DAT,

// scene 6 - sarnie flys thru the air
//	SC_SPRITE_SHIT
	SC_MOVE_CAMERA_TO,			197*16,16*16,120,
	SC_WAIT_ON_CAMERA_STOP,


// Scene 7 - MM Eats sarnie
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,220*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH2_02_DAT,


// Scene 8 - SB Leaves
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_TIMER,			10,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH2_03_DAT,
	SC_WAIT_ON_TIMER,			10,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_STOP
};

/*****************************************************************************/
/*** C2 End FMA **************************************************************/
/*****************************************************************************/
static const int s_FMAC2EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH3_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH3_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH3_02_DAT,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,16*16,
	SC_WAIT_ON_TIMER,			60*2,

// Scene 2 - inside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,207*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,212*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SNAP_CAMERA_TO,			197*16,16*16,
	SC_WAIT_ON_TIMER,			60,

// Scene 3 - SB enter by door
	SC_MOVE_CAMERA_TO,			235*16,16*16,60,
	SC_WAIT_ON_CAMERA_STOP,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,270*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,260*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH3_00_DAT,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,220*16,30*16,200,
	SC_MOVE_CAMERA_TO,			197*16,16*16,200,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,

// Scene 7 - BB gives new task
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH3_01_DAT,


// Scene 8 - SB Leaves
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_TIMER,			10,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH3_02_DAT,
	SC_WAIT_ON_TIMER,			10,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_STOP
};

/*****************************************************************************/
/*** C3 End FMA **************************************************************/
/*****************************************************************************/
static const int s_FMAC3EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH4_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH4_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH4_02_DAT,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,16*16,
	SC_WAIT_ON_TIMER,			30,

// Scene 2 - SB arrives outside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,40*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,16*16,30*16,96,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH4_00_DAT,

// Scene 3 - inside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,207*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,212*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SNAP_CAMERA_TO,			197*16,16*16,
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH4_01_DAT,
	SC_WAIT_ON_TIMER,			30,

// Scene 4 - SB enter by door, and runs over to them

	SC_MOVE_CAMERA_TO,			235*16,16*16,60,
	SC_WAIT_ON_CAMERA_STOP,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,270*16,30*16,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,260*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,220*16,30*16,200,
	SC_MOVE_CAMERA_TO,			197*16,16*16,200,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,

// Scene 5 - MM & BB need to hide here

// Scene 7 - BB gives new task
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH4_02_DAT,


// Scene 8 - SB Leaves
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_TIMER,			20,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

	SC_STOP
};

/*****************************************************************************/
/*** C4 End FMA **************************************************************/
/*****************************************************************************/
static const int s_FMAC4EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH5_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH5_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH5_02_DAT,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,16*16,
	SC_WAIT_ON_TIMER,			30,

// Scene 2 - SB arrives outside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,40*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,16*16,30*16,96,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH5_00_DAT,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH5_01_DAT,
/* NEED PORTHOLE TO FINISH THIS BABY
// Scene 3 - inside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,207*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,212*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,0,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SNAP_CAMERA_TO,			197*16,16*16,
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH4_01_DAT,
	SC_WAIT_ON_TIMER,			30,

// Scene 4 - SB enter by door, and runs over to them

	SC_MOVE_CAMERA_TO,			235*16,16*16,60,
	SC_WAIT_ON_CAMERA_STOP,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,270*16,30*16,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,260*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,220*16,30*16,200,
	SC_MOVE_CAMERA_TO,			197*16,16*16,200,
	SC_WAIT_ON_CAMERA_STOP,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,

// Scene 5 - MM & BB need to hide here

// Scene 7 - BB gives new task
	SC_WAIT_ON_TIMER,			30,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH4_02_DAT,

*/
// Scene 8 - SB Leaves
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_TIMER,			20,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH5_02_DAT,
	SC_WAIT_ON_TIMER,			20,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,40,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

	SC_STOP
};

/*****************************************************************************/
/*** C5 End FMA **************************************************************/
/*****************************************************************************/

static const int s_FMAC5EndScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH6_00_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH6_01_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH6_02_DAT,
	SC_REGISTER_CONVERSATION,	SCRIPTS_FMA_CH6_03_DAT,

// Scene 1 - Shade Shoals
	SC_SNAP_CAMERA_TO,			4*16,16*16,
	SC_WAIT_ON_TIMER,			30,

// Scene 2 - SB arrives outside Shady Shoals
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,40*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_SPONGEBOB,true,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,16*16,30*16,96,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH6_00_DAT,

// Scene 3 - SB Fixing TV
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,224*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_MM,true,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,220*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,1,
	SC_SET_ACTOR_VISIBILITY,	FMA_ACTOR_BB,true,

	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,208*16,30*16,

	SC_SNAP_CAMERA_TO,			197*16,16*16,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_WAIT_ON_TIMER,			10,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_WAIT_ON_TIMER,			10,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_WAIT_ON_TIMER,			10,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH6_01_DAT,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_WAIT_ON_TIMER,			10,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_WAIT_ON_TIMER,			10,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_WAIT_ON_TIMER,			10,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,1,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,


// Scene 4 - Goes to BB, and collects his prize
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,216*16,30*16,16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_IDLE,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH6_02_DAT,
	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,240*16,30*16,154,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

// Scene 5 - SB leaves Shady Shoals
	SC_SNAP_CAMERA_TO,			4*16,16*16,
	SC_WAIT_ON_TIMER,			30,

	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_SPONGEBOB,FMA_ANIM_WALK,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_SPONGEBOB,4*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_SPONGEBOB,0,

	SC_WALK_ACTOR_TO_POSITION,	FMA_ACTOR_SPONGEBOB,40*16,30*16,144,
	SC_WAIT_ON_ACTOR_STOP,		FMA_ACTOR_SPONGEBOB,

// Scene 6 - Back in shady
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_MM,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_MM,207*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_MM,0,
	SC_SET_ACTOR_ANIM_STATE,	FMA_ACTOR_BB,FMA_ANIM_IDLE,
	SC_SET_ACTOR_POSITION,		FMA_ACTOR_BB,212*16,30*16,
	SC_SET_ACTOR_FACING,		FMA_ACTOR_BB,1,
	SC_SNAP_CAMERA_TO,			197*16,16*16,
	SC_WAIT_ON_TIMER,			60,

// Scene 7 - TV goes pop

// Scene 8 - Outside, MM & BB SCREEEEEEEEEEEEEEEAM
	SC_SNAP_CAMERA_TO,			4*16,16*16,
	SC_WAIT_ON_CONVERSATION,	SCRIPTS_FMA_CH6_03_DAT,

	SC_STOP
};


// Actor graphics data
static const ACTOR_GRAPHICS_DATA	s_actorGraphicsData[FMA_NUM_ACTORS]=
{
	{	ACTORS_SPONGEBOB_SBK,			{	ANIM_SPONGEBOB_IDLEBREATH,		ANIM_SPONGEBOB_RUN				}	},	// ACTOR_SPONGEBOB
	{	ACTORS_MERMAIDMAN_SBK,			{	ANIM_MERMAIDMAN_IDLEBREATHE,	ANIM_MERMAIDMAN_IDLEBREATHE		}	},	// ACTOR_MM
	{	ACTORS_BARNACLEBOY_SBK,			{	ANIM_BARNACLEBOY_IDLEBREATHE,	ANIM_BARNACLEBOY_IDLEBREATHE	}	},	// ACTOR_BB
	{	ACTORS_GARY_SBK,				{	ANIM_GARY_IDLE,					ANIM_GARY_IDLE/*ANIM_GARY_SLITHER*/				}	},	// ACTOR_GARY
};


ACTOR_DATA		m_actorData[FMA_NUM_ACTORS];


static const int	*s_fmaScripts[CFmaScene::NUM_FMA_SCRIPTS]=
{
	s_FMAIntroScript,
	s_FMAC1EndScript,
	s_FMAC2EndScript,
	s_FMAC3EndScript,
	s_FMAC4EndScript,
	s_FMAC5EndScript,
};

static CFmaScene::FMA_SCRIPT_NUMBER	s_chosenScript=CFmaScene::FMA_SCRIPT__INTRO;



#include "pad\pads.h"
#include "gfx\font.h"


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::init()
{
	int			i;
	ACTOR_DATA	*actor;


	CThingManager::init();
	CConversation::init();

	m_level=new ("FMALevel") CLevel();
	m_level->init(25);

	m_cameraPos.vx=30;
	m_cameraPos.vy=280;
	m_cameraMoving=false;
	m_readyToShutdown=false;

	CActorPool::Reset();
	actor=m_actorData;
	for(i=0;i<FMA_NUM_ACTORS;i++)
	{
		actor->m_gfx=CActorPool::GetActor(s_actorGraphicsData[i].m_file);
		actor->m_active=false;
		actor->m_animState=FMA_ANIM_IDLE;
		actor->m_animFrame=0;
		actor->m_pos.vx=0;
		actor->m_pos.vy=0;
		actor->m_moving=false;
		actor->m_facing=0;
		actor++;
	}
	CActorPool::SetUpCache();
	CActorPool::CleanUpCache();

	m_frameCount=0;

	m_scriptRunning=true;
	m_pc=s_fmaScripts[s_chosenScript];

	CFader::setFadingIn();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::shutdown()
{
	for(int i=0;i<FMA_NUM_ACTORS;i++)
	{
		delete m_actorData[i].m_gfx;
	}
	CActorPool::Reset();

	m_level->shutdown();	delete m_level;
	CSoundMediator::dumpSong();

	CConversation::shutdown();
	CThingManager::shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::render()
{
	int			i;
	ACTOR_DATA	*actor;

	CThingManager::renderAllThings();
	CConversation::render();
	m_level->render();

	actor=m_actorData;
	for(i=0;i<FMA_NUM_ACTORS;i++)
	{
		if(actor->m_active)
		{
			// need clipping here
			DVECTOR	pos;
			pos.vx=actor->m_pos.vx-m_cameraPos.vx;
			pos.vy=actor->m_pos.vy-m_cameraPos.vy;
			actor->m_gfx->Render(pos,s_actorGraphicsData[i].m_anims[actor->m_animState],actor->m_animFrame,actor->m_facing);
		}
		actor++;
	}

	CActorPool::CleanUpCache();

#if defined (__USER_paul__) || defined (__USER_daveo__)
FontBank	*font;
char		buf[20];
font=new ("Font") FontBank();
font->initialise(&standardFont);
sprintf(buf,"%d,%d",m_cameraPos.vx,m_cameraPos.vy);
font->print(400,30,buf);
font->dump();
delete font;
#endif
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::think(int _frames)
{
#if defined (__USER_paul__) || defined (__USER_daveo__)
	if(PadGetHeld(0)&PAD_UP)
	{
		m_cameraPos.vy-=16;
	}
	else if(PadGetHeld(0)&PAD_DOWN)
	{
		m_cameraPos.vy+=16;
	}
	else if(PadGetHeld(0)&PAD_LEFT)
	{
		m_cameraPos.vx-=16;
	}
	else if(PadGetHeld(0)&PAD_RIGHT)
	{
		m_cameraPos.vx+=16;
	}
#endif
	if(m_scriptRunning==false&&!m_readyToShutdown)
	{
		startShutdown();
	}

	for(int i=0;i<_frames;i++)
	{
		if(m_scriptRunning)
		{
			int			i;
			ACTOR_DATA	*actor;

			actor=m_actorData;
			for(i=0;i<FMA_NUM_ACTORS;i++)
			{
				if(actor->m_active)
				{
					// Move actor?
					if(actor->m_moving)
					{
						int	totalFrames,currentFrame;
						totalFrames=actor->m_endMoveFrame-actor->m_startMoveFrame;
						currentFrame=totalFrames-(actor->m_endMoveFrame-m_frameCount);
						if(currentFrame==0)
						{
							actor->m_pos=actor->m_startMovePos;
						}
						else if(currentFrame==totalFrames)
						{
							actor->m_pos=actor->m_endMovePos;
							actor->m_moving=false;
						}
						else
						{
							actor->m_pos.vx=actor->m_startMovePos.vx+(((actor->m_endMovePos.vx-actor->m_startMovePos.vx)*currentFrame)/totalFrames);
							actor->m_pos.vy=actor->m_startMovePos.vy+(((actor->m_endMovePos.vy-actor->m_startMovePos.vy)*currentFrame)/totalFrames);
						}
					}
				}

				// Anim
				actor->m_animFrame++;
				if(actor->m_animFrame>=actor->m_gfx->getFrameCount(s_actorGraphicsData[i].m_anims[actor->m_animState]))
				{
					actor->m_animFrame=0;
				}

				actor++;
			}

			// Move Camera
			if(m_cameraMoving)
			{
				int	totalFrames,currentFrame;
				totalFrames=m_endCameraFrame-m_startCameraFrame;
				currentFrame=totalFrames-(m_endCameraFrame-m_frameCount);
				if(currentFrame==0)
				{
					m_cameraPos=m_startCameraPos;
				}
				else if(currentFrame==totalFrames)
				{
					m_cameraPos=m_endCameraPos;
					m_cameraMoving=false;
				}
				else
				{
					m_cameraPos.vx=m_startCameraPos.vx+(((m_endCameraPos.vx-m_startCameraPos.vx)*currentFrame)/totalFrames);
					m_cameraPos.vy=m_startCameraPos.vy+(((m_endCameraPos.vy-m_startCameraPos.vy)*currentFrame)/totalFrames);
				}
			}

			// Process script
			do
			{
				if(!m_stillProcessingCommand)
				{
					startNextScriptCommand();
				}
				if(m_stillProcessingCommand)
				{
					processCurrentScriptCommand();
				}
			}
			while(!m_doOtherProcessing);
		}

		m_frameCount++;
	}

	CThingManager::thinkAllThings(_frames);
	CConversation::think(_frames);
	m_level->setCameraCentre(m_cameraPos);
	m_level->think(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CFmaScene::readyToShutdown()
{
	return m_readyToShutdown&&!CFader::isFading();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::selectFma(FMA_SCRIPT_NUMBER _fma)
{
	s_chosenScript=_fma;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::startShutdown()
{
	CFader::setFadingOut();
	GameState::setNextScene(&MapScene);
	m_readyToShutdown=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::startNextScriptCommand()
{
	m_stillProcessingCommand=false;
	m_doOtherProcessing=false;

	switch(*m_pc)
	{
		case SC_SNAP_CAMERA_TO:			// x,y
			m_pc++;
			m_cameraPos.vx=*m_pc++;
			m_cameraPos.vy=*m_pc++;
			break;

		case SC_MOVE_CAMERA_TO:			// x,y,frames
			m_pc++;
			m_cameraMoving=true;
			m_startCameraPos=m_cameraPos;
			m_startCameraFrame=m_frameCount;
			m_endCameraPos.vx=*m_pc++;
			m_endCameraPos.vy=*m_pc++;
			m_endCameraFrame=m_frameCount+*m_pc++;
			break;
	
		case SC_REGISTER_CONVERSATION:	// scriptId
			m_pc++;
			CConversation::registerConversationScript((FileEquate)*(m_pc++));
			break;

		case SC_WAIT_ON_TIMER:			// frames
			m_timer=*(m_pc+1);
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_ACTOR_STOP:		// actor
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_CAMERA_STOP:	//
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_CONVERSATION:	// scriptId
			CConversation::trigger((FileEquate)*(m_pc+1));
			m_stillProcessingCommand=true;
			break;

		case SC_SET_ACTOR_VISIBILITY:	// actor,on/off
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_active=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_POSITION:		// actor,x,y
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_pos.vx=*m_pc++;
				actor->m_pos.vy=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_FACING:		// actor,facing
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_facing=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_ANIM_STATE:	// actor,state
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_animState=*m_pc++;
			}
			break;

		case SC_WALK_ACTOR_TO_POSITION:	// actor,x,y,frames
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_startMoveFrame=m_frameCount;
				actor->m_startMovePos=actor->m_pos;
				actor->m_endMovePos.vx=*m_pc++;
				actor->m_endMovePos.vy=*m_pc++;
				actor->m_endMoveFrame=m_frameCount+*m_pc++;
				actor->m_moving=true;
			}
			break;

		case SC_STOP:					//
			m_scriptRunning=false;
			m_doOtherProcessing=true;
			break;

		default:
			ASSERT(!"Bad script command");
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::processCurrentScriptCommand()
{
	switch(*m_pc)
	{
		case SC_SNAP_CAMERA_TO:			// x,y
		case SC_MOVE_CAMERA_TO:			// x,y,frames
		case SC_REGISTER_CONVERSATION:	// scriptId
		case SC_SET_ACTOR_VISIBILITY:	// actor,on/off
		case SC_SET_ACTOR_POSITION:		// actor,x,y
		case SC_SET_ACTOR_FACING:		// actor,facing
		case SC_SET_ACTOR_ANIM_STATE:	// actor,state
		case SC_WALK_ACTOR_TO_POSITION:	// actor,x,y,frames
		case SC_STOP:					//
			ASSERT(!"Shouldn't be here..");
			break;

		case SC_WAIT_ON_TIMER:			// frames
			if(m_timer--==0)
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;
		
		case SC_WAIT_ON_ACTOR_STOP:		// actor
			if(!m_actorData[*(m_pc+1)].m_moving)
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;

		case SC_WAIT_ON_CAMERA_STOP:	//
			if(!m_cameraMoving)
			{
				m_pc++;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;


		case SC_WAIT_ON_CONVERSATION:	// scriptId
			if(!CConversation::isActive())
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;

		default:
			ASSERT(!"Bad script command");
			break;
	}
}


/*===========================================================================
 end */
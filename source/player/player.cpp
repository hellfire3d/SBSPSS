/*=========================================================================

	player.cpp

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

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

#ifndef	__PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif

#ifndef	__PLAYER_PMBLOON_H__
#include "player\pmbloon.h"
#endif

#ifndef	__PLAYER_PMBUBBLE_H__
#include "player\pmbubble.h"
#endif

#ifndef	__PLAYER_PMNET_H__
#include "player\pmnet.h"
#endif

#ifndef	__PLAYER_PMCHOP_H__
#include "player\pmchop.h"
#endif

#ifndef	__PLAYER_PMCORAL_H__
#include "player\pmcoral.h"
#endif

#ifndef	__PLAYER_PMDEAD_H__
#include "player\pmdead.h"
#endif

#ifndef	__PLAYER_PMFLY_H__
#include "player\pmfly.h"
#endif

#ifndef	__PLAYER_PMJELLY_H__
#include "player\pmjelly.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef	__PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_Anim.h>
#endif

#ifndef	__ANIM_SPONGEBOB_NET_HEADER__
#include <ACTOR_SPONGEBOB_NET_Anim.h>
#endif

#ifndef	__ANIM_SPONGEBOB_CORALBLOWER_HEADER__
#include <ACTOR_SPONGEBOB_CORALBLOWER_Anim.h>
#endif

#ifndef	__ANIM_SPONGEBOB_JELLYLAUNCHER_HEADER__
#include <ACTOR_SPONGEBOB_JELLYLAUNCHER_Anim.h>
#endif

#ifndef	__ANIM_SPONGEBOB_GLASSES_HEADER__
#include <ACTOR_SPONGEBOB_GLASSES_Anim.h>
#endif

//#ifndef	__ANIM_SPONGEBOB_BUBBLEWAND_HEADER__
//#include <ACTOR_SPONGEBOB_BUBBLEWAND_Anim.h>
//#endif

#ifndef	__ANIM_SPONGEBOB_JELLYFISH_HEADER__
#include <ACTOR_SPONGEBOB_JELLYFISH_Anim.h>
#endif

#ifndef	__ANIM_SPONGEBOB_GLOVE_HEADER__
#include <ACTOR_SPONGEBOB_GLOVE_Anim.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

//#define _RECORD_DEMO_MODE_
#define _STATE_DEBUG_


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Two dice.  One says 'Re' on every face, the other says 'boot',
// 'install', 'try', 'tire', 'sume' and 'number'

/*
WEAPON MODES
	unamred				constant
	karate-chop			constant
	balloon				timed ( respawn )
	bubble mixture		(un)limited supply ( respawn )
	helmet				constant ( respawn )
	coral blower		constant ( respawn )
	net					constant
	jelly launcher		limited supply ( respawn )

POWER-UPS
	glasses				constant
	squeaky boots		timed ( respawn )
	mm & bb ring		timed
*/

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

#ifdef _RECORD_DEMO_MODE_

#include "player\demoplay.h"

#define MAX_DEMO_SIZE			512			// So max size of a demo is 1k
#define MAX_DEMO_TIME_IN_FRAMES	30*60		// Recorded demo will last 30 seconds

static CDemoPlayer::demoPlayerControl s_demoControls[MAX_DEMO_SIZE]={{PI_NONE,0}};
static int s_demoSize=0;
static int s_demoFrameCount=0;

static void writeDemoControls()
{
	char	filename[32];
	int		fh;
	int		fc=MAX_DEMO_TIME_IN_FRAMES;

	sprintf(filename,"demo____.dmo");
	fh=PCcreat((char *)filename,0);
	ASSERT(fh!=-1);
	PCwrite(fh,(char*)&fc,sizeof(fc));													// frame count
	PCwrite(fh,(char*)&s_demoSize,sizeof(s_demoSize));									// demo size
	for(int i=0;i<s_demoSize;i++)
		PCwrite(fh,(char*)&s_demoControls[i],sizeof(CDemoPlayer::demoPlayerControl));	// control data
	PCclose(fh);

	SYSTEM_DBGMSG("Written demo file %s with %d frames",filename,s_demoSize);
}
#endif


#ifdef _STATE_DEBUG_
static const char *s_modeText[NUM_PLAYERMODES]=
{
	"BASICUNARMED",
	"FULLUNARMED",
	"BALLOON",
	"BUBBLE MIXTURE",
	"NET",
	"CORALBLOWER",
	"JELLY LAUNCHER",
	"DEAD",
	"FLY",
};
#endif


int		s_screenPos;
int		m_cameraLookOffset;

int MAP2D_CENTRE_X=-256;
int MAP2D_CENTRE_Y=-170;
int MAP2D_BLOCKSTEPSIZE=16;

CPlayerModeBase				PLAYERMODE;
CPlayerModeChop				PLAYERMODECHOP;
CPlayerModeBalloon			PLAYERMODEBALLOON;
CPlayerModeBubbleMixture	PLAYERMODEBUBBLEMIXTURE;
CPlayerModeNet				PLAYERMODENET;
CPlayerModeCoralBlower		PLAYERMODECORALBLOWER;
CPlayerModeJellyLauncher	PLAYERMODEJELLYLAUNCHER;
CPlayerModeDead				PLAYERMODEDEAD;
CPlayerModeFly				PLAYERMODEFLY;

CPlayerMode	*CPlayer::s_playerModes[NUM_PLAYERMODES]=
{
	&PLAYERMODE,				// PLAYER_MODE_BASICUNARMED
	&PLAYERMODECHOP,			// PLAYER_MODE_FULLUNARMED
	&PLAYERMODEBALLOON,			// PLAYER_MODE_BALLOON
	&PLAYERMODEBUBBLEMIXTURE,	// PLAYER_MODE_BUBBLE_MIXTURE
	&PLAYERMODENET,				// PLAYER_MODE_NET
	&PLAYERMODECORALBLOWER,		// PLAYER_MODE_CORALBLOWER
	&PLAYERMODEJELLYLAUNCHER,	// PLAYER_MODE_JELLY_LAUNCHER
	&PLAYERMODEDEAD,			// PLAYER_MODE_DEAD
	&PLAYERMODEFLY,				// PLAYER_MODE_FLY
};


// A big bunch of 'temporary' variables for tweaking things
// This #def makes them static under a release build..
#ifdef __VERSION_DEBUG__
#define		pint		int
#else
#define		pint		static const int
#endif

pint	sbanimspeed=0;

pint	looktimeout=20;
pint	lookmaxoffsetup=3*MAP2D_BLOCKSTEPSIZE;
pint	lookmaxoffsetdown=6*MAP2D_BLOCKSTEPSIZE;
pint	lookspeed=2;
pint	lookreturnspeed=5;

pint	ledgeTimer=50;
pint	ledgeSpeedIn=1;
pint	ledgeSpeedOut=3;
pint	ledgeShift=1;

pint	cammove=2;

pint	waterDrainSpeed=4;
pint	waterSoakUpSpeed=40;



// --------------------------------- Addon stuff ---------------------------------
CActorGfx		*s_addonActorGfx[NUM_PLAYER_ADDONS]=
{
	NULL,										// PLAYER_ADDON_NET
	NULL,										// PLAYER_ADDON_CORALBLOWER
	NULL,										// PLAYER_ADDON_JELLYLAUNCHER
	NULL,										// PLAYER_ADDON_GLASSES
	NULL,										// PLAYER_ADDON_BUBBLEWAND
	NULL,										// PLAYER_ADDON_JELLYFISHINNET
	NULL,										// PLAYER_ADDON_GLOVE
};
FileEquate		s_addonActorPoolNames[NUM_PLAYER_ADDONS]=
{
	ACTORS_SPONGEBOB_NET_SBK,					// PLAYER_ADDON_NET
	ACTORS_SPONGEBOB_CORALBLOWER_SBK,			// PLAYER_ADDON_CORALBLOWER
	ACTORS_SPONGEBOB_JELLYLAUNCHER_SBK,			// PLAYER_ADDON_JELLYLAUNCHER
	ACTORS_SPONGEBOB_GLASSES_SBK,				// PLAYER_ADDON_GLASSES
	(FileEquate)-1,					// PLAYER_ADDON_BUBBLEWAND
	ACTORS_SPONGEBOB_JELLYFISH_SBK,				// PLAYER_ADDON_JELLYFISHINNET
	ACTORS_SPONGEBOB_GLOVE_SBK,					// PLAYER_ADDON_GLOVE
};
PLAYER_ADDONS	s_addonNumbers[NUM_PLAYERMODES]=
{
	NO_ADDON,									//	PLAYER_MODE_BASICUNARMED
	NO_ADDON,									//	PLAYER_MODE_FULLUNARMED
	NO_ADDON,									//	PLAYER_MODE_BALLOON
	PLAYER_ADDON_BUBBLEWAND,					//	PLAYER_MODE_BUBBLE_MIXTURE
	PLAYER_ADDON_NET,							//	PLAYER_MODE_NET
	PLAYER_ADDON_CORALBLOWER,					//	PLAYER_MODE_CORALBLOWER
	PLAYER_ADDON_JELLYLAUNCHER,					//	PLAYER_MODE_JELLY_LAUNCHER
	NO_ADDON,									//	PLAYER_MODE_DEAD
	NO_ADDON,									//	PLAYER_MODE_FLY
};

static s8 s_animMapNet[NUM_PLAYER_ADDONS][NUM_ANIM_SPONGEBOB]=
{
	//	PLAYER_ADDON_NET,
	{
	ANIM_SPONGEBOB_NET_BUTTBOUNCEEND,				// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	ANIM_SPONGEBOB_NET_BUTTBOUNCESTART,				// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_NET_FALL,						// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_NET_GETUP,						// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_NET_HITGROUND,					// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_NET_IDLEBREATH,					// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_NET_IDLEWIND,					// ANIM_SPONGEBOB_IDLEWIND,
	ANIM_SPONGEBOB_NET_JUMPEND,						// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_NET_JUMP,						// ANIM_SPONGEBOB_JUMP,
	ANIM_SPONGEBOB_NET_RUN,							// ANIM_SPONGEBOB_RUN,
	ANIM_SPONGEBOB_NET_RUNSTOP,						// ANIM_SPONGEBOB_RUNSTOP,
	ANIM_SPONGEBOB_NET_RUNSTART,					// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_NET_SOAKUP,						// ANIM_SPONGEBOB_SOAKUP,
	ANIM_SPONGEBOB_NET_TEETERBACK,					// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_NET_TEETERFRONT,					// ANIM_SPONGEBOB_TEETERFRONT,
	ANIM_SPONGEBOB_NET_SWIPE,						// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_NET_DEATHSPIN,					// ANIM_SPONGEBOB_DEATHSPIN,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	-1,												// ANIM_SPONGEBOB_FIREEND,
	-1,												// ANIM_SPONGEBOB_FIRESTART,
	-1,												// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_NET_GETHIT,						// ANIM_SPONGEBOB_GETHIT
	},

	//	PLAYER_ADDON_CORALBLOWER,
	{
	-1,												// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	-1,												// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_CORALBLOWER_FALL,				// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_CORALBLOWER_GETUP,				// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_CORALBLOWER_HITGROUND,			// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_CORALBLOWER_IDLEBREATH,			// ANIM_SPONGEBOB_IDLEBREATH,
	-1,												// ANIM_SPONGEBOB_IDLEWIND,
	ANIM_SPONGEBOB_CORALBLOWER_JUMPEND,				// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_CORALBLOWER_JUMP,				// ANIM_SPONGEBOB_JUMP,
	-1,												// ANIM_SPONGEBOB_RUN,
	-1,												// ANIM_SPONGEBOB_RUNSTOP,
	-1,												// ANIM_SPONGEBOB_RUNSTART,
	-1,												// ANIM_SPONGEBOB_SOAKUP,
	ANIM_SPONGEBOB_CORALBLOWER_TEETERBACK,			// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_CORALBLOWER_TEETERFRONT,			// ANIM_SPONGEBOB_TEETERFRONT,
	-1,												// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_CORALBLOWER_DEATHSPIN,			// ANIM_SPONGEBOB_DEATHSPIN,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	ANIM_SPONGEBOB_CORALBLOWER_FIREEND,				// ANIM_SPONGEBOB_FIREEND,
	ANIM_SPONGEBOB_CORALBLOWER_FIRESTART,			// ANIM_SPONGEBOB_FIRESTART,
	ANIM_SPONGEBOB_CORALBLOWER_IDLEWEAPON,			// ANIM_SPONGEBOB_IDLEWEAPON,
	ANIM_SPONGEBOB_CORALBLOWER_WALK,				// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_CORALBLOWER_GETHIT,				// ANIM_SPONGEBOB_GETHIT
	},

	// PLAYER_ADDON_JELLYLAUNCHER,
	{
	ANIM_SPONGEBOB_JELLYLAUNCHER_BUTTBOUNCEEND,		// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_BUTTBOUNCESTART,	// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_JELLYLAUNCHER_FALL,				// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_JELLYLAUNCHER_GETUP,				// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_JELLYLAUNCHER_HITGROUND,			// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_IDLEBREATH,		// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_JELLYLAUNCHER_IDLEWIND,			// ANIM_SPONGEBOB_IDLEWIND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_JUMPEND,			// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_JUMP,				// ANIM_SPONGEBOB_JUMP,
	ANIM_SPONGEBOB_JELLYLAUNCHER_RUN,				// ANIM_SPONGEBOB_RUN,
	ANIM_SPONGEBOB_JELLYLAUNCHER_RUNSTOP,			// ANIM_SPONGEBOB_RUNSTOP,
	ANIM_SPONGEBOB_JELLYLAUNCHER_RUNSTART,			// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_JELLYLAUNCHER_SOAKUP,			// ANIM_SPONGEBOB_SOAKUP,
	ANIM_SPONGEBOB_JELLYLAUNCHER_TEETERBACK,		// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_JELLYLAUNCHER_TEETERFRONT,		// ANIM_SPONGEBOB_TEETERFRONT,
	-1,												// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_JELLYLAUNCHER_DEATHSPIN,			// ANIM_SPONGEBOB_DEATHSPIN,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	ANIM_SPONGEBOB_JELLYLAUNCHER_FIREEND,			// ANIM_SPONGEBOB_FIREEND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_FIRESTART,			// ANIM_SPONGEBOB_FIRESTART,
	-1,												// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_JELLYLAUNCHER_GETHIT,			// ANIM_SPONGEBOB_GETHIT
	},

	// PLAYER_ADDON_GLASSES,
	{
	ANIM_SPONGEBOB_GLASSES_BUTTBOUNCEEND,			// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	ANIM_SPONGEBOB_GLASSES_BUTTBOUNCESTART,			// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_GLASSES_FALL,					// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_GLASSES_GETUP,					// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_GLASSES_HITGROUND,				// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_GLASSES_IDLEBREATH,				// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_GLASSES_IDLEWIND,				// ANIM_SPONGEBOB_IDLEWIND,
	ANIM_SPONGEBOB_GLASSES_JUMPEND,					// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_GLASSES_JUMP,					// ANIM_SPONGEBOB_JUMP,
	ANIM_SPONGEBOB_GLASSES_RUN,						// ANIM_SPONGEBOB_RUN,
	ANIM_SPONGEBOB_GLASSES_RUNSTOP,					// ANIM_SPONGEBOB_RUNSTOP,
	ANIM_SPONGEBOB_GLASSES_RUNSTART,				// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_GLASSES_SOAKUP,					// ANIM_SPONGEBOB_SOAKUP,
	ANIM_SPONGEBOB_GLASSES_TEETERBACK,				// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_GLASSES_TEETERFRONT,				// ANIM_SPONGEBOB_TEETERFRONT,
	ANIM_SPONGEBOB_GLASSES_SWIPE,					// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_GLASSES_DEATHSPIN,				// ANIM_SPONGEBOB_DEATHSPIN,
	ANIM_SPONGEBOB_GLASSES_BALLOONJUMP,				// ANIM_SPONGEBOB_BALLOONJUMP,
	ANIM_SPONGEBOB_GLASSES_BLOWBUBBLE,				// ANIM_SPONGEBOB_BLOWBUBBLE,
	ANIM_SPONGEBOB_GLASSES_FIREEND,					// ANIM_SPONGEBOB_FIREEND,
	ANIM_SPONGEBOB_GLASSES_FIRESTART,				// ANIM_SPONGEBOB_FIRESTART,
	ANIM_SPONGEBOB_GLASSES_IDLEWEAPON,				// ANIM_SPONGEBOB_IDLEWEAPON,
	ANIM_SPONGEBOB_GLASSES_WALK,					// ANIM_SPONGEBOB_WALK,
	ANIM_SPONGEBOB_GLASSES_KARATE,					// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_GLASSES_GETHIT,					// ANIM_SPONGEBOB_GETHIT
	},

	// PLAYER_ADDON_BUBBLEWAND,
	{
	-1,		// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	-1,		// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	-1,				// ANIM_SPONGEBOB_FALL,
	-1,				// ANIM_SPONGEBOB_GETUP,
	-1,			// ANIM_SPONGEBOB_HITGROUND,
	-1,			// ANIM_SPONGEBOB_IDLEBREATH,
	-1,			// ANIM_SPONGEBOB_IDLEWIND,
	-1,				// ANIM_SPONGEBOB_JUMPEND,
	-1,				// ANIM_SPONGEBOB_JUMP,
	-1,					// ANIM_SPONGEBOB_RUN,
	-1,				// ANIM_SPONGEBOB_RUNSTOP,
	-1,			// ANIM_SPONGEBOB_RUNSTART,
	-1,				// ANIM_SPONGEBOB_SOAKUP,
	-1,			// ANIM_SPONGEBOB_TEETERBACK,
	-1,			// ANIM_SPONGEBOB_TEETERFRONT,
	-1,				// ANIM_SPONGEBOB_SWIPE,
	-1,			// ANIM_SPONGEBOB_DEATHSPIN,
	-1,										// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,										// ANIM_SPONGEBOB_BLOWBUBBLE,
	-1,										// ANIM_SPONGEBOB_FIREEND,
	-1,										// ANIM_SPONGEBOB_FIRESTART,
	-1,										// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,										// ANIM_SPONGEBOB_WALK,
	-1,										// ANIM_SPONGEBOB_KARATE,
	-1,					// ANIM_SPONGEBOB_GETHIT
	},

	// PLAYER_ADDON_JELLYFISHINNET
	{
	ANIM_SPONGEBOB_JELLYFISH_BUTTBOUNCEEND,			// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	ANIM_SPONGEBOB_JELLYFISH_BUTTBOUNCESTART,		// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_JELLYFISH_FALL,					// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_JELLYFISH_GETUP,					// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_JELLYFISH_HITGROUND,				// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_JELLYFISH_IDLEBREATH,			// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_JELLYFISH_IDLEWIND,				// ANIM_SPONGEBOB_IDLEWIND,
	ANIM_SPONGEBOB_JELLYFISH_JUMPEND,				// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_JELLYFISH_JUMP,					// ANIM_SPONGEBOB_JUMP,
	ANIM_SPONGEBOB_JELLYFISH_RUN,					// ANIM_SPONGEBOB_RUN,
	ANIM_SPONGEBOB_JELLYFISH_RUNSTOP,				// ANIM_SPONGEBOB_RUNSTOP,
	ANIM_SPONGEBOB_JELLYFISH_RUNSTART,				// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_JELLYFISH_SOAKUP,				// ANIM_SPONGEBOB_SOAKUP,
	ANIM_SPONGEBOB_JELLYFISH_TEETERBACK,			// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_JELLYFISH_TEETERFRONT,			// ANIM_SPONGEBOB_TEETERFRONT,
	-1,//ANIM_SPONGEBOB_JELLYFISH_SWIPE,					// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_JELLYFISH_DEATHSPIN,				// ANIM_SPONGEBOB_DEATHSPIN,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	-1,												// ANIM_SPONGEBOB_FIREEND,
	-1,												// ANIM_SPONGEBOB_FIRESTART,
	-1,												// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_JELLYFISH_GETHIT,				// ANIM_SPONGEBOB_GETHIT
	},

	// PLAYER_ADDON_GLOVE
	{
	-1,												// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	-1,												// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	-1,												// ANIM_SPONGEBOB_FALL,
	-1,												// ANIM_SPONGEBOB_GETUP,
	-1,												// ANIM_SPONGEBOB_HITGROUND,
	-1,												// ANIM_SPONGEBOB_IDLEBREATH,
	-1,												// ANIM_SPONGEBOB_IDLEWIND,
	-1,												// ANIM_SPONGEBOB_JUMPEND,
	-1,												// ANIM_SPONGEBOB_JUMP,
	-1,												// ANIM_SPONGEBOB_RUN,
	-1,												// ANIM_SPONGEBOB_RUNSTOP,
	-1,												// ANIM_SPONGEBOB_RUNSTART,
	-1,												// ANIM_SPONGEBOB_SOAKUP,
	-1,												// ANIM_SPONGEBOB_TEETERBACK,
	-1,												// ANIM_SPONGEBOB_TEETERFRONT,
	-1,												// ANIM_SPONGEBOB_SWIPE,
	-1,												// ANIM_SPONGEBOB_DEATHSPIN,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	-1,												// ANIM_SPONGEBOB_FIREEND,
	-1,												// ANIM_SPONGEBOB_FIRESTART,
	-1,												// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	ANIM_SPONGEBOB_GLOVE_KARATE,					// ANIM_SPONGEBOB_KARATE,
	-1,												// ANIM_SPONGEBOB_GETHIT
	},
};

// -------------------------------------------------------------------------------




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::init()
{
	int	i;

	CPlayerThing::init();

	m_fontBank=new ("PlayerFont") FontBank();
	m_fontBank->initialise(&standardFont);
	m_fontBank->setOt(5);

	m_spriteBank=new ("PlayerSprites") SpriteBank();
	m_spriteBank->load(SPRITES_SPRITES_SPR);

	m_layerCollision=NULL;

	m_actorGfx=CActorPool::GetActor(ACTORS_SPONGEBOB_SBK);

	for(i=0;i<NUM_PLAYERMODES;i++)
	{
		s_playerModes[i]->initialise(this);
	}
	m_currentPlayerModeClass=NULL;
	setMode(PLAYER_MODE_FULLUNARMED);	//PKG

m_animNo=0;
m_animFrame=0;
	setFacing(FACING_RIGHT);
	m_lives++;respawn();

	m_lives=CGameSlotManager::getSlotData()->m_lives;

	m_lastPadInput=m_padInput=PI_NONE;

	s_screenPos=128;

	resetPlayerCollisionSizeToBase();

	m_divingHelmet=false;
	setIsInWater(true);

	// Always ( cept for one level ) need this
	registerAddon(PLAYER_ADDON_GLOVE);
//	registerAddon(PLAYER_ADDON_NET);
//	registerAddon(PLAYER_ADDON_CORALBLOWER);
//	registerAddon(PLAYER_ADDON_JELLYLAUNCHER);
//	registerAddon(PLAYER_ADDON_GLASSES);
//	registerAddon(PLAYER_ADDON_BUBBLEWAND);
//	registerAddon(PLAYER_ADDON_JELLYFISHINNET);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::shutdown()
{
	int	i;

	for(i=0;i<NUM_PLAYERMODES;i++)
	{
		s_playerModes[i]->shutdown();
	}

	for(i=0;i<NUM_PLAYER_ADDONS;i++)
	{
		if(s_addonActorGfx[i])
		{
			delete s_addonActorGfx[i];
			s_addonActorGfx[i]=NULL;
		}
	}
	delete m_actorGfx;

	m_spriteBank->dump();	delete m_spriteBank;
	m_fontBank->dump();		delete m_fontBank;

	CPlayerThing::shutdown();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int oldmode=-1;
int newmode=-1;

int	scmax=160;
int scspeed=5;

void	CPlayer::think(int _frames)
{
	int	i;

	ASSERT(!(getIsInWater()==false&&isWearingDivingHelmet()==false));

	if(isWearingDivingHelmet()&&getIsInWater()==false&&
	   m_currentMode!=PLAYER_MODE_DEAD&&m_currentMode!=PLAYER_MODE_FLY)
	{
		m_healthWaterLevel-=waterDrainSpeed*_frames;
		if(m_healthWaterLevel<=0)
		{
			dieYouPorousFreak();
		}
	}

if(PadGetDown(0)&PAD_L1)
{
	oldmode=m_currentMode;
	newmode=PLAYER_MODE_FLY;
}
else if(oldmode!=-1&&!(PadGetHeld(0)&PAD_L1))
{
	newmode=oldmode;
	oldmode=-1;
}


#ifdef __USER_paul__
if(PadGetHeld(0)&PAD_L1&&PadGetHeld(0)&PAD_L2)
{
	if(m_currentMode!=PLAYER_MODE_DEAD)
	{
		dieYouPorousFreak();
	}
}
#endif
if(newmode!=-1)
{
	setMode((PLAYER_MODE)newmode);
	newmode=-1;
}

	CThing	*platform;
	platform=isOnPlatform();
	if(platform)
	{
		DVECTOR	posDelta;
		posDelta=platform->getPosDelta();
		if(((CNpcPlatform*)platform)->getHeightFromPlatformAtPosition(Pos.vx+posDelta.vx,Pos.vy+posDelta.vy)==0)
		{
			shove(posDelta);
		}
	}


	m_allowConversation=false;
	
	if(m_healthReactFrames)
	{
		m_healthReactFrames-=_frames;
		if(m_healthReactFrames<0)
		{
			m_healthReactFrames=0;
		}
	}

	for(i=0;i<_frames;i++)
	{
		// Think
		updatePadInput();

		// Trying to converate?
		if(m_allowConversation==false&&
		   getPadInputDown()&PI_UP)
		{
			m_allowConversation=true;
		}

		m_currentPlayerModeClass->think();

		// Powerups
		if(m_squeakyBootsTimer)
		{
			m_squeakyBootsTimer--;
		}
		if(m_invincibilityRingTimer)
		{
			m_invincibilityRingTimer--;
		}

		// Flashing..
		if(m_invincibleFrameCount)
		{
			m_invincibleFrameCount--;
		}

		// Stop the player vanishing off the edge of the telly..
		if(Pos.vx<m_playerPosLimitBox.x1)		Pos.vx=m_playerPosLimitBox.x1;
		else if(Pos.vx>m_playerPosLimitBox.x2)	Pos.vx=m_playerPosLimitBox.x2;
		if(Pos.vy<m_playerPosLimitBox.y1)		Pos.vy=m_playerPosLimitBox.y1;
		else if(Pos.vy>m_playerPosLimitBox.y2)	Pos.vy=m_playerPosLimitBox.y2;

		// Look around
		int	pad=getPadInputHeld();
		if(pad&PI_UP&&canDoLookAround())
		{
			if(m_padLookAroundTimer>0)
			{
				m_padLookAroundTimer=0;
			}
			else if(m_padLookAroundTimer>-looktimeout)
			{
				m_padLookAroundTimer--;
			}
			else if(m_cameraLookOffset>-lookmaxoffsetup)
			{
				m_cameraLookOffset-=lookspeed;
				if(m_cameraLookOffset<-lookmaxoffsetup)
				{
					m_cameraLookOffset=-lookmaxoffsetup;
				}
			}
		}
		else if(pad&PI_DOWN&&canDoLookAround())
		{
			if(m_padLookAroundTimer<0)
			{
				m_padLookAroundTimer=0;
			}
			else if(m_padLookAroundTimer<looktimeout)
			{
				m_padLookAroundTimer++;
			}
			else if(m_cameraLookOffset<lookmaxoffsetdown)
			{
				m_cameraLookOffset+=lookspeed;
				if(m_cameraLookOffset>lookmaxoffsetdown)
				{
					m_cameraLookOffset=lookmaxoffsetdown;
				}
			}
		}
		else
		{
			m_padLookAroundTimer=0;
		}

		// Return to centre
		if(m_padLookAroundTimer==0&&m_cameraLookOffset<0)
		{
			m_cameraLookOffset+=lookreturnspeed;
			if(m_cameraLookOffset>0)
			{
				m_cameraLookOffset=0;
			}
		}
		if(m_padLookAroundTimer==0&&m_cameraLookOffset>0)
		{
			m_cameraLookOffset-=lookreturnspeed;
			if(m_cameraLookOffset<0)
			{
				m_cameraLookOffset=0;
			}
		}

		// Automatic anim sfx
		playAnimFrameSfx(m_animNo,m_animFrame);
	}

	// Ledge look-ahead stuff
	if(m_ledgeLookAhead&&m_ledgeLookAhead==m_lastLedgeLookAhead)
	{
		if(m_ledgeLookTimer<ledgeTimer)
		{
			m_ledgeLookTimer+=_frames;
		}
		else
		{
			int limit;
			limit=(m_ledgeLookAhead*MAP2D_BLOCKSTEPSIZE)<<ledgeShift;
			if(m_ledgeLookAhead>0)
			{
				if(m_ledgeLookOffset<limit)
				{
					// Look down
					m_ledgeLookOffset+=ledgeSpeedIn*_frames;
					if(m_ledgeLookOffset>limit)
					{
						m_ledgeLookOffset=limit;
					}
				}
				else if(m_ledgeLookOffset>limit)
				{
					// Look up
					m_ledgeLookOffset-=ledgeSpeedIn*_frames;
					if(m_ledgeLookOffset<limit)
					{
						m_ledgeLookOffset=limit;
					}
				}
			}
		}
	}
	else
	{
		if(m_ledgeLookOffset>0)
		{
			// Relax from look down
			m_ledgeLookOffset-=ledgeSpeedOut*_frames;
			if(m_ledgeLookOffset<=0)
			{
				m_ledgeLookOffset=0;
				m_ledgeLookTimer=0;
			}
		}
		else if(m_ledgeLookOffset<0)
		{
			// Relax from look up
			m_ledgeLookOffset+=ledgeSpeedOut*_frames;
			if(m_ledgeLookOffset>=0)
			{
				m_ledgeLookOffset=0;
				m_ledgeLookTimer=0;
			}
		}
	}
	m_lastLedgeLookAhead=m_ledgeLookAhead;
	m_ledgeLookAhead=0;

	// Left/right scroll as SB moves left/right
	int max=scmax*scspeed;
	m_leftRightScrollPosition+=m_moveVelocity.vx;
	if(m_leftRightScrollPosition<-max)m_leftRightScrollPosition=-max;
	else if(m_leftRightScrollPosition>max)m_leftRightScrollPosition=max;

	// Camera focus point stuff
	calcCameraFocusPointTarget();
	for(i=0;i<_frames;i++)
	{
		m_currentCamFocusPoint.vx+=(m_currentCamFocusPointTarget.vx-m_currentCamFocusPoint.vx)>>cammove;
		m_currentCamFocusPoint.vy+=(m_currentCamFocusPointTarget.vy-m_currentCamFocusPoint.vy)>>cammove;
	}

	// Final camera position
	int yoff;
	yoff=m_cameraLookOffset+(m_ledgeLookOffset>>ledgeShift);
	if(yoff<-lookmaxoffsetup)yoff=-lookmaxoffsetup;
	else if(yoff>lookmaxoffsetdown)yoff=lookmaxoffsetdown;
	if(!m_lockCamera)
	{
		m_cameraPos.vx=m_currentCamFocusPoint.vx;
		m_cameraPos.vy=m_currentCamFocusPoint.vy+yoff;
	}

	// Limit camera scroll to the edges of the map
	if(m_cameraPos.vx<m_cameraPosLimitBox.x1)		m_cameraPos.vx=m_cameraPosLimitBox.x1;
	else if(m_cameraPos.vx>m_cameraPosLimitBox.x2)	m_cameraPos.vx=m_cameraPosLimitBox.x2;
	if(m_cameraPos.vy<m_cameraPosLimitBox.y1)		m_cameraPos.vy=m_cameraPosLimitBox.y1;
	else if(m_cameraPos.vy>m_cameraPosLimitBox.y2)	m_cameraPos.vy=m_cameraPosLimitBox.y2;
	
	CPlayerThing::think(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int panim=-1;
#include "gfx\prim.h"	// (pkg)

#ifdef _STATE_DEBUG_
int stateDebugX=280;
int stateDebugY=60;
#endif

#ifdef __USER_paul__
#define NUM_LASTPOS	50
static DVECTOR	lastpos[NUM_LASTPOS];
static int		lastposnum=0;
#endif

#ifdef __USER_paul__
int mouth=-1,eyes=-1;
#endif

#include "gui\gui.h"
void	CPlayer::render()
{
	CPlayerThing::render();


#ifdef __E3__
#ifdef _STATE_DEBUG_
char buf[100];
#ifdef __USER_paul__
sprintf(buf,"%04d (%02d) ,%04d (%02d)\ndfg:%+02d\nMode:%s",Pos.vx,Pos.vx&0x0f,Pos.vy,Pos.vy&0x0f,getHeightFromGround(Pos.vx,Pos.vy),s_modeText[m_currentMode]);
#else
sprintf(buf,"Pos: %04d,%04d",Pos.vx,Pos.vy);
#endif
m_fontBank->print(stateDebugX,stateDebugY,buf);
#endif
#endif


#ifdef __USER_paul__
if(PadGetDown(0)&PAD_R1)
{
	DVECTOR	clear={0,0};
	for(int i=0;i<NUM_LASTPOS;i++)
	{
		lastpos[i]=clear;
	}
}
if(Pos.vx!=lastpos[lastposnum].vx||Pos.vy!=lastpos[lastposnum].vy)
{
	lastposnum=(lastposnum+1)%NUM_LASTPOS;
	lastpos[lastposnum]=Pos;
}
for(int i=0;i<NUM_LASTPOS;i++)
{
	int	x,y;
	x=lastpos[i].vx-m_cameraPos.vx;
	y=lastpos[i].vy-m_cameraPos.vy;
	DrawLine(x-4,y-4,x+4,y+4,0,0,255,0);
	DrawLine(x-4,y+4,x+4,y-4,0,0,255,0);
}
#endif

	// Render
	DVECTOR	sbPos=
	{
		Pos.vx-m_cameraPos.vx,
		Pos.vy-m_cameraPos.vy,
	};
	renderSb(&sbPos,m_animNo,m_animFrame>>sbanimspeed);
	m_currentPlayerModeClass->render(&sbPos);


	// Health
	if(!isWearingDivingHelmet())
	{
		// In water - Use normal SB face for health
		static int	s_fullHealthFrames[]=
		{
			FRM__HEALTH_FULL_1,
			FRM__HEALTH_FULL_2,
			FRM__HEALTH_FULL_3,
			FRM__HEALTH_FULL_4,
			FRM__HEALTH_FULL_5,
		};
		static int	s_emptyHealthFrames[]=
		{
			FRM__HEALTH_EMPTY_1,
			FRM__HEALTH_EMPTY_2,
			FRM__HEALTH_EMPTY_3,
			FRM__HEALTH_EMPTY_4,
			FRM__HEALTH_EMPTY_5,
		};
		int			i,x,y;
		POLY_FT4	*ft4;
		int			*frames;

		x=HEALTH_ICONX;
		y=HEALTH_ICONY;
		if(m_health==0||m_healthReactFrames)
		{
			frames=s_emptyHealthFrames;
		}
		else
		{
			frames=s_fullHealthFrames;
		}

		int	ygap;
		ygap=m_spriteBank->getFrameHeader(*frames)->H;
		for(i=5;i>0;i--)
		{
			ft4=m_spriteBank->printFT4(*frames++,x,y,0,0,0);
			setSemiTrans(ft4,i>m_health);
			y+=ygap;
		}
	}
	else
	{
		// Out of water - Use bowl of water
		POLY_FT4	*ft4;
		sFrameHdr	*fh;
		int			V,W,H,partH;
		
		ft4=m_spriteBank->printFT4(FRM__WATERHILIGHT,HEALTH_ICONX,HEALTH_ICONY,0,0,0);
		setSemiTrans(ft4,true);

		m_spriteBank->printFT4(FRM__WATERMETER,HEALTH_ICONX,HEALTH_ICONY,0,0,0);

		fh=m_spriteBank->getFrameHeader(FRM__WATER);
		ft4=m_spriteBank->printFT4(fh,0,0,0,0,0);
		setSemiTrans(ft4,true);
		V=fh->V;
		W=fh->W;
		H=fh->H;
		partH=(H*(255-(m_healthWaterLevel>>WATERLEVELSHIFT)))>>8;
		if(partH>H)partH=H;
		setXYWH(ft4,HEALTH_ICONX,HEALTH_ICONY+(partH),W,H-partH);
		ft4->v0=V+(partH);
		ft4->v1=V+(partH);
	}


	// Mode specific ui
	int	itemX=COLLECTEDITEM_BASEX;

	// Pickups
	m_currentPlayerModeClass->renderModeUi();
	if(isWearingBoots())
	{
		int			x,y;
		sFrameHdr	*fh=m_spriteBank->getFrameHeader(FRM__SHOE);
		x=itemX-(fh->W/2);
		y=COLLECTEDITEM_BASEY-(fh->H/2);
		m_spriteBank->printFT4(fh,x+2,y+2,0,0,0);
		m_spriteBank->printFT4(fh,x-2,y-2,0,0,0);
		itemX+=COLLECTEDITEM_GAP;
	}
	if(isWearingDivingHelmet())
	{
		sFrameHdr	*fh=m_spriteBank->getFrameHeader(FRM__HELMET);
		m_spriteBank->printFT4(fh,itemX-(fh->W/2),COLLECTEDITEM_BASEY-(fh->H/2),0,0,0);
		itemX+=COLLECTEDITEM_GAP;
	}

}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Pre-calcs the visible edges of the map ( ie: the hard limits
				for the camera pos )
	Params:		camera box ( in tiles )
	Returns:
  ---------------------------------------------------------------------- */
  void CPlayer::setCameraBox(CameraBox _cameraBox)
{
	m_cameraPosLimitBox.x1=_cameraBox.x1;
	m_cameraPosLimitBox.y1=_cameraBox.y1;
	m_cameraPosLimitBox.x2=_cameraBox.x2-(32*MAP2D_BLOCKSTEPSIZE);		// Made up numbers! :) (pkg);
	m_cameraPosLimitBox.y2=_cameraBox.y2-(16*MAP2D_BLOCKSTEPSIZE);

	m_playerPosLimitBox.x1=_cameraBox.x1+64;
	m_playerPosLimitBox.y1=_cameraBox.y1+64;
	m_playerPosLimitBox.x2=_cameraBox.x2-64;
	m_playerPosLimitBox.y2=_cameraBox.y2-64;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getHeightFromGround(int _x,int _y,int _maxHeight)
{
	int	height;

	height=m_layerCollision->getHeightFromGround(_x,_y,_maxHeight);
	if(height>=_maxHeight)
	{
		CThing *platform;
		platform=isOnPlatform();
		if(platform)
		{
			height=((CNpcPlatform*)platform)->getHeightFromPlatformAtPosition(_x,_y);
			if(height>_maxHeight)height=_maxHeight;
			else if(height<-_maxHeight)height=-_maxHeight;
		}
	}

	return height;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getHeightFromPlatformNoGround(int _x,int _y,int _maxHeight)
{
	CThing	*platform;
	int		height;

	platform=isOnPlatform();
	ASSERT(platform);
	height=((CNpcPlatform*)platform)->getHeightFromPlatformAtPosition(_x,_y);
	if(height>_maxHeight)height=_maxHeight;
	else if(height<-_maxHeight)height=-_maxHeight;

	return height;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getHeightFromGroundNoPlatform(int _x,int _y,int _maxHeight=32)
{
	return( m_layerCollision->getHeightFromGround(_x,_y,_maxHeight) );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::addHealth(int _health)
{
	if(!isWearingDivingHelmet())
	{
		m_health+=_health;
		if(m_health>MAX_HEALTH)
		{
			m_health=MAX_HEALTH;
		}
	}
	else
	{
		m_healthWaterLevel+=WATERHEALTHPART*_health;
		if(m_healthWaterLevel>WATERMAXHEALTH)
		{
			m_healthWaterLevel=WATERMAXHEALTH;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::addLife()
{
	m_lives++;
	if(m_lives>MAX_LIVES)
	{
		m_lives=MAX_LIVES;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
ATTACK_STATE	CPlayer::getAttackState()
{
	return m_currentPlayerModeClass->getAttackState();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::registerAddon(PLAYER_ADDONS _addon)
{
	if(!s_addonActorGfx[_addon])
	{
		FileEquate	sbk;
		sbk=s_addonActorPoolNames[_addon];
		if(sbk!=-1)
		{
			s_addonActorGfx[_addon]=CActorPool::GetActor(sbk);
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::setMode(PLAYER_MODE _mode)
{
	resetPlayerCollisionSizeToBase();
	m_currentMode=_mode;
	m_currentPlayerModeClass=s_playerModes[_mode];
	m_currentPlayerModeClass->enter();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getFacing()
{
	  return m_facing;
}
void CPlayer::setFacing(int _facing)
{
	m_facing=_facing;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayer::getAnimFrame()
{
	return m_animFrame;
}
void CPlayer::setAnimFrame(int _animFrame)
{
	m_animFrame=_animFrame;
}
int CPlayer::getAnimFrameCount()
{
	return m_actorGfx->getFrameCount(m_animNo)<<sbanimspeed;
}
int CPlayer::getAnimNo()
{
	return m_animNo;
}
void CPlayer::setAnimNo(int _animNo)
{
	m_animNo=_animNo;
	setAnimFrame(0);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::springPlayerUp()
{
	m_currentPlayerModeClass->springPlayerUp();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::teleportTo(int _x,int _y)
{
	DVECTOR	pos={_x,_y};

	CameraBox	releaseCamBox={0,0,29999,29999};
	setCameraBox(releaseCamBox);

	setPos(pos);
	setRespawnPos(pos);

	m_leftRightScrollPosition=0;
	calcCameraFocusPointTarget();
	m_currentCamFocusPoint=m_currentCamFocusPointTarget;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::playAnimFrameSfx(int _animNo,int _animFrame)
{
	static int				lastAnimNo=-1;
	static const AnimSfx	*sfx;

	if(_animNo!=lastAnimNo)
	{
		// Lookup the new anim number and cache it for next time :)
		int	i;
		sfx=s_animSfx;
		for(i=0;i<s_numAnimSfx;i++)
		{
			if(sfx->m_animNumber==_animNo)
			{
				break;
			}
			sfx++;
		}
		if(i==s_numAnimSfx)
		{
			// No sfx for this anim
			sfx=NULL;
		}
		lastAnimNo=_animNo;
	}

	// Are there any sounds for this anim at this frame?
	if(sfx)
	{
		const AnimFrameSfx	*frameSfx;
		int					i;

		ASSERT(sfx->m_numAnimFrameSfx);

		frameSfx=sfx->m_animFrameSfx;
		for(i=0;i<sfx->m_numAnimFrameSfx;i++)
		{
			if(m_animFrame==frameSfx->m_frame)
			{
				CSoundMediator::SFXID sfxId=frameSfx->m_sfxId;
				if(m_squeakyBootsTimer)
				{
					// Ugh.. horrible way to change the sfx when wearing squeaky boots (pkg)
					if(sfxId==CSoundMediator::SFX_SPONGEBOB_WALK_1)sfxId=CSoundMediator::SFX_SPONGEBOB_SQUEAKY_SHOES_1;
					else if(sfxId==CSoundMediator::SFX_SPONGEBOB_WALK_2)sfxId=CSoundMediator::SFX_SPONGEBOB_SQUEAKY_SHOES_2;
				}
				CSoundMediator::playSfx(sfxId);
				break;
			}
			if(m_animFrame<frameSfx->m_frame)
			{
				break;
			}
			frameSfx++;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::calcCameraFocusPointTarget()
{
	m_currentCamFocusPointTarget.vx=Pos.vx+MAP2D_CENTRE_X+(m_leftRightScrollPosition/scspeed);
	m_currentCamFocusPointTarget.vy=Pos.vy+MAP2D_CENTRE_Y;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::respawn()
{
	// Strip any items that the player might be holding
//	if(m_currentMode!=PLAYER_MODE_BASICUNARMED)
//	{
		setMode(PLAYER_MODE_FULLUNARMED);
//	}
//	else
//	{
//		setMode(PLAYER_MODE_BASICUNARMED);
//	}

	m_allowConversation=false;

	m_health=MAX_HEALTH;
	m_healthWaterLevel=WATERMAXHEALTH;
	m_healthReactFrames=0;
	m_invincibleFrameCount=INVINCIBLE_FRAMES__START;
	Pos=m_respawnPos;
	m_cameraLookOffset=0;

	m_lockCamera=false;
	m_leftRightScrollPosition=0;
	calcCameraFocusPointTarget();
	m_currentCamFocusPoint=m_currentCamFocusPointTarget;
	m_cameraPos.vx=m_currentCamFocusPoint.vx;
	m_cameraPos.vy=m_currentCamFocusPoint.vy;

	m_padLookAroundTimer=0;
	m_ledgeLookAhead=m_lastLedgeLookAhead=0;
	m_ledgeLookOffset=0;
	m_ledgeLookTimer=0;

	m_glassesFlag=0;
	m_squeakyBootsTimer=0;
	m_invincibilityRingTimer=0;
	m_bubbleAmmo=0;
	m_jellyAmmo=0;

	m_moveVelocity.vx=m_moveVelocity.vy=0;
	
	clearPlatform();

	updateCollisionArea();

	m_lives--;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Yes - This function is fat! It can be tidied up when all of the anims
				are finalised. Etracting the repeated code to a function will probly not
				improve things cos of the amount of data that would need to be passed about.
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::renderSb(DVECTOR *_pos,int _animNo,int _animFrame)
{
	POLY_FT4	*ft4;
	int			trans;
	int			addon;

	trans=m_invincibleFrameCount||m_invincibilityRingTimer;

	// Render an addon?
	addon=s_addonNumbers[m_currentMode];
	if(addon!=NO_ADDON)
	{
		s8	addonAnimNo=s_animMapNet[addon][_animNo];
		if(addonAnimNo!=-1)
		{
			CActorGfx	*addonGfx=s_addonActorGfx[addon];
			if(addonGfx)
			{
				if(_animFrame>=addonGfx->getFrameCount(addonAnimNo))
				{
					PAUL_DBGMSG("FRAME OVERRUN ON SPONGEBOB ADDON!  ( %d vs %d )",m_actorGfx->getFrameCount(_animNo),addonGfx->getFrameCount(addonAnimNo));
				}
				else
				{
					ft4=addonGfx->Render(*_pos,addonAnimNo,_animFrame,m_facing==FACING_RIGHT?0:1);
					setSemiTrans(ft4,trans);
				}
			}
		}
	}

	// Render JFish in a net?
	if(m_currentMode==PLAYER_MODE_NET&&m_currentPlayerModeClass->isJellyfishNetFull())
	{
		s8	addonAnimNo=s_animMapNet[PLAYER_ADDON_JELLYFISHINNET][_animNo];
		if(addonAnimNo!=-1)
		{
			CActorGfx	*addonGfx=s_addonActorGfx[PLAYER_ADDON_JELLYFISHINNET];
			if(addonGfx)
			{
				if(_animFrame>=addonGfx->getFrameCount(addonAnimNo))
				{
					PAUL_DBGMSG("FRAME OVERRUN ON SPONGEBOB JELLYFISH ADDON!  ( %d vs %d )",m_actorGfx->getFrameCount(_animNo),addonGfx->getFrameCount(addonAnimNo));
				}
				else
				{
					ft4=addonGfx->Render(*_pos,addonAnimNo,_animFrame,m_facing==FACING_RIGHT?0:1);
					setRGB0(ft4,255,128,255);
					setSemiTrans(ft4,trans);
				}
			}
		}
	}
	// Render glasses addon?
	if(isWearingGlasses())
	{
		s8	addonAnimNo=s_animMapNet[PLAYER_ADDON_GLASSES][_animNo];
		if(addonAnimNo!=-1)
		{
			CActorGfx	*addonGfx=s_addonActorGfx[PLAYER_ADDON_GLASSES];
			if(addonGfx)
			{
				if(_animFrame>=addonGfx->getFrameCount(addonAnimNo))
				{
					PAUL_DBGMSG("FRAME OVERRUN ON SPONGEBOB GLASSES ADDON!  ( %d vs %d )",m_actorGfx->getFrameCount(_animNo),addonGfx->getFrameCount(addonAnimNo));
				}
				else
				{
					ft4=addonGfx->Render(*_pos,addonAnimNo,_animFrame,m_facing==FACING_RIGHT?0:1);
					setSemiTrans(ft4,trans);
				}
			}
		}
	}

	// Render glove addon?
	if(m_currentMode==PLAYER_MODE_FULLUNARMED)
	{
		s8	addonAnimNo=s_animMapNet[PLAYER_ADDON_GLOVE][_animNo];
		if(addonAnimNo!=-1)
		{
			CActorGfx	*addonGfx=s_addonActorGfx[PLAYER_ADDON_GLOVE];
			if(addonGfx)
			{
				if(_animFrame>=addonGfx->getFrameCount(addonAnimNo))
				{
					PAUL_DBGMSG("FRAME OVERRUN ON SPONGEBOB GLOVE ADDON!  ( %d vs %d )",m_actorGfx->getFrameCount(_animNo),addonGfx->getFrameCount(addonAnimNo));
				}
				else
				{
					ft4=addonGfx->Render(*_pos,addonAnimNo,_animFrame,m_facing==FACING_RIGHT?0:1);
					setSemiTrans(ft4,trans);
				}
			}
		}
	}

	// Render SB
	ft4=m_actorGfx->Render(*_pos,_animNo,_animFrame,m_facing==FACING_RIGHT?0:1);
	setSemiTrans(ft4,trans);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Says whether SB can do the look up/down thing
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	CPlayer::canDoLookAround()
{
	return m_currentPlayerModeClass->canDoLookAround();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::inSoakUpState()
{
	if(isWearingDivingHelmet()&&
	   (m_layerCollision->getCollisionBlock(Pos.vx,Pos.vy)&COLLISION_TYPE_MASK)==COLLISION_TYPE_FLAG_WATER)
	{
		m_healthWaterLevel+=waterSoakUpSpeed;
		if(m_healthWaterLevel>WATERMAXHEALTH)
		{
			m_healthWaterLevel=WATERMAXHEALTH;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		When _reactDirection is REACT__GET_DIRECTION_FROM_THING then
				_thing must point to the thing that caused the damage
	Returns:
  ---------------------------------------------------------------------- */
#if	defined(__USER_daveo__)
int invincibleSponge=true;		// NB: This is for debugging purposes only so don't try and use it for a permenant cheat mode..
#else
int invincibleSponge=false;		// NB: This is for debugging purposes only so don't try and use it for a permenant cheat mode..
#endif
void CPlayer::takeDamage(DAMAGE_TYPE _damage,REACT_DIRECTION _reactDirection,CThing *_thing)
{
	if(m_invincibleFrameCount==0&&			// Don't take damage if still recovering from the last hit
	   m_invincibilityRingTimer==0&&		// Or if we have the invincibility ring on
	   m_currentMode!=PLAYER_MODE_DEAD)		// Or already dead! :)
	{
		int	ouchThatHurt=true;
		int	ouchThatHurtSoMuchThatImJustGoingToDieNow=false;

		// Check if we are currently immune to this damage type
		switch(_damage)
		{
			case DAMAGE__NONE:
				ouchThatHurt=false;
				break;

			case DAMAGE__ELECTROCUTION:
			case DAMAGE__SHOCK_ENEMY:
				if(m_squeakyBootsTimer)
				{
					ouchThatHurt=false;
				}
				break;

			case DAMAGE__FALL:
			case DAMAGE__LAVA:
			case DAMAGE__HIT_ENEMY:
			case DAMAGE__GAS_ENEMY:
			case DAMAGE__POISON_ENEMY:
			case DAMAGE__SWALLOW_ENEMY:
			case DAMAGE__PINCH_ENEMY:
			case DAMAGE__SQUASH_ENEMY:
			case DAMAGE__BURN_ENEMY:
			case DAMAGE__BITE_ENEMY:
				break;

			case DAMAGE__KILL_OUTRIGHT:
				ouchThatHurt=ouchThatHurtSoMuchThatImJustGoingToDieNow=true;
				break;
		}

		if(ouchThatHurt)
		{
			int	died=false;
			if(invincibleSponge)
			{
				m_invincibleFrameCount=INVINCIBLE_FRAMES__HIT;
			}
			else
			{
				if(!isWearingDivingHelmet())
				{
					if(!ouchThatHurtSoMuchThatImJustGoingToDieNow)
					{
						m_health--;
					}
					else
					{
						m_health=-1;
					}
					if(m_health<0)
					{
						died=true;
					}
				}
				else
				{
					if(!ouchThatHurtSoMuchThatImJustGoingToDieNow)
					{
						m_healthWaterLevel-=WATERHEALTHPART;
					}
					else
					{
						m_health=-1;
					}
					if(m_healthWaterLevel<0)
					{
						died=true;
					}
				}
			}

			if(died)
			{
				dieYouPorousFreak();
			}
			else
			{
				if(_reactDirection!=REACT__NO_REACTION)
				{
					if(_reactDirection==REACT__GET_DIRECTION_FROM_THING)
					{
						ASSERT(_thing);
						if(Pos.vx<_thing->getPos().vx)
						{
							_reactDirection=REACT__LEFT;
						}
						else
						{
							_reactDirection=REACT__RIGHT;
						}
					}

					m_moveVelocity.vx=((int)_reactDirection);
					switch(_reactDirection)
					{
						case REACT__LEFT:
							setFacing(FACING_RIGHT);
							break;
						case REACT__RIGHT:
							setFacing(FACING_LEFT);
							break;
						case REACT__UP:
						case REACT__GET_DIRECTION_FROM_THING:
						case REACT__NO_REACTION:
							break;
					}
					m_currentPlayerModeClass->setState(STATE_JUMPBACK);
				}
				m_invincibleFrameCount=INVINCIBLE_FRAMES__HIT;
				m_healthReactFrames=25;
			}
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::dieYouPorousFreak()
{
	ASSERT(m_currentMode!=PLAYER_MODE_DEAD);

	CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_DEFEATED_JINGLE);
	setMode(PLAYER_MODE_DEAD);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::updatePadInput()
{
	m_lastPadInput=m_padInput;
	m_padInput=readPadInput();
	m_padInputDown=(PLAYERINPUT)(m_padInput&(m_lastPadInput^-1));
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
PLAYERINPUT CPlayer::readPadInput()
{
	PLAYERINPUT	input;
	int			pad;

	input=PI_NONE;
	pad=PadGetHeld(0);
	
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_UP))
	{
		input=(PLAYERINPUT)(input|PI_UP);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_DOWN))
	{
		input=(PLAYERINPUT)(input|PI_DOWN);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT))
	{
		input=(PLAYERINPUT)(input|PI_LEFT);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT))
	{
		input=(PLAYERINPUT)(input|PI_RIGHT);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_JUMP))
	{
		input=(PLAYERINPUT)(input|PI_JUMP);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_FIRE))
	{
		input=(PLAYERINPUT)(input|PI_FIRE);
	}
	if(pad&CPadConfig::getButton(CPadConfig::PAD_CFG_CATCH))
	{
		input=(PLAYERINPUT)(input|PI_CATCH);
	}


#ifdef _RECORD_DEMO_MODE_
	CDemoPlayer::demoPlayerControl	*crnt;
	PLAYERINPUT						lastInput;
	crnt=&s_demoControls[s_demoSize];
	if(s_demoFrameCount==0)
	{
		crnt->m_inputValue=input;
	}
	lastInput=(PLAYERINPUT)crnt->m_inputValue;
	if(crnt->m_length==255)
	{
		lastInput=(PLAYERINPUT)(input-1);
	}
	if(lastInput==input)
	{
		crnt->m_length++;
	}
	else
	{
		s_demoSize++;
		ASSERT(s_demoSize<MAX_DEMO_SIZE);
		crnt++;
		crnt->m_inputValue=input;
		crnt->m_length=1;
	}
	s_demoFrameCount++;
	if(s_demoFrameCount==MAX_DEMO_TIME_IN_FRAMES)
	{
		writeDemoControls();
		ASSERT(!"DEMO ENDED");
	}
#endif

	return input;
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::justButtBouncedABadGuy()
{
	m_currentPlayerModeClass->setState(STATE_BUTTBOUNCEUP);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::shove( DVECTOR move )
{
	int		colHeight;

	// X movement
	colHeight=m_layerCollision->getHeightFromGround(Pos.vx+move.vx,Pos.vy,5);
	if(colHeight<0)
	{
		// Stop at the edge of the obstruction
		int	dir,vx,cx,i;
		if(move.vx<0)
		{
			dir=-1;
			vx=move.vx;
		}
		else
		{
			dir=+1;
			vx=move.vx;
		}
		cx=Pos.vx;
		for(i=0;i<vx;i++)
		{
			if(m_layerCollision->getHeightFromGround(cx,Pos.vy)<0)
			{
				break;
			}
			cx+=dir;
		}
		if(i)
			Pos.vx=cx-dir;
	}
	else
	{
		// No obstruction
		Pos.vx+=move.vx;
	}

	// Y movement
	colHeight=m_layerCollision->getHeightFromGround(Pos.vx,Pos.vy+move.vy,5);
	if(colHeight<0)
	{
		// Stop at the edge of the obstruction
		int	dir,vy,cy,i;
		if(move.vy<0)
		{
			dir=-1;
			vy=move.vy;
		}
		else
		{
			dir=+1;
			vy=move.vy;
		}
		cy=Pos.vy;
		for(i=0;i<vy;i++)
		{
			if(m_layerCollision->getHeightFromGround(Pos.vx,cy)<0)
			{
				break;
			}
			cy+=dir;
		}
		if(i)
			Pos.vy=cy-dir;
	}
	else
	{
		// No obstruction
		Pos.vy+=move.vy;
	}
}

void	CPlayer::setPlatform(CThing *_newPlatform)
{
	m_platform=_newPlatform;
}
void	CPlayer::clearPlatform()
{
	m_platform=NULL;
}

void	CPlayer::setHasPlatformCollided( bool newVal )
{
	m_hasPlatformCollided = newVal;
}

bool	CPlayer::getHasPlatformCollided()
{
	return( m_hasPlatformCollided );
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayer::moveVertical(int _moveDistance)
{
	DVECTOR			pos;
	int				hitGround;

	pos=Pos;
	hitGround=false;

	// Are we falling?
	if(_moveDistance>0)
	{
		int colHeightBefore,colHeightAfter;

		// Yes.. Check to see if we're about to hit/go through the ground/platform
		colHeightBefore=getHeightFromGround(pos.vx,pos.vy,16);
		colHeightAfter=getHeightFromGround(pos.vx,pos.vy+_moveDistance,16);
		if(isOnPlatform()&&
		   !(colHeightBefore>=0&&colHeightAfter<=0))
		{
			colHeightBefore=getHeightFromPlatformNoGround(pos.vx,pos.vy,16);
			colHeightAfter=getHeightFromPlatformNoGround(pos.vx,pos.vy+_moveDistance,16);
		}

		if(colHeightBefore>=0&&colHeightAfter<=0)
		{
			// About to hit a 'fall to death' block?
			if((m_layerCollision->getCollisionBlock(pos.vx,pos.vy+_moveDistance)&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_DEATH)
			{
				// No
				// Stick at ground level
				pos.vy+=colHeightAfter+_moveDistance;
				_moveDistance=0;
				hitGround=true;
			}
			else
			{
				// Yeah!
				if(m_currentMode!=PLAYER_MODE_DEAD)
				{
					// Lock the camera, kill the player and let him fall to his death..
					setMode(PLAYER_MODE_DEAD);
					m_lockCamera=true;
				}
			}
		}
	}
	else
	{
		// Must be below ground
		// Are we jumping into an impassable block?
		if(_moveDistance<0&&
		   (m_layerCollision->getCollisionBlock(pos.vx,pos.vy+_moveDistance)&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_NORMAL&&
		   getHeightFromGround(pos.vx,pos.vy+_moveDistance)<=0)
		{
			pos.vy=(pos.vy&0xfff0);
			_moveDistance=0;
			hitGround=true;
		}
	}
	pos.vy+=_moveDistance;
	setPlayerPos(&pos);

	return hitGround;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayer::moveHorizontal(int _moveDistance)
{
	int	hitWall;

	hitWall=false;
	if(_moveDistance)
	{
		CLayerCollision	*collision;
		DVECTOR			pos;
		int				colHeight;

		collision=getLayerCollision();
		pos=getPlayerPos();
		colHeight=getHeightFromGround(pos.vx,pos.vy,5);
		if(colHeight==0)
		{
			// Ok.. we're on the ground. What happens if we move left/right
			colHeight=getHeightFromGround(pos.vx+_moveDistance,pos.vy);
			if(colHeight<-8)
			{
				// Big step up. Stop at the edge of the obstruction
				int	dir,vx,cx,i;
				if(_moveDistance<0)
				{
					dir=-1;
					vx=-_moveDistance;
				}
				else
				{
					dir=+1;
					vx=_moveDistance;
				}
				cx=pos.vx;
				for(i=0;i<vx;i++)
				{
					if(getHeightFromGround(cx,pos.vy)<-8)
					{
						break;
					}
					cx+=dir;
				}
				if(i)
					pos.vx=cx-dir;

				hitWall=true;
				_moveDistance=0;
				
				// Get the height at this new position and then try the step-up code below.
				// Without this, there are problems when you run up a slope and hit a wall at the same time
				colHeight=getHeightFromGround(pos.vx,pos.vy);
			}
			if(colHeight&&colHeight>=-8&&colHeight<=8)
			{
				// Small step up/down. Follow the contour of the level
				pos.vy+=colHeight;
			}
		}
		else if(colHeight>0) // Lets you jump through platforms from below
		{
			if((m_layerCollision->getCollisionBlock(pos.vx+_moveDistance,pos.vy)&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_NORMAL&&
			   getHeightFromGround(pos.vx+_moveDistance,pos.vy,5)<0)
			{
				// Stop at the edge of the obstruction
				int	dir,vx,cx,i;
				if(_moveDistance<0)
				{
					dir=-1;
					vx=-_moveDistance;
				}
				else
				{
					dir=+1;
					vx=_moveDistance;
				}
				cx=pos.vx;
				for(i=0;i<vx;i++)
				{
					if(getHeightFromGround(cx,pos.vy)<0)
					{
						break;
					}
					cx+=dir;
				}
				if(i)
					cx-=dir;
				pos.vx=cx;
				_moveDistance=0;
			}
		}
		pos.vx+=_moveDistance;
		setPlayerPos(&pos);
	}
	
	return hitWall;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::resetPlayerCollisionSizeToBase()
{
	setPlayerCollisionSize(0,-COLSIZE_BASE_HEIGHT/2,COLSIZE_BASE_WIDTH,COLSIZE_BASE_HEIGHT);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::setPlayerCollisionSize(int _x,int _y,int _w,int _h)
{
	setCollisionSize(_w,_h);
	setCollisionCentreOffset(_x,_y);
}
void CPlayer::getPlayerCollisionSize(int *_x,int *_y,int *_w,int *_h)
{
	DVECTOR	offset,size;

	offset=getCollisionCentreOffset();
	size=getCollisionSize();

	*_x=offset.vx;
	*_y=offset.vy;
	*_w=size.vx;
	*_h=size.vy;
}

/*===========================================================================
end */

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

#include	"system\vid.h"
#include "player\player.h"

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
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

#ifndef __PLAYER_PMCART_H__
#include "player\pmcart.h"
#endif

#ifndef __PLAYER_PMSWAL_H__
#include "player\pmswal.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

#ifndef	__PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __PICKUPS_PICKUP_H__
#include "pickups\pickup.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __STRING_ENUMS__
#include <trans.h>
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

#ifndef	__ANIM_SPONGEBOB_WAND_HEADER__
#include <ACTOR_SPONGEBOB_WAND_Anim.h>
#endif

#ifndef	__ANIM_SPONGEBOB_JELLYFISH_HEADER__
#include <ACTOR_SPONGEBOB_JELLYFISH_Anim.h>
#endif

#ifndef	__ANIM_SPONGEBOB_GLOVE_HEADER__
#include <ACTOR_SPONGEBOB_GLOVE_Anim.h>
#endif

#include	"fx\fx.h"
/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

#ifdef __VERSION_DEBUG__
//#define _RECORD_DEMO_MODE_
#define _STATE_DEBUG_
#endif


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Two dice.  One says 'Re' on every face, the other says 'boot',
// 'install', 'try', 'tire', 'sume' and 'number'

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
	"CART",
	"SWALLOW",
};
#endif


int		m_cameraLookOffset;

int MAP2D_CENTRE_X=-(INGAME_SCREENW/2);
int MAP2D_CENTRE_Y=-(INGAME_SCREENH/2)-32;
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
CPlayerModeCart				PLAYERMODECART;
CPlayerModeSwallow			PLAYERMODESWALLOW;

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
	&PLAYERMODECART,			// PLAYER_MODE_CART
	&PLAYERMODESWALLOW,			// PLAYER_MODE_SWALLOW
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
pint	lookmaxoffsetup=4*MAP2D_BLOCKSTEPSIZE;
pint	lookmaxoffsetdown=4*MAP2D_BLOCKSTEPSIZE;
pint	camerahardlockup=3*MAP2D_BLOCKSTEPSIZE;
pint	camerahardlockdown=3*MAP2D_BLOCKSTEPSIZE;
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
	NULL,										// PLAYER_ADDON_BUBBLEWAND
	NULL,										// PLAYER_ADDON_JELLYFISHINNET
	NULL,										// PLAYER_ADDON_GLOVE
};
FileEquate		s_addonActorPoolNames[NUM_PLAYER_ADDONS]=
{
	ACTORS_SPONGEBOB_NET_SBK,					// PLAYER_ADDON_NET
	ACTORS_SPONGEBOB_CORALBLOWER_SBK,			// PLAYER_ADDON_CORALBLOWER
	ACTORS_SPONGEBOB_JELLYLAUNCHER_SBK,			// PLAYER_ADDON_JELLYLAUNCHER
	ACTORS_SPONGEBOB_WAND_SBK,					// PLAYER_ADDON_BUBBLEWAND
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
	NO_ADDON,									//	PLAYER_MODE_CART
	NO_ADDON,									//	PLAYER_MODE_SWALLOW
};

s8 s_animMapNet[NUM_PLAYER_ADDONS][NUM_ANIM_SPONGEBOB]=
{
	//	PLAYER_ADDON_NET,
	{
	ANIM_SPONGEBOB_NET_BUTTBOUNCEEND,				// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	ANIM_SPONGEBOB_NET_BUTTBOUNCESTART,				// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_NET_FALL,						// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_NET_GETUP,						// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_NET_HITGROUND,					// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_NET_IDLEBREATH,					// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_NET_JUMPEND,						// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_NET_JUMP,						// ANIM_SPONGEBOB_JUMP,
	ANIM_SPONGEBOB_NET_RUN,							// ANIM_SPONGEBOB_RUN,
	ANIM_SPONGEBOB_NET_RUNSTOP,						// ANIM_SPONGEBOB_RUNSTOP,
	ANIM_SPONGEBOB_NET_RUNSTART,					// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_NET_TEETERBACK,					// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_NET_TEETERFRONT,					// ANIM_SPONGEBOB_TEETERFRONT,
	ANIM_SPONGEBOB_NET_SWIPE,						// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_NET_DEATHTAR,					// ANIM_SPONGEBOB_DEATHTAR,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	-1,												// ANIM_SPONGEBOB_FIREEND,
	-1,												// ANIM_SPONGEBOB_FIRESTART,
	ANIM_SPONGEBOB_NET_IDLEWEAPON,					// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_NET_GETHIT,						// ANIM_SPONGEBOB_GETHIT
	ANIM_SPONGEBOB_NET_CROUCHDOWN,					// ANIM_SPONGEBOB_CROUCHDOWN
	ANIM_SPONGEBOB_NET_CROUCHUP,					// ANIM_SPONGEBOB_CROUCHUP
	ANIM_SPONGEBOB_NET_LOOKUP,						// ANIM_SPONGEBOB_LOOKUP
	-1,												// ANIM_SPONGEBOB_IDLEFACEUPSIDEDOWN
	-1,												// ANIM_SPONGEBOB_IDLEBANDAID,
//NOIDLE	-1,												// ANIM_SPONGEBOB_IDLEINHAT,
	-1,												// ANIM_SPONGEBOB_IDLEMATURE,
	-1,												// ANIM_SPONGEBOB_IDLEMOUSTACHE,
	-1,												// ANIM_SPONGEBOB_IDLENOFACE,
	-1,												// ANIM_SPONGEBOB_IDLEZORRO,
	ANIM_SPONGEBOB_NET_CELEBRATE,					// ANIM_SPONGEBOB_CELEBRATE,
	ANIM_SPONGEBOB_NET_IDLEBLINK,					// ANIM_SPONGEBOB_IDLEBLINK,
	ANIM_SPONGEBOB_NET_ELECTROCUTED,				// ANIM_SPONGEBOB_ELECTROCUTED,
	ANIM_SPONGEBOB_NET_DEATHBOUNCE,					// ANIM_SPONGEBOB_DEATHBOUNCE,
	},

	//	PLAYER_ADDON_CORALBLOWER,
	{
	-1,												// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	-1,												// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_CORALBLOWER_FALL,				// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_CORALBLOWER_GETUP,				// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_CORALBLOWER_HITGROUND,			// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_CORALBLOWER_IDLEBREATH,			// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_CORALBLOWER_JUMPEND,				// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_CORALBLOWER_JUMP,				// ANIM_SPONGEBOB_JUMP,
	-1,												// ANIM_SPONGEBOB_RUN,
	-1,												// ANIM_SPONGEBOB_RUNSTOP,
	-1,												// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_CORALBLOWER_TEETERBACK,			// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_CORALBLOWER_TEETERFRONT,			// ANIM_SPONGEBOB_TEETERFRONT,
	-1,												// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_CORALBLOWER_DEATHTAR,			// ANIM_SPONGEBOB_DEATHTAR,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	ANIM_SPONGEBOB_CORALBLOWER_FIREEND,				// ANIM_SPONGEBOB_FIREEND,
	ANIM_SPONGEBOB_CORALBLOWER_FIRESTART,			// ANIM_SPONGEBOB_FIRESTART,
	ANIM_SPONGEBOB_CORALBLOWER_IDLEWEAPON,			// ANIM_SPONGEBOB_IDLEWEAPON,
	ANIM_SPONGEBOB_CORALBLOWER_WALK,				// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_CORALBLOWER_GETHIT,				// ANIM_SPONGEBOB_GETHIT
	ANIM_SPONGEBOB_CORALBLOWER_CROUCHDOWN,			// ANIM_SPONGEBOB_CROUCHDOWN
	ANIM_SPONGEBOB_CORALBLOWER_CROUCHUP,			// ANIM_SPONGEBOB_CROUCHUP
	ANIM_SPONGEBOB_CORALBLOWER_LOOKUP,				// ANIM_SPONGEBOB_LOOKUP
	-1,												// ANIM_SPONGEBOB_IDLEFACEUPSIDEDOWN
	-1,												// ANIM_SPONGEBOB_IDLEBANDAID,
//NOIDLE	-1,												// ANIM_SPONGEBOB_IDLEINHAT,
	-1,												// ANIM_SPONGEBOB_IDLEMATURE,
	-1,												// ANIM_SPONGEBOB_IDLEMOUSTACHE,
	-1,												// ANIM_SPONGEBOB_IDLENOFACE,
	-1,												// ANIM_SPONGEBOB_IDLEZORRO,
	ANIM_SPONGEBOB_CORALBLOWER_CELEBRATE,			// ANIM_SPONGEBOB_CELEBRATE,
	ANIM_SPONGEBOB_CORALBLOWER_IDLEBLINK,			// ANIM_SPONGEBOB_IDLEBLINK,
	ANIM_SPONGEBOB_CORALBLOWER_ELECTROCUTED,		// ANIM_SPONGEBOB_ELECTROCUTED,
	ANIM_SPONGEBOB_CORALBLOWER_DEATHBOUNCE,			// ANIM_SPONGEBOB_DEATHBOUNCE,
	},

	// PLAYER_ADDON_JELLYLAUNCHER,
	{
	ANIM_SPONGEBOB_JELLYLAUNCHER_BUTTBOUNCEEND,		// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_BUTTBOUNCESTART,	// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_JELLYLAUNCHER_FALL,				// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_JELLYLAUNCHER_GETUP,				// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_JELLYLAUNCHER_HITGROUND,			// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_IDLEBREATH,		// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_JELLYLAUNCHER_JUMPEND,			// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_JUMP,				// ANIM_SPONGEBOB_JUMP,
	ANIM_SPONGEBOB_JELLYLAUNCHER_RUN,				// ANIM_SPONGEBOB_RUN,
	ANIM_SPONGEBOB_JELLYLAUNCHER_RUNSTOP,			// ANIM_SPONGEBOB_RUNSTOP,
	ANIM_SPONGEBOB_JELLYLAUNCHER_RUNSTART,			// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_JELLYLAUNCHER_TEETERBACK,		// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_JELLYLAUNCHER_TEETERFRONT,		// ANIM_SPONGEBOB_TEETERFRONT,
	-1,												// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_JELLYLAUNCHER_DEATHTAR,			// ANIM_SPONGEBOB_DEATHTAR,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	ANIM_SPONGEBOB_JELLYLAUNCHER_FIREEND,			// ANIM_SPONGEBOB_FIREEND,
	ANIM_SPONGEBOB_JELLYLAUNCHER_FIRESTART,			// ANIM_SPONGEBOB_FIRESTART,
	ANIM_SPONGEBOB_JELLYLAUNCHER_IDLEWEAPON,		// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_JELLYLAUNCHER_GETHIT,			// ANIM_SPONGEBOB_GETHIT
	ANIM_SPONGEBOB_JELLYLAUNCHER_CROUCHDOWN,		// ANIM_SPONGEBOB_CROUCHDOWN
	ANIM_SPONGEBOB_JELLYLAUNCHER_CROUCHUP,			// ANIM_SPONGEBOB_CROUCHUP
	ANIM_SPONGEBOB_JELLYLAUNCHER_LOOKUP,			// ANIM_SPONGEBOB_LOOKUP
	-1,												// ANIM_SPONGEBOB_IDLEFACEUPSIDEDOWN
	-1,												// ANIM_SPONGEBOB_IDLEBANDAID,
//NOIDLE	-1,												// ANIM_SPONGEBOB_IDLEINHAT,
	-1,												// ANIM_SPONGEBOB_IDLEMATURE,
	-1,												// ANIM_SPONGEBOB_IDLEMOUSTACHE,
	-1,												// ANIM_SPONGEBOB_IDLENOFACE,
	-1,												// ANIM_SPONGEBOB_IDLEZORRO,
	ANIM_SPONGEBOB_JELLYLAUNCHER_CELEBRATE,			// ANIM_SPONGEBOB_CELEBRATE,
	ANIM_SPONGEBOB_JELLYLAUNCHER_IDLEBLINK,			// ANIM_SPONGEBOB_IDLEBLINK,
	ANIM_SPONGEBOB_JELLYLAUNCHER_ELECTROCUTED,		// ANIM_SPONGEBOB_ELECTROCUTED,
	ANIM_SPONGEBOB_JELLYLAUNCHER_DEATHBOUNCE,		// ANIM_SPONGEBOB_DEATHBOUNCE,
	},

	// PLAYER_ADDON_BUBBLEWAND,
	{
	ANIM_SPONGEBOB_WAND_BUTTBOUNCEEND,				// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	ANIM_SPONGEBOB_WAND_BUTTBOUNCESTART,			// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_WAND_FALL,						// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_WAND_GETUP,						// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_WAND_HITGROUND,					// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_WAND_IDLEBREATH,					// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_WAND_JUMPEND,					// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_WAND_JUMP,						// ANIM_SPONGEBOB_JUMP,
	ANIM_SPONGEBOB_WAND_RUN,						// ANIM_SPONGEBOB_RUN,
	ANIM_SPONGEBOB_WAND_RUNSTOP,					// ANIM_SPONGEBOB_RUNSTOP,
	ANIM_SPONGEBOB_WAND_RUNSTART,					// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_WAND_TEETERBACK,					// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_WAND_TEETERFRONT,				// ANIM_SPONGEBOB_TEETERFRONT,
	-1,												// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_WAND_DEATHTAR,					// ANIM_SPONGEBOB_DEATHTAR,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	ANIM_SPONGEBOB_WAND_BLOWBUBBLE,					// ANIM_SPONGEBOB_BLOWBUBBLE,
	-1,												// ANIM_SPONGEBOB_FIREEND,
	-1,												// ANIM_SPONGEBOB_FIRESTART,
	ANIM_SPONGEBOB_WAND_IDLEWEAPON,					// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_WAND_GETHIT,						// ANIM_SPONGEBOB_GETHIT
	ANIM_SPONGEBOB_WAND_CROUCHDOWN,					// ANIM_SPONGEBOB_CROUCHDOWN
	ANIM_SPONGEBOB_WAND_CROUCHUP,					// ANIM_SPONGEBOB_CROUCHUP
	ANIM_SPONGEBOB_WAND_LOOKUP,						// ANIM_SPONGEBOB_LOOKUP
	-1,												// ANIM_SPONGEBOB_IDLEFACEUPSIDEDOWN
	-1,												// ANIM_SPONGEBOB_IDLEBANDAID,
//NOIDLE	-1,												// ANIM_SPONGEBOB_IDLEINHAT,
	-1,												// ANIM_SPONGEBOB_IDLEMATURE,
	-1,												// ANIM_SPONGEBOB_IDLEMOUSTACHE,
	-1,												// ANIM_SPONGEBOB_IDLENOFACE,
	-1,												// ANIM_SPONGEBOB_IDLEZORRO,
	ANIM_SPONGEBOB_WAND_CELEBRATE,					// ANIM_SPONGEBOB_CELEBRATE,
	ANIM_SPONGEBOB_WAND_IDLEBLINK,					// ANIM_SPONGEBOB_IDLEBLINK,
	ANIM_SPONGEBOB_WAND_ELECTROCUTED,				// ANIM_SPONGEBOB_ELECTROCUTED,
	ANIM_SPONGEBOB_WAND_DEATHBOUNCE,				// ANIM_SPONGEBOB_DEATHBOUNCE,
	},

	// PLAYER_ADDON_JELLYFISHINNET
	{
	ANIM_SPONGEBOB_JELLYFISH_BUTTBOUNCEEND,			// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	ANIM_SPONGEBOB_JELLYFISH_BUTTBOUNCESTART,		// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	ANIM_SPONGEBOB_JELLYFISH_FALL,					// ANIM_SPONGEBOB_FALL,
	ANIM_SPONGEBOB_JELLYFISH_GETUP,					// ANIM_SPONGEBOB_GETUP,
	ANIM_SPONGEBOB_JELLYFISH_HITGROUND,				// ANIM_SPONGEBOB_HITGROUND,
	ANIM_SPONGEBOB_JELLYFISH_IDLEBREATH,			// ANIM_SPONGEBOB_IDLEBREATH,
	ANIM_SPONGEBOB_JELLYFISH_JUMPEND,				// ANIM_SPONGEBOB_JUMPEND,
	ANIM_SPONGEBOB_JELLYFISH_JUMP,					// ANIM_SPONGEBOB_JUMP,
	ANIM_SPONGEBOB_JELLYFISH_RUN,					// ANIM_SPONGEBOB_RUN,
	ANIM_SPONGEBOB_JELLYFISH_RUNSTOP,				// ANIM_SPONGEBOB_RUNSTOP,
	ANIM_SPONGEBOB_JELLYFISH_RUNSTART,				// ANIM_SPONGEBOB_RUNSTART,
	ANIM_SPONGEBOB_JELLYFISH_TEETERBACK,			// ANIM_SPONGEBOB_TEETERBACK,
	ANIM_SPONGEBOB_JELLYFISH_TEETERFRONT,			// ANIM_SPONGEBOB_TEETERFRONT,
	ANIM_SPONGEBOB_JELLYFISH_SWIPE,					// ANIM_SPONGEBOB_SWIPE,
	ANIM_SPONGEBOB_JELLYFISH_DEATHTAR,				// ANIM_SPONGEBOB_DEATHTAR,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	-1,												// ANIM_SPONGEBOB_FIREEND,
	-1,												// ANIM_SPONGEBOB_FIRESTART,
	ANIM_SPONGEBOB_JELLYFISH_IDLEWEAPON,			// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	-1,												// ANIM_SPONGEBOB_KARATE,
	ANIM_SPONGEBOB_JELLYFISH_GETHIT,				// ANIM_SPONGEBOB_GETHIT
	ANIM_SPONGEBOB_JELLYFISH_CROUCHDOWN,			// ANIM_SPONGEBOB_CROUCHDOWN
	ANIM_SPONGEBOB_JELLYFISH_CROUCHUP,				// ANIM_SPONGEBOB_CROUCHUP
	ANIM_SPONGEBOB_JELLYFISH_LOOKUP,				// ANIM_SPONGEBOB_LOOKUP
	-1,												// ANIM_SPONGEBOB_IDLEFACEUPSIDEDOWN
	-1,												// ANIM_SPONGEBOB_IDLEBANDAID,
//NOIDLE	-1,												// ANIM_SPONGEBOB_IDLEINHAT,
	-1,												// ANIM_SPONGEBOB_IDLEMATURE,
	-1,												// ANIM_SPONGEBOB_IDLEMOUSTACHE,
	-1,												// ANIM_SPONGEBOB_IDLENOFACE,
	-1,												// ANIM_SPONGEBOB_IDLEZORRO,
	ANIM_SPONGEBOB_JELLYFISH_CELEBRATE,				// ANIM_SPONGEBOB_CELEBRATE,
	ANIM_SPONGEBOB_JELLYFISH_IDLEBLINK,				// ANIM_SPONGEBOB_IDLEBLINK,
	ANIM_SPONGEBOB_JELLYFISH_ELECTROCUTED,			// ANIM_SPONGEBOB_ELECTROCUTED,
	ANIM_SPONGEBOB_JELLYFISH_DEATHBOUNCE,			// ANIM_SPONGEBOB_DEATHBOUNCE,
	},

	// PLAYER_ADDON_GLOVE
	{
	-1,												// ANIM_SPONGEBOB_BUTTBOUNCEEND,
	-1,												// ANIM_SPONGEBOB_BUTTBOUNCESTART,
	-1,												// ANIM_SPONGEBOB_FALL,
	-1,												// ANIM_SPONGEBOB_GETUP,
	-1,												// ANIM_SPONGEBOB_HITGROUND,
	-1,												// ANIM_SPONGEBOB_IDLEBREATH,
	-1,												// ANIM_SPONGEBOB_JUMPEND,
	-1,												// ANIM_SPONGEBOB_JUMP,
	-1,												// ANIM_SPONGEBOB_RUN,
	-1,												// ANIM_SPONGEBOB_RUNSTOP,
	-1,												// ANIM_SPONGEBOB_RUNSTART,
	-1,												// ANIM_SPONGEBOB_TEETERBACK,
	-1,												// ANIM_SPONGEBOB_TEETERFRONT,
	-1,												// ANIM_SPONGEBOB_SWIPE,
	-1,												// ANIM_SPONGEBOB_DEATHTAR,
	-1,												// ANIM_SPONGEBOB_BALLOONJUMP,
	-1,												// ANIM_SPONGEBOB_BLOWBUBBLE,
	-1,												// ANIM_SPONGEBOB_FIREEND,
	-1,												// ANIM_SPONGEBOB_FIRESTART,
	-1,												// ANIM_SPONGEBOB_IDLEWEAPON,
	-1,												// ANIM_SPONGEBOB_WALK,
	ANIM_SPONGEBOB_GLOVE_KARATE,					// ANIM_SPONGEBOB_KARATE,
	-1,												// ANIM_SPONGEBOB_GETHIT
	-1,												// ANIM_SPONGEBOB_CROUCHDOWN
	-1,												// ANIM_SPONGEBOB_CROUCHUP
	-1,												// ANIM_SPONGEBOB_LOOKUP
	-1,												// ANIM_SPONGEBOB_IDLEFACEUPSIDEDOWN
	-1,												// ANIM_SPONGEBOB_IDLEBANDAID,
//NOIDLE	-1,												// ANIM_SPONGEBOB_IDLEINHAT,
	-1,												// ANIM_SPONGEBOB_IDLEMATURE,
	-1,												// ANIM_SPONGEBOB_IDLEMOUSTACHE,
	-1,												// ANIM_SPONGEBOB_IDLENOFACE,
	-1,												// ANIM_SPONGEBOB_IDLEZORRO,
	-1,												// ANIM_SPONGEBOB_CELEBRATE,
	-1,												// ANIM_SPONGEBOB_IDLEBLINK,
	-1,												// ANIM_SPONGEBOB_ELECTROCUTED,
	-1,												// ANIM_SPONGEBOB_DEATHBOUNCE,
	},
};

// -------------------------------------------------------------------------------


int			m_cameraXScrollDir;
int			m_cameraXScrollPos;
int CAMERA_SCROLLLIMIT=8;							// SB is this many tiles off centre at most
int CAMERA_SCROLLTHRESHOLD=0;		// (pkg 17/7)	// If SB moves when more than this many tiles off-centre, the camera will *always* scroll
int CAMERA_STARTMOVETHRESHOLD=0;	// (pkg 17/7)	// If SB moves faster than this then the camera starts scrolling
int CAMERA_STOPMOVETHRESHOLD=0;		// (pkg 17/7)	// If SB moves slower than this then the camera stops scrolling
int CAMERA_SCROLLSPEED=1000;						// Speed of the scroll
int	CAMERA_ACCURACYSHIFT=8;
const int	CAMERA_TILESIZE=16;

// Well, there goes any chance of refined camera control for the player! :(		( pkg 17/7 )


int			m_isFalling;
int			m_fallFrames;
int			m_cameraFallYScrollPos;
int			m_cameraFallYScrollSpeed;
int			CAMERA_FALL_DROP_SPEED=6;
int			CAMERA_FALL_MAX_OFFSET=4*16;
int			CAMERA_FALL_FRAME_THRESHOLD=20;
int			CAMERA_FALL_RETURN_SPEED=1;




#ifdef CAM_X_AUTO_CENTERING
int	returnspeed=1500;
int	returntimeout=25;
int	returntimeoutcount=0;
int	returnsafespace=4*16;
#endif


extern int checkx;
extern int checky;



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
	m_fontBank->setOt(POWERUPUI_OT);

	m_scalableFontBank=new ("PlayerFont") ScalableFontBank();
	m_scalableFontBank->initialise(&standardFont);
	m_scalableFontBank->setOt(POWERUPUI_OT);
	
	m_actorGfx=CActorPool::GetActor(ACTORS_SPONGEBOB_SBK);

	for(i=0;i<NUM_PLAYERMODES;i++)
	{
		s_playerModes[i]->initialise(this);
	}
	CurrentPrompt=-1;
	PromptRGB=0;
	PromptFade=0;

m_animNo=0;
m_animFrame=0;
	setFacing(FACING_RIGHT);
	m_currentPlayerModeClass=NULL;
	m_lastModeBeforeDeath=PLAYER_MODE_BASICUNARMED;	// Player will then respawn into this mode
	respawn();

	m_lastPadInput=m_padInput=PI_NONE;

	setCartCam(false);
	setReverseCameraMovement(false);

	resetPlayerCollisionSizeToBase();

	m_lockoutPlatform = NULL;
	m_divingHelmet=false;
	setIsInWater(true);

#ifdef __USER_paul__
	/*
registerAddon(PLAYER_ADDON_GLOVE);
registerAddon(PLAYER_ADDON_NET);
registerAddon(PLAYER_ADDON_CORALBLOWER);
registerAddon(PLAYER_ADDON_JELLYLAUNCHER);
registerAddon(PLAYER_ADDON_BUBBLEWAND);
registerAddon(PLAYER_ADDON_JELLYFISHINNET);
*/
#endif
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

	m_scalableFontBank->dump();	delete m_scalableFontBank;
	m_fontBank->dump();			delete m_fontBank;

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

void	CPlayer::think(int _frames)
{
	int	i;

#ifdef	__USER_daveo__
if(PadGetDown(0)&PAD_R1)
{
	TestFX(getPos(),this);
}
#endif


#ifdef _STATE_DEBUG_
if(PadGetDown(0)&PAD_L1&&m_currentMode!=PLAYER_MODE_DEAD)
{
	oldmode=m_currentMode;
	newmode=PLAYER_MODE_FLY;
}
else if(oldmode!=-1&&!(PadGetHeld(0)&PAD_L1))
{
	newmode=oldmode;
	oldmode=-1;
}
#endif
if(newmode!=-1)
{
	setMode((PLAYER_MODE)newmode);
	newmode=-1;
}

	if(!isDead())
	{
		CThing	*platform;
		platform=isOnPlatform();
		if(platform)
		{
			if ( ( (CNpcPlatform *) platform )->isCart() )
			{
				if ( m_currentMode != PLAYER_MODE_CART && m_currentMode != PLAYER_MODE_DEAD )
				{
					setMode( PLAYER_MODE_CART );
				}
			}
			else
			{
				DVECTOR	posDelta;
				posDelta=platform->getPosDelta();
				posDelta.vy = 0;
				shove(posDelta);

				int platformOffset = ( ( CNpcPlatform* ) platform )->getHeightFromPlatformAtPosition( Pos.vx, Pos.vy );

				int height = 16;
				
				for ( int testX = -1 ; testX < 2 ; testX++ )
				{
					int testHeight = CGameScene::getCollision()->getHeightFromGround(Pos.vx + (checkx * testX),Pos.vy,16);

					if ( testHeight < height )
					{
						height = testHeight;
					}
				}

				// Hmm.. this *almost* stops him elevating through walls :/
				if ( platformOffset < height )
				{
					int goingToHitWall=false;
					int	i;
					for(i=-1;i<2;i++)
					{
						int x=Pos.vx+(checkx*i);
						int	y=Pos.vy-HEIGHT_FOR_HEAD_COLLISION;
						//if(getHeightFromGroundNoPlatform(x,y,16)>=0&&getHeightFromGroundNoPlatform(x,y+platformOffset,16)<=0&&((CGameScene::getCollision()->getCollisionBlock(x,y+platformOffset)&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_NORMAL))
						if(getHeightFromGroundNoPlatform(x,y+platformOffset,16)<=0&&((CGameScene::getCollision()->getCollisionBlock(x,y+platformOffset)&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_NORMAL))
						{
							goingToHitWall=true;
							setLockoutPlatform( m_platform );
							break;
						}
					}

					if(!goingToHitWall)
					{
						Pos.vy += platformOffset;
					}
				}
			}
		}
	}

	m_allowConversation=false;

	m_xMove = Pos.vx;
	m_moveLeftRight = 0;

///
#ifdef __USER_paul__
//if(PadGetDown(0)&PAD_TRIANGLE)
//{
//	addLife();
//}
#endif
///


	// Pending teleport?
	if(m_teleportPending)
	{
		processTeleportToCommand();
	}


	for(i=0;i<_frames;i++)
	{
		updatePadInput();


		// Only do the weapon change stuff on the first frame. As the buttons pressed do not
		// change over these frames there is no point in doing it every frame
		if(m_currentMode!=PLAYER_MODE_DEAD)
		{
			if(i==0&&m_currentPlayerModeClass->getState()!=STATE_CELEBRATE)
			{
				m_tryingToManuallyPickupWeapon=false;
				m_tryingToAutomaticallyPickupWeapon=false;

				// Weapon collect/drop/swap stuff..
				if(m_currentMode==PLAYER_MODE_BASICUNARMED)
				{
					// Always trying to pick up weapon if unarmed... means that when SB walks
					// over an item whilst unarmed, he automatically picks it up
					m_tryingToAutomaticallyPickupWeapon=true;
				}
				bool	CanChange=true;
				switch(m_currentPlayerModeClass->getState())
				{
					case STATE_BUTTBOUNCE:
					case STATE_BUTTFALL:
					case STATE_BUTTLAND:
					case STATE_BUTTBOUNCEUP:
						CanChange=false;
						break;

					default:
						break;
				}

				if(CanChange && (m_currentMode!=PLAYER_MODE_CART && getPadInputDown()&PI_WEAPONCHANGE))
				{
					// If already armed then drop current weapon
					if(m_currentMode!=PLAYER_MODE_BASICUNARMED)
					{
						static const int	s_pickupsToDrop[NUM_PLAYERMODES]=
						{
							-1,						// PLAYER_MODE_BASICUNARMED
							-1,						// PLAYER_MODE_FULLUNARMED
							-1,						// PLAYER_MODE_BALLOON
							PICKUP__BUBBLE_WAND,	// PLAYER_MODE_BUBBLE_MIXTURE
							PICKUP__NET,			// PLAYER_MODE_NET
							PICKUP__CORAL_BLOWER,	// PLAYER_MODE_CORALBLOWER
							PICKUP__JELLY_LAUNCHER,	// PLAYER_MODE_JELLY_LAUNCHER
							-1,						// PLAYER_MODE_DEAD
							-1,						// PLAYER_MODE_FLY
							-1,						// PLAYER_MODE_CART
							-1,						// PLAYER_MODE_SWALLOW
						};

						int	pickupToDrop;
						pickupToDrop=s_pickupsToDrop[m_currentMode];
						if(pickupToDrop!=-1)
						{
							DVECTOR	pickupPos;
							CBasePickup	*pickup;
							pickupPos.vx=Pos.vx;
							pickupPos.vy=Pos.vy-30;
							pickup=createPickup((PICKUP_TYPE)pickupToDrop,&pickupPos);
							pickup->setPos(&pickupPos);
							((CBaseWeaponPickup*)pickup)->setHasBeenCollected();
						}
						setMode(PLAYER_MODE_BASICUNARMED);
					}

					// Now trying to pick up a weapon..
					m_tryingToManuallyPickupWeapon=true;
				}
			}
		}
		else
		{
			// Don't try to pick up weapons when dead.. doh!
			m_tryingToAutomaticallyPickupWeapon=false;
			m_tryingToManuallyPickupWeapon=false;
		}

		int level=GameScene.getLevelNumber();
		// Out of spats?
		if(m_numSpatulasHeld==0 && level!=5)
		{
			int	oldTimer=m_spatulaWarningTimer;
			m_spatulaWarningTimer++;
			if((m_spatulaWarningTimer&64)!=(oldTimer&64))
			{
				CSoundMediator::playSfx(CSoundMediator::SFX_BEEP10);
			}
		}

		// Trying to converate?
		if(m_allowConversation==false&&
		   m_currentPlayerModeClass->canConverse()&&
		   getPadInputDown()&PI_UP)
		{
			m_allowConversation=true;
		}

		// Think for the current player mode
		int		oldBlock;
		DVECTOR	oldPos;
		oldBlock=CGameScene::getCollision()->getCollisionBlock(Pos.vx,Pos.vy)&COLLISION_TYPE_MASK;
		oldPos=Pos;
		m_currentPlayerModeClass->think();
		if(oldBlock==COLLISION_TYPE_FLAG_DESTRUCTABLE_FLOOR&&
		   (Pos.vx>>4!=oldPos.vx>>4||Pos.vy>>4!=oldPos.vy>>4))
		{
			GameScene.GetLevel().destroyMapTile(oldPos);
		}


		// Is player stood on any special collision?
		for(int j=0;j<2;j++)
		{
			int	x=Pos.vx+((j==0)?-checkx:+checkx);
			if(getHeightFromGroundNoPlatform(x,Pos.vy,5)==0)
			{
				int block;
				block=CGameScene::getCollision()->getCollisionBlock(x,Pos.vy)&COLLISION_TYPE_MASK;

				// Conveyor belt movement
				if(block==COLLISION_TYPE_FLAG_MOVE_LEFT)
				{
					moveHorizontal(-1);
					break;
				}
				else if(block==COLLISION_TYPE_FLAG_MOVE_RIGHT)
				{
					moveHorizontal(+1);
					break;
				}
			}
		}

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

		// Camera scroll..
		if(m_cartCamActive)
		{
			// Just force the cam eo think we are running right so that the cam
			// is always in the right place when in a cart
			m_cameraXScrollDir=-1;
		}
		if(m_cameraXScrollDir==-1)
		{
			if(m_cameraXScrollPos>-(CAMERA_SCROLLLIMIT*CAMERA_TILESIZE)<<CAMERA_ACCURACYSHIFT)
			{
				m_cameraXScrollPos-=CAMERA_SCROLLSPEED;
				if(m_cameraXScrollPos<-(CAMERA_SCROLLLIMIT*CAMERA_TILESIZE)<<CAMERA_ACCURACYSHIFT)
				{
					m_cameraXScrollPos=-(CAMERA_SCROLLLIMIT*CAMERA_TILESIZE)<<CAMERA_ACCURACYSHIFT;
					m_cameraXScrollDir=0;
				}
			}
#ifdef CAM_X_AUTO_CENTERING
			returntimeoutcount=0;
#endif
		}
		else if(m_cameraXScrollDir==+1)
		{
			if(m_cameraXScrollPos<((CAMERA_SCROLLLIMIT*CAMERA_TILESIZE)<<CAMERA_ACCURACYSHIFT))
			{
				m_cameraXScrollPos+=CAMERA_SCROLLSPEED;
				if(m_cameraXScrollPos>(CAMERA_SCROLLLIMIT*CAMERA_TILESIZE)<<CAMERA_ACCURACYSHIFT)
				{
					m_cameraXScrollPos=(CAMERA_SCROLLLIMIT*CAMERA_TILESIZE)<<CAMERA_ACCURACYSHIFT;
					m_cameraXScrollDir=0;
				}
			}
#ifdef CAM_X_AUTO_CENTERING
			returntimeoutcount=0;
#endif
		}

#ifdef CAM_X_AUTO_CENTERING
		if(m_moveVelocity.vx==0)
		{
			m_cameraXScrollDir=0;
			if(m_cameraXScrollPos<-returnsafespace||m_cameraXScrollPos>returnsafespace)
			{
				if(returntimeoutcount<returntimeout)
				{
					returntimeoutcount++;
				}
				else
				{
					if(m_cameraXScrollPos<0)
					{
						m_cameraXScrollPos+=returnspeed;
						if(m_cameraXScrollPos>0)
						{
							m_cameraXScrollPos=0;
						}
					}
					else if(m_cameraXScrollPos>0)
					{
						m_cameraXScrollPos-=returnspeed;
						if(m_cameraXScrollPos<0)
						{
							m_cameraXScrollPos=0;
						}
					}
				}
			}
		}
#endif

		// Stop the player vanishing off the edge of the telly..
		if(m_currentMode!=PLAYER_MODE_DEAD)		// Lock camera when player is dead
		{
			if(Pos.vx<m_playerPosLimitBox.x1)		Pos.vx=m_playerPosLimitBox.x1;
			else if(Pos.vx>m_playerPosLimitBox.x2)	Pos.vx=m_playerPosLimitBox.x2;
			if(Pos.vy<m_playerPosLimitBox.y1)		Pos.vy=m_playerPosLimitBox.y1;
			else if(Pos.vy>m_playerPosLimitBox.y2)	Pos.vy=m_playerPosLimitBox.y2;
		}

		// Falling camera lag
		if(m_isFalling)
		{
			if(m_fallFrames>CAMERA_FALL_FRAME_THRESHOLD)
			{
				if(m_cameraFallYScrollSpeed<CAMERA_FALL_DROP_SPEED)
				{
					m_cameraFallYScrollSpeed++;
				}
				m_cameraFallYScrollPos-=m_cameraFallYScrollSpeed;
				if(m_cameraFallYScrollPos<-CAMERA_FALL_MAX_OFFSET)
				{
					m_cameraFallYScrollPos=-CAMERA_FALL_MAX_OFFSET;
				}
			}
			else
			{
				m_fallFrames++;
			}
			m_isFalling=false;
		}
		else
		{
			if(m_cameraFallYScrollPos)
			{
				m_cameraFallYScrollPos+=CAMERA_FALL_RETURN_SPEED;
				if(m_cameraFallYScrollPos>0)
				{
					m_cameraFallYScrollPos=0;
				}
			}
			m_fallFrames=0;
			m_cameraFallYScrollSpeed=0;
		}

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

	m_xMove = Pos.vx - m_xMove;

	ASSERT(!(getIsInWater()==false&&isWearingDivingHelmet()==false));
	if(isWearingDivingHelmet())
	{
		if(getIsInWater()==false&&m_currentMode!=PLAYER_MODE_DEAD&&m_currentMode!=PLAYER_MODE_FLY)
		{
			// Out of water and wearing helmet!
	
			// Drain water/health
			int oldLevel=m_healthWaterLevel/WATER_COUNTER_SECONDTIME;
			m_healthWaterLevel-=waterDrainSpeed*_frames;
			if(oldLevel<=6&&oldLevel>m_healthWaterLevel/WATER_COUNTER_SECONDTIME)
			{
				CSoundMediator::playSfx(CSoundMediator::SFX_BEEP5);
			}
			if(m_healthWaterLevel<=0)
			{
				dieYouPorousFreak(DEATHTYPE__DRYUP);
				CSoundMediator::playSfx(CSoundMediator::SFX_BEEP5);
			}

			// Breath sound
			m_helmetSoundTimer+=_frames;
			if(m_helmetSoundTimer>150+(m_healthWaterLevel>>WATERLEVELSHIFT))
			{
				CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_DIVING_HELMET);
				m_helmetSoundTimer=0;
			}
		}
		else if(getIsInWater())
		{
			// Back in water - fill up helmet! :)
			m_healthWaterLevel+=waterSoakUpSpeed;
			if(m_healthWaterLevel>WATERMAXHEALTH)
			{
				m_healthWaterLevel=WATERMAXHEALTH;
			}
		}
	}

	// Ledge look-ahead stuff
	if(m_ledgeLookAhead)
	{
		if(m_ledgeLookTimer<ledgeTimer)
		{
			m_ledgeLookTimer+=_frames;
		}
		else
		{
			int limit;
			limit=(m_ledgeLookAhead*MAP2D_BLOCKSTEPSIZE)<<ledgeShift;
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
	if(yoff<-camerahardlockup)yoff=-camerahardlockup;
	else if(yoff>camerahardlockdown)yoff=camerahardlockdown;
	if(m_currentMode!=PLAYER_MODE_DEAD)		// Lock camera when player is dead
	{
		m_cameraPos.vx=m_currentCamFocusPoint.vx;
		m_cameraPos.vy=m_currentCamFocusPoint.vy+yoff;
		CGameScene::shakeCamera(m_cameraPos);
	}

	// Limit camera scroll to the edges of the map
	if(m_cameraPos.vx<m_cameraPosLimitBox.x1)		m_cameraPos.vx=m_cameraPosLimitBox.x1;
	else if(m_cameraPos.vx>m_cameraPosLimitBox.x2)	m_cameraPos.vx=m_cameraPosLimitBox.x2;
	if(m_cameraPos.vy<m_cameraPosLimitBox.y1)		m_cameraPos.vy=m_cameraPosLimitBox.y1;
	else if(m_cameraPos.vy>m_cameraPosLimitBox.y2)	m_cameraPos.vy=m_cameraPosLimitBox.y2;

	m_ignoreNewlyPressedButtonsOnPadThisThink=false;

	// Restore flipped camera
	setReverseCameraMovement(false);

	// flashing pants..
	if(m_pantFlashTimer)
	{
		m_pantFlashTimer-=_frames;
	}

	CPlayerThing::think(_frames);
	promptThink(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::detectHazardousSurface()
{
	// DOn't worry if on platform or already dead
	if(!isOnPlatform()&&
	   m_currentMode!=PLAYER_MODE_DEAD)
	{
		int				i;	
		DVECTOR	const	&pos=getPlayerPos();

		for(i=0;i<2;i++)
		{
			int				x,height;

			x=pos.vx+(i==0?-checkx:checkx);
			height=CGameScene::getCollision()->getHeightFromGround(x,pos.vy);
			if(height<=0)
			{
				int block;
				block=CGameScene::getCollision()->getCollisionBlock(x,pos.vy)&COLLISION_TYPE_MASK;

				// Death?
				if(block==COLLISION_TYPE_FLAG_DEATH_LIQUID)
				{
					dieYouPorousFreak(DEATHTYPE__LIQUID);
					break;
				}
				else if(block==COLLISION_TYPE_FLAG_DEATH_INSTANT)
				{
					dieYouPorousFreak(DEATHTYPE__NORMAL);
					break;
				}
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
int panim=-1;
#include "gfx\prim.h"	// (pkg)

#ifdef _STATE_DEBUG_
int stateDebugX=280;
int stateDebugY=180;
#endif

#ifdef __USER_paul__
#define NUM_LASTPOS	100
typedef struct
{
	int	vx,vy;
	int	h;
	int	onp;
}POSMEM;
static POSMEM	lastpos[NUM_LASTPOS];
static int		lastposnum=0;
int				drawlastpos=false;
#endif

#ifdef __USER_paul__
int mouth=-1,eyes=-1;
#endif

#include "gui\gui.h"
void	CPlayer::render()
{
	CPlayerThing::render();


#ifdef _STATE_DEBUG_x
char buf[100];
#ifdef __USER_paul__
sprintf(buf,"%04d (%02d) ,%04d (%02d)\ndfg:%+02d\nMode:%s",Pos.vx,Pos.vx&0x0f,Pos.vy,Pos.vy&0x0f,getHeightFromGround(Pos.vx,Pos.vy),s_modeText[m_currentMode]);
#else
sprintf(buf,"Pos: %04d,%04d",Pos.vx,Pos.vy);
#endif
m_fontBank->setTrans(0);
m_fontBank->setColour(255,255,255);

m_fontBank->print(stateDebugX,stateDebugY,buf);
#endif


#ifdef __USER_paul__
if(PadGetDown(0)&PAD_R1)
{
	POSMEM	clear={0,0,0,false};
	for(int i=0;i<NUM_LASTPOS;i++)
	{
		lastpos[i]=clear;
	}
	drawlastpos=drawlastpos==true?false:true;
}
if(Pos.vx!=lastpos[lastposnum].vx||Pos.vy!=lastpos[lastposnum].vy)
{
	lastposnum=(lastposnum+1)%NUM_LASTPOS;
	POSMEM	*p=&lastpos[lastposnum];
	p->vx=Pos.vx;
	p->vy=Pos.vy;
	p->onp=isOnPlatform()!=NULL;
	if(p->onp)
	{
		p->h=getHeightFromPlatformNoGround(Pos.vx,Pos.vy,150);
	}
	else
	{
		p->h=getHeightFromGround(Pos.vx,Pos.vy,150);
	}
}
if(drawlastpos)
{
	POSMEM	*p=lastpos;
	for(int i=0;i<NUM_LASTPOS;i++)
	{
		int	x,y;
		x=p->vx-m_cameraPos.vx;
		y=p->vy-m_cameraPos.vy;
		DrawLine(x-4,y-4,x+4,y+4,0,0,255,0);
		DrawLine(x-4,y+4,x+4,y-4,0,0,255,0);
		y=y+p->h;
		if(p->onp)
		{
			DrawLine(x-6,y,x+6,y,255,0,255,0);
		}
		else
		{
			DrawLine(x-6,y,x+6,y,128,0,255,0);
		}

		p++;
	}
}
#endif

	SpriteBank	*sb=CGameScene::getSpriteBank();


	// Render player
	if(m_animNo!=ANIM_SPONGEBOB_DONOTDRAW)
	{
		DVECTOR	sbPos=
		{
			Pos.vx-m_cameraPos.vx,
			Pos.vy-m_cameraPos.vy+1,		// Odd.. source sprites were moved up by one pixel at some stage
		};
		renderSb(&sbPos,m_animNo,m_animFrame>>sbanimspeed);
		m_currentPlayerModeClass->render(&sbPos);
	}


	// UI
	// Don't draw it when the conversations are active - it's messy :/
	if(!CConversation::isActive())
	{
		int			count,warn;
		sFrameHdr	*fh;
		char		countBuf[5];
		int			x,y;

		// Spat/token count
		warn=false;
		if(GameScene.getLevelNumber()!=5)
		{
			// Spat count
			count=m_numSpatulasHeld;
			fh=sb->getFrameHeader(FRM__SPATULA);
			if(m_numSpatulasHeld==0)
			{
				warn=true;
			}
		}
		else
		{
			// Token count
			count=CGameSlotManager::getSlotData()->getKelpTokenCollectedCount(GameScene.getChapterNumber()-1,GameScene.getLevelNumber()-1);
			fh=sb->getFrameHeader(FRM__TOKEN);
		}
		sprintf(countBuf,"x%d",count);
		x=SB_UI_XBASE;
		y=SB_UI_YBASE;
		sb->printFT4(fh,x,y,0,0,POWERUPUI_OT);
		x+=fh->W;
		if(warn&&m_spatulaWarningTimer&32)
		{
			m_fontBank->setColour(255,0,0);
		}
		m_fontBank->print(x,y,countBuf);
		m_fontBank->setColour(128,128,128);
		x+=SB_UI_GAP_FROM_SPAT_COUNT_TO_PICKUPS;

		if(isWearingBoots())
		{
			// Boots
			int			pickupX,pickupY;
			sFrameHdr	*fh=sb->getFrameHeader(FRM__SHOE);
			if(m_squeakyBootsTimer>SQUEAKY_BOOTS_FLASH_TIME||m_squeakyBootsTimer&2)
			{
				sb->printFT4(fh,x,y,0,0,POWERUPUI_OT);
				sb->printFT4(fh,x+4,y+4,0,0,POWERUPUI_OT);
			}
			x+=fh->W+SB_UI_GAP_BETWEEN_ITEMS+4;
		}

		// Mode specific ui
		m_currentPlayerModeClass->renderModeUi();


		// Fiving helmet.. now appears in same place as mode specific ui..
		if(isWearingDivingHelmet())
		{
			// Helmet
			POLY_FT4	*ft4;
			int			x,y;
			int			V,W,H,partH;

			x=POWERUPUI_ICONX;
			y=POWERUPUI_ICONY;

			ft4=sb->printFT4(FRM__WATERHILIGHT,x,y,0,0,POWERUPUI_OT);
			setSemiTrans(ft4,true);

			fh=sb->getFrameHeader(FRM__WATER);
			ft4=sb->printFT4(fh,0,0,0,0,POWERUPUI_OT);
			setSemiTrans(ft4,true);
			V=fh->V;
			W=fh->W;
			H=fh->H;
			partH=(H*(255-(m_healthWaterLevel>>WATERLEVELSHIFT)))>>8;
			if(partH>H)partH=H;
			setXYWH(ft4,x,y+(partH),W,H-partH);
			ft4->v0=V+(partH);
			ft4->v1=V+(partH);

			sb->printFT4(FRM__WATERMETER,x,y,0,0,POWERUPUI_OT);

			if(!getIsInWater()&&m_healthWaterLevel<(WATER_COUNTER_SECONDTIME*6)&&m_currentPlayerModeClass->getState()!=STATE_SOAKUP)
			{
				int		digit;
				DVECTOR	src={x+(W/2),y+(H/2)};
				DVECTOR	dst={INGAME_SCREENW/2,(INGAME_SCREENH/3)*1};
				int		frame;
				int		digitX,digitY,scale;
				char	buf[4];
				int		r,g,b;

				digit=m_healthWaterLevel/WATER_COUNTER_SECONDTIME;
				if(digit<0)digit=0;

				frame=WATER_COUNTER_SECONDTIME-(m_healthWaterLevel%WATER_COUNTER_SECONDTIME);
				if(frame>WATER_COUNTER_MOVINGTIME)frame=WATER_COUNTER_MOVINGTIME;
				digitX=src.vx+(((dst.vx-src.vx)*frame)/WATER_COUNTER_MOVINGTIME);
				digitY=src.vy+(((dst.vy-src.vy)*frame)/WATER_COUNTER_MOVINGTIME);
				scale=WATER_COUNTER_STARTSCALE+(((WATER_COUNTER_ENDSCALE-WATER_COUNTER_STARTSCALE)*frame)/WATER_COUNTER_MOVINGTIME);

				sprintf(buf,"%d",digit);
				m_scalableFontBank->setJustification(FontBank::JUST_CENTRE);
				m_scalableFontBank->setScale(scale);
				r=WATER_COUNTER_R2-(((WATER_COUNTER_R2-WATER_COUNTER_R1)*(m_healthWaterLevel))/(WATER_COUNTER_SECONDTIME*6));
				g=WATER_COUNTER_G2-(((WATER_COUNTER_G2-WATER_COUNTER_G1)*(m_healthWaterLevel))/(WATER_COUNTER_SECONDTIME*6));
				b=WATER_COUNTER_B2-(((WATER_COUNTER_B2-WATER_COUNTER_B1)*(m_healthWaterLevel))/(WATER_COUNTER_SECONDTIME*6));
				m_scalableFontBank->setColour(r,g,b);
				m_scalableFontBank->print(digitX,digitY,buf);
			}

			x+=fh->W+SB_UI_GAP_BETWEEN_ITEMS;
		}
	
	}
	promptRender();

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
	Purpose:	Like the normal setRespawnPos() but plays the telephone ring sound if
				this is not the current respawn point. To be used from the respawn triggers
				so that they make the ring sound
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::setRespawnPosAndRingTelephone(DVECTOR const &_respawn)
{
	if(m_respawnPos.vx!=_respawn.vx||
	   m_respawnPos.vy!=_respawn.vy)
	{
		CSoundMediator::playSfx(CSoundMediator::SFX_TELEPHONE_BOX);
		setRespawnPos(_respawn);
	}
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

	height=CGameScene::getCollision()->getHeightFromGround(_x,_y,_maxHeight);
	if(height>=_maxHeight)
	{
		CThing *platform;
		platform=isOnPlatform();
		if(platform)
		{
			int platformHeight=((CNpcPlatform*)platform)->getHeightFromPlatformAtPosition(_x,_y);

			if ( platformHeight < height )
			{
				height = platformHeight;
				if(height>_maxHeight)height=_maxHeight;
				else if(height<-_maxHeight)height=-_maxHeight;
			}
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
	return( CGameScene::getCollision()->getHeightFromGround(_x,_y,_maxHeight) );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::setLedgeLookAhead(int _lookAhead)
{
	if(m_ledgeLookAhead!=_lookAhead)
	{
		m_ledgeLookAhead=_lookAhead;
		m_ledgeLookTimer=0;
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
	CGameSlotManager::GameSlot	*gameSlot;

	gameSlot=CGameSlotManager::getSlotData();

	if(gameSlot->m_lives<MAX_LIVES)
	{
		gameSlot->m_lives++;
	}

	m_pantFlashTimer=PANT_FLASH_TIME;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::addSpatula(int Count=1)
{
	m_numSpatulasHeld+=Count;

	if(!m_hasReceivedExtraLifeFor100Spats&&m_numSpatulasHeld==100)
	{
		addLife();
		m_hasReceivedExtraLifeFor100Spats=true;
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
		s_addonActorGfx[_addon]=CActorPool::GetActor(sbk);
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
	int	state;

	if(_mode==PLAYER_MODE_DEAD)
	{
		ASSERT(m_currentMode!=PLAYER_MODE_DEAD);
		m_lastModeBeforeDeath=m_currentMode;
	}

	resetPlayerCollisionSizeToBase();

	if(m_currentPlayerModeClass)
	{
		state=m_currentPlayerModeClass->getState();
	}
	else
	{
		state=STATE_IDLE;
	}
	m_currentMode=_mode;
	m_currentPlayerModeClass=s_playerModes[_mode];
	m_currentPlayerModeClass->setInitialState(state);
	if(!m_currentPlayerModeClass->setState(state))
	{
		m_currentPlayerModeClass->setState(STATE_IDLE);
		m_moveVelocity.vx=m_moveVelocity.vy=0;
	}
	m_currentPlayerModeClass->enter();

	m_tryingToManuallyPickupWeapon=false;
	m_tryingToAutomaticallyPickupWeapon=false;
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
void	CPlayer::springPlayerUp(int _springHeight)
{
	m_currentPlayerModeClass->springPlayerUp(_springHeight);
	CSoundMediator::playSfx(CSoundMediator::SFX_BOUNCY_PLATFORM,false,true);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::setFloating()
{
	m_currentPlayerModeClass->setFloating();
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int haha=25;
void	CPlayer::floatPlayerUp()
{
	m_moveVelocity.vy-=haha;
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

	m_teleportPendingPos=pos;
	m_teleportPending=true;
}

void CPlayer::processTeleportToCommand()
{
	ASSERT(m_teleportPending);

	CameraBox	releaseCamBox={0,0,29999,29999};
	setCameraBox(releaseCamBox);

	setPos(m_teleportPendingPos);
	setRespawnPos(m_teleportPendingPos);

	calcCameraFocusPointTarget();
	m_currentCamFocusPoint=m_currentCamFocusPointTarget;
	m_teleportPending=false;
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
				if(isWearingBoots())
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
	m_currentCamFocusPointTarget.vx=Pos.vx+MAP2D_CENTRE_X-(m_cameraXScrollPos>>CAMERA_ACCURACYSHIFT);
	m_currentCamFocusPointTarget.vy=Pos.vy+MAP2D_CENTRE_Y-m_cameraFallYScrollPos;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::respawn()
{
	// Set previous mode
	setMode(m_lastModeBeforeDeath);

	m_allowConversation=false;

	m_numSpatulasHeld=0;
	m_spatulaWarningTimer=0;
	m_healthWaterLevel=WATERMAXHEALTH;
	m_invincibleFrameCount=INVINCIBLE_FRAMES__START;
	m_recoveringFromElectrocution=false;
	m_helmetSoundTimer=0;
	Pos=m_respawnPos;
	m_cameraLookOffset=0;

	m_cameraXScrollDir=0;
	m_cameraXScrollPos=0;

	m_cameraFallYScrollPos=0;
	m_isFalling=false;
	m_fallFrames=false;
	m_cameraFallYScrollSpeed=0;

	calcCameraFocusPointTarget();
	m_currentCamFocusPoint=m_currentCamFocusPointTarget;
	m_cameraPos.vx=m_currentCamFocusPoint.vx;
	m_cameraPos.vy=m_currentCamFocusPoint.vy;

	m_padLookAroundTimer=0;
	m_ledgeLookAhead=0;
	m_ledgeLookOffset=0;
	m_ledgeLookTimer=0;
	m_tryingToManuallyPickupWeapon=false;
	m_tryingToAutomaticallyPickupWeapon=false;

	m_squeakyBootsTimer=0;
	m_invincibilityRingTimer=0;
	m_bubbleAmmo=0;
	m_jellyAmmo=0;
	m_jellyfishAmmoCount=0;
	m_pantFlashTimer=0;
	m_hasReceivedExtraLifeFor100Spats=false;

	// Re-stock ammo if need be
	if(m_currentMode==PLAYER_MODE_BUBBLE_MIXTURE)
	{
		giveBubbleAmmoFromWeapon();
	}
	else if(m_currentMode==PLAYER_MODE_JELLY_LAUNCHER)
	{
		giveJellyAmmoFromWeapon();
	}

	m_moveVelocity.vx=m_moveVelocity.vy=0;

	clearPlatform();

	updateCollisionArea();

	m_ignoreNewlyPressedButtonsOnPadThisThink=true;

	m_canExitLevel=false;
	m_teleportPending=false;

	setSBOTPos(OTPOS__ACTOR_POS);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:	Yes - This function is fat! It can be tidied up when all of the anims
				are finalised. Etracting the repeated code to a function will probly not
				improve things cos of the amount of data that would need to be passed about.
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const int	ExtraListYPos=50-32;
void CPlayer::renderSb(DVECTOR *_pos,int _animNo,int _animFrame)
{
	int			playerMode;
	int			trans;
	int			addon;
	POLY_FT4	*ft4;


	// Y clipping ( damn useful when player is dead.. ;)
	if(_pos->vy<0-256||_pos->vy>256+256)
	{
		return;
	}


	if(m_currentMode==PLAYER_MODE_DEAD)
	{
		playerMode=m_lastModeBeforeDeath;
	}
	else
	{
		playerMode=m_currentMode;
	}
	trans=m_invincibleFrameCount||m_invincibilityRingTimer;

	// Render an addon?
	addon=s_addonNumbers[playerMode];
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
	if(playerMode==PLAYER_MODE_NET&&getJellyFishAmmo())
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
					setShadeTex(ft4,0);
					setRGB0(ft4,255,128,255);
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
	CThing	*platform;
	platform=isOnPlatform();
	if(!isDead())
	{
		if(platform)
		{
			if ( ( (CNpcPlatform *) platform )->isCart() )
			{
				m_actorGfx->RotateScale( ft4, *_pos, ( (CNpcPlatform *) platform )->getCollisionAngle() , ONE, ONE );
			}
		}
	}
	setSemiTrans(ft4,trans);


	// Pants?
	if(m_pantFlashTimer>0)
	{
/*		SpriteBank	*sb=CGameScene::getSpriteBank();
		sFrameHdr	*fh;
		DVECTOR		drawPos;
		int			size;

		fh=sb->getFrameHeader(FRM__PANTS);
		drawPos=*_pos;
		drawPos.vy+=PANT_FLASH_Y_OFFSET;
		size=m_pantFlashTimer&8?4096:8192;
		ft4=sb->printRotatedScaledSprite(fh,drawPos.vx,drawPos.vy,size,size,0,0);
*/
		if (FrameFlipFlag&1 && !GameScene.getIsPaused())
		{
			m_scalableFontBank->setJustification(FontBank::JUST_CENTRE);
			m_scalableFontBank->setScale(511);
			m_scalableFontBank->setColour(255,255,255);
			m_scalableFontBank->print( 256, ExtraListYPos, "Extra Life!");
		}
	}
	

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
void	CPlayer::ignoreNewlyPressedButtonsOnPadThisThink()
{
	m_ignoreNewlyPressedButtonsOnPadThisThink=true;
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
	   (CGameScene::getCollision()->getCollisionBlock(Pos.vx,Pos.vy)&COLLISION_TYPE_MASK)==COLLISION_TYPE_FLAG_SOAKUP)
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
#if	defined(__USER_daveo__) || defined(__USER_paul__)
int invincibleSponge=true;		// NB: This is for debugging purposes only so don't try and use it for a permenant cheat mode..
#else
int invincibleSponge=false;		// NB: This is for debugging purposes only so don't try and use it for a permenant cheat mode..
#endif
void CPlayer::takeDamage(DAMAGE_TYPE _damage,REACT_DIRECTION _reactDirection,CThing *_thing)
{
	if((m_invincibleFrameCount==0||_damage==DAMAGE__KILL_OUTRIGHT)&&	// Don't take damage if still recovering from the last hit
	   m_invincibilityRingTimer==0&&									// Or if we have the invincibility ring on
	   m_currentPlayerModeClass->getState()!=STATE_SOAKUP&&				// Or soaking up
	   m_currentPlayerModeClass->getState()!=STATE_CELEBRATE&&			// Or celebrating
	   m_currentMode!=PLAYER_MODE_DEAD &&								// Or already dead! :)
	   m_currentMode!=PLAYER_MODE_SWALLOW)								
	{
		int						ouchThatHurt=true;
		int						ouchThatHurtSoMuchThatImJustGoingToDieNow=false;
		CSoundMediator::SFXID	painSound=CSoundMediator::SFX_SPONGEBOB_OUCH;

		// Check if we are currently immune to this damage type
		m_recoveringFromElectrocution=false;
		switch(_damage)
		{
			case DAMAGE__NONE:
				ouchThatHurt=false;
				break;

			case DAMAGE__ELECTROCUTION:
			case DAMAGE__SHOCK_ENEMY:
				if(isWearingBoots())
				{
					ouchThatHurt=false;
				}
				else
				{
					painSound=CSoundMediator::SFX_BOSS_JELLYFISH___SB_GETS_ELECTROCUTED;
					m_recoveringFromElectrocution=true;
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
			case DAMAGE__COLLISION_DAMAGE:
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
				if(ouchThatHurtSoMuchThatImJustGoingToDieNow||getSpatulasHeld()==0/*&&m_currentMode!=PLAYER_MODE_NET)*/)
				{
					died=true;
				}
				else
				{
					// NET REMOVED FROM HIT SYSTEM (PKG)
					/*
					if(m_currentMode==PLAYER_MODE_NET)
					{
						// Launch net pickup
						setMode(PLAYER_MODE_BASICUNARMED);
						DVECTOR	pickupPos;
						CBasePickup	*pickup;
						pickupPos.vx=Pos.vx;
						pickupPos.vy=Pos.vy-30;
						pickup=createPickup(PICKUP__NET,&pickupPos);
						pickup->setPos(&pickupPos);
					}
					else
					*/
					{
						// Launch all spatulas!
						GameScene.dropHealth(Pos,m_numSpatulasHeld,1);
						m_numSpatulasHeld=0;
						m_spatulaWarningTimer=0;
					}
				}
			}

			if(died)
			{
				DEATH_TYPE	deathType;
				switch(_damage)
				{
					case DAMAGE__SQUASH_ENEMY:
						deathType=DEATHTYPE__SQUASH;
						break;

					default:
						deathType=DEATHTYPE__NORMAL;
						break;
				}
				dieYouPorousFreak(deathType);
				CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_LONG_STROBEY);
			}
			else
			{
				CThing	*platform;
				platform=isOnPlatform();
				if(platform)
				{
					if ( ( (CNpcPlatform *) platform )->isCart() )
					{
						m_invincibleFrameCount=INVINCIBLE_FRAMES__HIT;
						return;
					}
				}

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
					CSoundMediator::playSfx(painSound);
					m_currentPlayerModeClass->setState(STATE_JUMPBACK);
				}
				m_invincibleFrameCount=INVINCIBLE_FRAMES__HIT;
				CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_SHORT);
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
void	CPlayer::collectedQuestItem()
{
	m_currentPlayerModeClass->setState(STATE_CELEBRATE);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CPlayer::getLivesLeft()
{
	return CGameSlotManager::getSlotData()->m_lives;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::dieYouPorousFreak(DEATH_TYPE _deathType)
{
	ASSERT(m_currentMode!=PLAYER_MODE_DEAD);

	m_deathType=_deathType;
	CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_DEFEATED_JINGLE);
	setMode(PLAYER_MODE_DEAD);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	When SB dies and falls off the screen, he needs to be drawn in
				front of the level - hence this
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::setSBOTPosToFront()
{
	setSBOTPos(0);
}

void CPlayer::setSBOTPos(int _ot)
{
	int	i;

	for(i=0;i<NUM_PLAYER_ADDONS;i++)
	{
		if(s_addonActorGfx[i])
		{
			s_addonActorGfx[i]->SetOtPos(_ot);
		}
	}

	m_actorGfx->SetOtPos(_ot);
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
	if(!m_ignoreNewlyPressedButtonsOnPadThisThink)
	{
		m_padInputDown=(PLAYERINPUT)(m_padInput&(m_lastPadInput^-1));
	}
	else
	{
		m_padInputDown=PI_NONE;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
typedef struct
{
	CPadConfig::PAD_CFG	m_input;
	PLAYERINPUT			m_output;
} PAD_CONVERSION;
static const PAD_CONVERSION	s_padConversionTable[]=
{
	{	CPadConfig::PAD_CFG_UP,				PI_UP			},
	{	CPadConfig::PAD_CFG_DOWN,			PI_DOWN			},
	{	CPadConfig::PAD_CFG_LEFT,			PI_LEFT			},
	{	CPadConfig::PAD_CFG_RIGHT,			PI_RIGHT		},
	{	CPadConfig::PAD_CFG_JUMP,			PI_JUMP			},
	{	CPadConfig::PAD_CFG_FIRE,			PI_FIRE			},
	{	CPadConfig::PAD_CFG_CATCH,			PI_CATCH		},
	{	CPadConfig::PAD_CFG_WEAPONCHANGE,	PI_WEAPONCHANGE	},
};
static const int s_padConversionTableSize=sizeof(s_padConversionTable)/sizeof(PAD_CONVERSION);
PLAYERINPUT CPlayer::readPadInput()
{
	PLAYERINPUT				input;
	int						pad;
	const PAD_CONVERSION	*table;
	int						i;

	input=PI_NONE;
	pad=PadGetHeld(0);
	table=s_padConversionTable;
	for(i=0;i<s_padConversionTableSize;i++)
	{
		if(pad&CPadConfig::getButton(table->m_input))
		{
			input=(PLAYERINPUT)(input|table->m_output);
		}
		table++;
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
	CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_BUTTBOUNCE);
	m_currentPlayerModeClass->setState(STATE_BUTTBOUNCEUP);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayer::shove( DVECTOR const &move )
{
	int		colHeight;

	// X movement
	int xDist = move.vx;

	colHeight=0;

	while( xDist )
	{
		colHeight=CGameScene::getCollision()->getHeightFromGround(Pos.vx+xDist,Pos.vy-HEIGHT_FOR_HEAD_COLLISION,HEIGHT_FOR_HEAD_COLLISION+1);
		if ( colHeight < HEIGHT_FOR_HEAD_COLLISION )
		{
			break;
		}

		if ( xDist > 0 )
		{
			if ( xDist >= 8 )
			{
				xDist -= 8;
			}
			else
			{
				xDist = 0;
			}
		}
		else if ( xDist < 0 )
		{
			if ( xDist <= -8 )
			{
				xDist += 8;
			}
			else
			{
				xDist = 0;
			}
		}
	}

	if(colHeight<HEIGHT_FOR_HEAD_COLLISION)
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
			if(CGameScene::getCollision()->getHeightFromGround(cx,Pos.vy-HEIGHT_FOR_HEAD_COLLISION,HEIGHT_FOR_HEAD_COLLISION+1)<HEIGHT_FOR_HEAD_COLLISION)
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
	colHeight=CGameScene::getCollision()->getHeightFromGround(Pos.vx,Pos.vy+move.vy,5);
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
			if(CGameScene::getCollision()->getHeightFromGround(Pos.vx,cy)<0)
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

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::moveLeft()
{
	if(m_moveVelocity.vx<-CAMERA_STARTMOVETHRESHOLD||m_cameraXScrollPos<-(CAMERA_SCROLLTHRESHOLD*CAMERA_TILESIZE)<<CAMERA_ACCURACYSHIFT)
	{
		m_cameraXScrollDir=m_reverseCameraMovement==false?+1:-1;
	}
	else if(m_moveVelocity.vx>-CAMERA_STOPMOVETHRESHOLD)
	{
		m_cameraXScrollDir=0;
	}
}
void	CPlayer::moveRight()
{
	if(m_moveVelocity.vx>CAMERA_STARTMOVETHRESHOLD||m_cameraXScrollPos>(CAMERA_SCROLLTHRESHOLD*CAMERA_TILESIZE)<<CAMERA_ACCURACYSHIFT)
	{
		m_cameraXScrollDir=m_reverseCameraMovement==false?-1:+1;
	}
	else if(m_moveVelocity.vx<CAMERA_STOPMOVETHRESHOLD)
	{
		m_cameraXScrollDir=0;
	}
}
void	CPlayer::fall()
{
	m_isFalling=true;
}
void	CPlayer::buttFall()
{
	m_cameraFallYScrollSpeed=CAMERA_FALL_DROP_SPEED;
	m_isFalling=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::setPlatform(CThing *_newPlatform)
{
	if ( _newPlatform != m_lockoutPlatform )
	{
		m_platform=_newPlatform;
		m_lockoutPlatform = NULL;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::clearPlatform()
{
	m_platform=NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::setLockoutPlatform(CThing *_newPlatform)
{
	// this platform is not to be used until the player has hit either
	// (a) another platform
	// (b) the ground

	// should stop player-platform interaction when he is knocked off by collision

	m_lockoutPlatform = _newPlatform;
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayer::forceFall()
{
	if(m_currentMode!=PLAYER_MODE_DEAD)
	{
		if ( m_currentPlayerModeClass->getState() != STATE_FALL )
		{
			m_currentPlayerModeClass->setState(STATE_FALL);
			DVECTOR	moveVel;

			moveVel=*getMoveVelocity();
			moveVel.vy=0;
			m_fallFrames=0;
			setMoveVelocity(&moveVel);
		}
	}
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
	int				hitThisSuspectBlock;

	pos=Pos;
	hitGround=false;
	hitThisSuspectBlock=0;

	// Are we falling?
	if(_moveDistance>0)
	{
		if ( isOnPlatform() )
		{
			// must also detect that we have landed on platform

			return( true );
		}

		int colHeightBefore[2],colHeightAfter[2],blockAfter[2],moveRequired[2];
		int	i;
		int	move;

		// Get heights of the two edges
		for(i=0;i<+2;i++)
		{
			int x=pos.vx+((i==0?-checkx:+checkx));
			colHeightBefore[i]=getHeightFromGround(x,pos.vy,16);
			colHeightAfter[i]=getHeightFromGround(x,pos.vy+_moveDistance,16);
			blockAfter[i]=CGameScene::getCollision()->getCollisionBlock(x,pos.vy+_moveDistance);
		}

		// See if either side is about to go through the ground
		for(i=0;i<2;i++)
		{
			if(colHeightBefore[i]>=0&&colHeightAfter[i]<=0)
			{
				int	goingIntoThisBlock=blockAfter[i]&COLLISION_TYPE_MASK;
				if(goingIntoThisBlock!=COLLISION_TYPE_FLAG_DEATH_FALL)
				{
					moveRequired[i]=colHeightAfter[i];
					hitGround=true;
				}
				else
				{
					moveRequired[i]=0;
				}
				if(!hitThisSuspectBlock)hitThisSuspectBlock=goingIntoThisBlock;
			}
			else
			{
				moveRequired[i]=0;
			}
		}

		// Find the smallest move required to hit ground
		move=0;
		for(i=0;i<2;i++)
		{
			if(moveRequired[i]<move)
			{
				move=moveRequired[i];
			}
		}

		_moveDistance+=move;
	}
	else if(_moveDistance<0)
	{
		int colHeightBefore[3],colHeightAfter[3],blockAfter[3],moveRequired[3];
		int	i;
		int	move;

		// -------------- Jumping - See if the feet have hit anything --------------

		// Get heights of the two edges
		for(i=-1;i<+2;i++)
		{
			int x=pos.vx+(checkx*i);
			colHeightBefore[i+1]=getHeightFromGround(x,pos.vy,16);
			colHeightAfter[i+1]=getHeightFromGround(x,pos.vy+_moveDistance,16);
			blockAfter[i+1]=CGameScene::getCollision()->getCollisionBlock(x,pos.vy+_moveDistance);
		}

		// See if either side is about to go through the ground
		for(i=0;i<3;i++)
		{
			if(colHeightBefore[i]>=0&&colHeightAfter[i]<=0&&((blockAfter[i]&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_NORMAL))
			{
				moveRequired[i]=colHeightAfter[i];
				hitGround=true;
				if(!hitThisSuspectBlock)hitThisSuspectBlock=blockAfter[i];
			}
			else
			{
				moveRequired[i]=0;
			}
		}

		// Find the smallest move required to hit ground
		move=0;
		for(i=0;i<3;i++)
		{
			if(moveRequired[i]<move)
			{
				move=moveRequired[i];
			}
		}

		_moveDistance-=move;


		// -------------- Jumping - See if the head has hit anything --------------
		
		// Get heights of the two edges
		int	y=pos.vy-HEIGHT_FOR_HEAD_COLLISION;
		for(i=-1;i<+2;i++)
		{
			int x=pos.vx+(checkx*i);
			colHeightBefore[i+1]=getHeightFromGround(x,y,16);
			colHeightAfter[i+1]=getHeightFromGround(x,y+_moveDistance,16);
			blockAfter[i+1]=CGameScene::getCollision()->getCollisionBlock(x,y+_moveDistance);
		}

		bool isGoingToFall = false;

		// See if either side is about to go through the ground
		for(i=0;i<3;i++)
		{
			if(colHeightBefore[i]>=0&&colHeightAfter[i]<=0&&((blockAfter[i]&COLLISION_TYPE_MASK)!=COLLISION_TYPE_FLAG_NORMAL))
			{
				moveRequired[i]=16+colHeightAfter[i];
				//moveRequired[i]=colHeightAfter[i];
//				hitGround=true;

				// do not call hitground code, because this will set it to STATE_IDLE for a frame
				// instead, do the appropriate stuff for a fall
				if(!hitThisSuspectBlock)hitThisSuspectBlock=blockAfter[i];

				isGoingToFall = true;
			}
			else
			{
				moveRequired[i]=0;
			}
		}

		if ( isGoingToFall )
		{
			// check where feet are

			if ( getHeightFromGround(pos.vx, pos.vy, 16 ) == 0 )
			{
				// standing on ground, hence do not fall
				hitGround = true;
			}
			else
			{
				m_currentPlayerModeClass->setState(STATE_FALL);
				DVECTOR	moveVel;

				moveVel=*getMoveVelocity();
				moveVel.vy=0;
				m_fallFrames=0;
				setMoveVelocity(&moveVel);
			}
		}

		// Find the smallest move required to hit ground
		move=0;
		for(i=0;i<3;i++)
		{
			if(moveRequired[i]<move)
			{
				move=moveRequired[i];
			}
		}

		_moveDistance-=move;
	}
	else
	{
		// Stood on any important types of collision?
		hitThisSuspectBlock=CGameScene::getCollision()->getCollisionBlock(pos.vx,pos.vy+_moveDistance);
	}

	pos.vy+=_moveDistance;
	setPlayerPos(&pos);


	if(m_currentMode!=PLAYER_MODE_DEAD)
	{
		switch(hitThisSuspectBlock&COLLISION_TYPE_MASK)
		{
			case COLLISION_TYPE_FLAG_DAMAGE:
				takeDamage(DAMAGE__COLLISION_DAMAGE);
				break;
			case COLLISION_TYPE_FLAG_ELECTRIC:
				if(!isWearingBoots())
				{
					takeDamage(DAMAGE__COLLISION_DAMAGE);
				}
				break;
				/*
			case COLLISION_TYPE_FLAG_DEATH_LIQUID:
				dieYouPorousFreak(DEATHTYPE__LIQUID);
				break;
			case COLLISION_TYPE_FLAG_DEATH_INSTANT:
				dieYouPorousFreak(DEATHTYPE__NORMAL);
				break;
				*/
			case COLLISION_TYPE_FLAG_DEATH_FALL:
				dieYouPorousFreak(DEATHTYPE__FALL_TO_DEATH);
				break;
			default:
				break;
		}
	}

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
		DVECTOR			pos;
		int				dirToMove;
		int				i;
		int				colHeightEdges[2];
		int				colHeight;

		pos=getPlayerPos();
		dirToMove=_moveDistance<0?-1:+1;
		for(i=abs(_moveDistance);i&&!hitWall;i--)
		{
			int	touchingGround,x,x2;

			colHeightEdges[0]=getHeightFromGround(pos.vx-checkx,pos.vy,16);
			colHeightEdges[1]=getHeightFromGround(pos.vx+checkx,pos.vy,16);

			touchingGround=false;
			x=x2=pos.vx+dirToMove;

			if(dirToMove>0)
			{
				// Moving right
				if(colHeightEdges[1]==0)
				{
					// Right side of SB touching ground
					x+=checkx;
					x2-=checkx;
					touchingGround=true;
				}
				else if(colHeightEdges[0]==0)
				{
					// Left side of SB touching ground
					x-=checkx;
					x2+=checkx;
					touchingGround=true;
				}
			}
			else
			{
				// Moving left
				if(colHeightEdges[0]==0)
				{
					// Left side of SB touching ground
					x-=checkx;
					x2+=checkx;
					touchingGround=true;
				}
				else if(colHeightEdges[1]==0)
				{
					// Right side of SB touching ground
					x+=checkx;
					x2-=checkx;
					touchingGround=true;
				}
			}

			if(touchingGround)
			{
				// SB touching ground on at least one edge
				colHeight=getHeightFromGround(x,pos.vy,16);

				// Head collision ( a bit of a hack )
				int oktomove=false;
				switch ( CGameScene::getCollision()->getCollisionBlock( x, pos.vy-HEIGHT_FOR_HEAD_COLLISION ) & COLLISION_TYPE_MASK )
				{
					case COLLISION_TYPE_NORMAL:
						oktomove=true;
						break;

					default:
						break;
				}
				if(oktomove)
				{
					if(colHeight>=-2&&colHeight<=2)
					{
						// Move along the ground
						pos.vx+=dirToMove;
						pos.vy+=colHeight;

						// Heh - these 4 lines stop SB going down a slope on the 'wrong edge' :)
						colHeight=getHeightFromGround(x2,pos.vy,16);
						if(colHeight<0)
						{
							pos.vy+=colHeight;
						}
					}
					else if(colHeight<0)
					{
						// Hit wall
						hitWall=true;
					}
					else
					{
						// Moved off edge of ledge
						pos.vx+=dirToMove;
					}
				}
			}
			else
			{
				// SB not touching ground
				if(dirToMove<0)
				{
					x-=checkx;
				}
				else
				{
					x+=checkx;
				}

				// Head collision ( cheers Charles! :)
				colHeight = getHeightFromGround( x, pos.vy-HEIGHT_FOR_HEAD_COLLISION, 16 );
				if ( colHeight < 0 )
				{
					switch ( CGameScene::getCollision()->getCollisionBlock( x, pos.vy-HEIGHT_FOR_HEAD_COLLISION ) & COLLISION_TYPE_MASK )
					{
						case COLLISION_TYPE_NORMAL:
							break;

						default:
							hitWall=true;
							break;
					}
				}

				if ( !hitWall )
				{
					colHeight=getHeightFromGround(x,pos.vy,16);
					if(colHeight>=0)
					{
						// Move in air
						pos.vx+=dirToMove;
					}
					else
					{
						// Hit wall
						hitWall=true;
					}
				}
			}
		}
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
DVECTOR const	&offset=getCollisionCentreOffset();
DVECTOR const	&size=getCollisionSize();

	*_x=offset.vx;
	*_y=offset.vy;
	*_w=size.vx;
	*_h=size.vy;
}

void CPlayer::getPlayerNormalCollisionSize(int *_x,int *_y,int *_w,int *_h)
{
	*_x=0;
	*_y=-COLSIZE_BASE_HEIGHT/2;
	*_w=COLSIZE_BASE_WIDTH;
	*_h=COLSIZE_BASE_HEIGHT;
}
/*****************************************************************************/
/*****************************************************************************/
/*** On Screen Prompts *******************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Prompt Data is stored reverse order

const int	PromptIconX=32;
const int	PromptTextXOfs=20;
const int	PromptTextYOfs=-4;
const int	PromptY=(INGAME_SCREENH-32);
const int	PromptXGap=20;
const int	PromptYGap=12;
const int	PromptTMode=1;
const int	PromptOnScreenTime=50*5;
const int	PromptFadeSpeed=8;

static const CPlayer::sPromptData	KaratePromptData[]=
{
	{CPadConfig::PAD_CFG_WEAPONCHANGE,	STR_PROMPT_KARATE_UNEQUIP},
	{CPadConfig::PAD_CFG_FIRE,			STR_PROMPT_KARATE_CHOP},
	{CPadConfig::PAD_CFG_NONE,			STR_PROMPT_KARATE_TITLE}
};
static const CPlayer::sPromptData	BubblePromptData[]=
{
	{CPadConfig::PAD_CFG_WEAPONCHANGE,	STR_PROMPT_BUBBLEWAND_UNEQUIP},
	{CPadConfig::PAD_CFG_FIRE,			STR_PROMPT_BUBBLEWAND_BLOW},
	{CPadConfig::PAD_CFG_NONE,			STR_PROMPT_BUBBLEWAND_TITLE}
};
static const CPlayer::sPromptData	NetPromptData[]=
{
	{CPadConfig::PAD_CFG_WEAPONCHANGE,	STR_PROMPT_NET_UNEQUIP},
	{CPadConfig::PAD_CFG_CATCH,			STR_PROMPT_NET_CATCH},
	{CPadConfig::PAD_CFG_FIRE,			STR_PROMPT_NET_THROW},
	{CPadConfig::PAD_CFG_NONE,			STR_PROMPT_NET_TITLE}
};
static const CPlayer::sPromptData	CoralBlowerPromptData[]=
{
	{CPadConfig::PAD_CFG_WEAPONCHANGE,	STR_PROMPT_CORALBLOWER_UNEQUIP},
	{CPadConfig::PAD_CFG_UP,			STR_PROMPT_CORALBLOWER_AIM},
	{CPadConfig::PAD_CFG_CATCH,			STR_PROMPT_CORALBLOWER_SUCK},
	{CPadConfig::PAD_CFG_FIRE,			STR_PROMPT_CORALBLOWER_FIRE},
	{CPadConfig::PAD_CFG_NONE,			STR_PROMPT_CORALBLOWER_TITLE}
};
static const CPlayer::sPromptData	JellyLauncherPromptData[]=
{
	{CPadConfig::PAD_CFG_WEAPONCHANGE,	STR_PROMPT_JELLYLAUNCHER_UNEQUIP},
	{CPadConfig::PAD_CFG_FIRE,			STR_PROMPT_JELLYLAUNCHER_FIRE2},
	{CPadConfig::PAD_CFG_FIRE,			STR_PROMPT_JELLYLAUNCHER_FIRE},
	{CPadConfig::PAD_CFG_NONE,			STR_PROMPT_JELLYLAUNCHER_TITLE}
};

CPlayer::sPromptTable	CPlayer::PromptTable[NUM_PLAYERMODES]=
{
	{0,0},							// PLAYER_MODE_BASICUNARMED
	{KaratePromptData,0},			// PLAYER_MODE_FULLUNARMED
	{0,0},							// PLAYER_MODE_BALLOON
	{BubblePromptData,0},			// PLAYER_MODE_BUBBLE_MIXTURE
	{NetPromptData,0},				// PLAYER_MODE_NET
	{CoralBlowerPromptData,0},		// PLAYER_MODE_CORALBLOWER
	{JellyLauncherPromptData,0},	// PLAYER_MODE_JELLY_LAUNCHER
	{0,0},							// PLAYER_MODE_DEAD
	{0,0},							// PLAYER_MODE_FLY
	{0,0},							// PLAYER_MODE_CART
	{0,0},							// PLAYER_MODE_SWALLOW
};

/*****************************************************************************/
void	CPlayer::promptThink(int _frames)
{
int		NewPrompt=m_currentMode;
	
		if (NewPrompt==0)
		{ // No pickup- so kill all prompts
			PromptFade=-PromptFadeSpeed*4;
		}

		if (!PromptTable[NewPrompt].Data || PromptTable[NewPrompt].Shown)
		{
			NewPrompt=-1;
		}
	
// Check for Prompt change
		if (CurrentPrompt!=NewPrompt && NewPrompt!=-1)
		{
			if (CurrentPrompt==-1)
			{ // no prompt, so just display it
				CurrentPrompt=NewPrompt;
				PromptTimer=PromptOnScreenTime;
				PromptRGB=0;
				PromptFade=+PromptFadeSpeed;
				PromptTable[CurrentPrompt].Shown=1;	// Set as shown
			}
			else
			{
				PromptFade=-PromptFadeSpeed;
				if (NewPrompt!=-1)
				{
					PromptFade*=4;	// Fast fade, cos new prompt waiting
				}
			}
		}

// Control Current Prompt
	if (CurrentPrompt!=-1)
	{
		PromptRGB+=PromptFade;
		if (PromptRGB>127)
		{ // Fade in/on screen
			PromptRGB=127;
			PromptTimer-=_frames;
			if (PromptTimer<0)
			{
				PromptFade=-PromptFadeSpeed;
			}
		}
		else
		if (PromptRGB<0) 
		{
			PromptRGB=0;
			CurrentPrompt=-1;
		}
	
	}
	
}

/*****************************************************************************/

void	CPlayer::promptRender()
{
	if (CurrentPrompt==-1) return;

sPromptTable	&ThisTable=PromptTable[CurrentPrompt];
sPromptData	*Ptr=(sPromptData*)ThisTable.Data;
int			X=0;
int			Y=PromptY-PromptYGap;
SpriteBank	*sb=CGameScene::getSpriteBank();
POLY_FT4	*Ft4;
int			MaxTLen=0;

		if (!Ptr) return;	// no prompt, so go away

		m_fontBank->setOt(0);
		m_fontBank->setTrans(1);
		m_fontBank->setColour(PromptRGB,PromptRGB,PromptRGB);
		m_fontBank->setSMode(PromptTMode);

		while ((CPadConfig::PAD_CFG)Ptr->m_input!=CPadConfig::PAD_CFG_NONE)
		{
			X=PromptIconX;
			int	Icon[2]={0,0};
			switch(CPadConfig::getButton((CPadConfig::PAD_CFG)Ptr->m_input))
			{
				case PAD_CROSS:		Icon[0]=FRM__BUTX;	break;
				case PAD_TRIANGLE:	Icon[0]=FRM__BUTT;	break;
				case PAD_CIRCLE:	Icon[0]=FRM__BUTC;	break;
				case PAD_SQUARE:	Icon[0]=FRM__BUTS;	break;
				case PAD_UP:		Icon[0]=FRM__BUTU;	Icon[1]=FRM__BUTD; break;
				default:			ASSERT(!"Unknown Pad Button");	break;

			}
			for (int i=0; i<2; i++)
			{
				if (Icon[i])
				{
					//Icon
					Ft4=sb->printFT4(Icon[i],X,Y,0,0,0); setSemiTrans(Ft4,1); Ft4->tpage|=PromptTMode<<5; setRGB0(Ft4,PromptRGB,PromptRGB,PromptRGB);
					//Icon Mask	- to aid alpha fade
					int	Col=(PromptRGB*3)/2;
					Ft4=sb->printFT4(Icon[i],X,Y,0,0,0); setSemiTrans(Ft4,1); Ft4->tpage|=2<<5;			setRGB0(Ft4,Col,Col,Col);
					X+=PromptXGap;
				}
			}
// text
			
			int	TLen=m_fontBank->getStringWidth(Ptr->Text);
			if (MaxTLen<TLen) MaxTLen=TLen;
			m_fontBank->print(X,Y+PromptTextYOfs,Ptr->Text);

			Y-=PromptYGap;
			Ptr++;
		}
// Title
		m_fontBank->print(PromptIconX,Y+PromptTextYOfs,Ptr->Text);
// Background
			
int			BackRGB=PromptRGB/2;
TPOLY_F4	*F4=GetPrimTF4(); setTSemiTrans(F4,1); setTABRMode(F4,0);

			setXYWH(F4,PromptIconX-2,Y-2,MaxTLen+(X-PromptIconX)+4,(PromptY-Y)+4);
			setRGB0(F4,BackRGB,BackRGB,BackRGB);
			AddPrimToList(F4,0);
// revert Fontbank settings - shouldnt have to do this, grrrr
			m_fontBank->setTrans(0);
			m_fontBank->setColour(128,128,128);
			m_fontBank->setSMode(0);
}

/*===========================================================================
end */
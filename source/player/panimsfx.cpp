/*=========================================================================

	panimsfx.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	Handles automatically generated sound effects based upon anim frames

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\player.h"


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

	
// ANIM_PLAYER_ANIM_BUTTBOUNCEEND
static const CPlayer::AnimFrameSfx s_buttBounceEndSfx[]=
{
	{	1,	CSoundMediator::SFX_SPONGEBOB_BUTTBOUNCE,	},
};
static const int	s_buttBounceEndCount=sizeof(s_buttBounceEndSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_BUTTBOUNCESTART
// ANIM_PLAYER_ANIM_DEATHBACKWARDS
// ANIM_PLAYER_ANIM_DEATHDRY
// ANIM_PLAYER_ANIM_DEATHFORWARDS
// ANIM_PLAYER_ANIM_DEATHSPIN
// ANIM_PLAYER_ANIM_DEATHTAR
// ANIM_PLAYER_ANIM_ELECTRICSHOCK
// ANIM_PLAYER_ANIM_ELECTRICSHOCKEND
// ANIM_PLAYER_ANIM_ELECTRICSHOCKSTART
// ANIM_PLAYER_ANIM_FACEBACK
// ANIM_PLAYER_ANIM_FACEFRONT
// ANIM_PLAYER_ANIM_FALL
// ANIM_PLAYER_ANIM_GETUP
// ANIM_PLAYER_ANIM_GETUPRUN
// ANIM_PLAYER_ANIM_HITGROUND
// ANIM_PLAYER_ANIM_HOVER
// ANIM_PLAYER_ANIM_HOVEREND
// ANIM_PLAYER_ANIM_HOVERSTART
// ANIM_PLAYER_ANIM_IDLEBREATHE
// ANIM_PLAYER_ANIM_IDLEHOOLA
// ANIM_PLAYER_ANIM_IDLELOOK
// ANIM_PLAYER_ANIM_IDLEWIGGLEARM

// ANIM_PLAYER_ANIM_JUMPEND
static const CPlayer::AnimFrameSfx s_jumpEndSfx[]=
{
	{	4,		CSoundMediator::SFX_SPONGEBOB_WALK_2	},
	{	5,		CSoundMediator::SFX_SPONGEBOB_WALK_1	},
};
static const int	s_jumpEndCount=sizeof(s_jumpEndSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_KARATE			Might have to go into the CPlayerStateChop::enter code (PKG)
static const CPlayer::AnimFrameSfx s_chopSfx[]=
{
	{	1,	CSoundMediator::SFX_SPONGEBOB_KARATE_1,		},
};
static const int	s_chopCount=sizeof(s_chopSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_KNOCKBACK
// ANIM_PLAYER_ANIM_KNOCKFORWARD

// ANIM_PLAYER_ANIM_RUN
static const CPlayer::AnimFrameSfx s_runSfx[]=
{
	{	6,		CSoundMediator::SFX_SPONGEBOB_WALK_1	},
	{	18,		CSoundMediator::SFX_SPONGEBOB_WALK_2	},
};
static const int	s_runCount=sizeof(s_runSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_RUNSTART
// ANIM_PLAYER_ANIM_RUNSTOP
static const CPlayer::AnimFrameSfx s_runStopSfx[]=
{
	{	6,		CSoundMediator::SFX_SPONGEBOB_WALK_1	},
	{	18,		CSoundMediator::SFX_SPONGEBOB_WALK_2	},
};
static const int	s_runStopCount=sizeof(s_runStopSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_SOAKUP
// ANIM_PLAYER_ANIM_TALK01
// ANIM_PLAYER_ANIM_TALK02
// ANIM_PLAYER_ANIM_TALK03
// ANIM_PLAYER_ANIM_TALK04
// ANIM_PLAYER_ANIM_TEETERBACK
// ANIM_PLAYER_ANIM_TEETERFRONT






// This is the table that ties up anims to sfx
// CPlayer::setAnimFrame() uses this table to generate sfx based upon anim frames
const CPlayer::AnimSfx CPlayer::s_animSfx[]=
{
	{	s_buttBounceEndCount,	s_buttBounceEndSfx		},		// ANIM_PLAYER_ANIM_BUTTBOUNCEEND
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_BUTTBOUNCESTART
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_DEATHBACKWARDS
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_DEATHDRY
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_DEATHFORWARDS
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_DEATHSPIN
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_DEATHTAR
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_ELECTRICSHOCK
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_ELECTRICSHOCKEND
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_ELECTRICSHOCKSTART
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_FACEBACK
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_FACEFRONT
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_FALL
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_GETUP
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_GETUPRUN
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_HITGROUND
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_HOVER
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_HOVEREND
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_HOVERSTART
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_IDLEBREATHE
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_IDLEHOOLA
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_IDLELOOK
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_IDLEWIGGLEARM
	{	s_jumpEndCount,			s_jumpEndSfx			},		// ANIM_PLAYER_ANIM_JUMPEND
	{	s_chopCount,			s_chopSfx				},		// ANIM_PLAYER_ANIM_KARATE
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_KNOCKBACK
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_KNOCKFORWARD
	{	s_runCount,				s_runSfx				},		// ANIM_PLAYER_ANIM_RUN
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_RUNSTART
	{	s_runStopCount,			s_runStopSfx			},		// ANIM_PLAYER_ANIM_RUNSTOP
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_SOAKUP
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_TALK01
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_TALK02
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_TALK03
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_TALK04
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_TEETERBACK
	{	0,						NULL					},		// ANIM_PLAYER_ANIM_TEETERFRONT
};

 
/*===========================================================================
end */

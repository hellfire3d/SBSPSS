/*=========================================================================

	panimsfx.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	Handles automatically generated sound effects based upon anim frames
				NB: DON'T FORGET TO CHANGE s_numAnimSfx!!!!!!!

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

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
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


// ANIM_SPONGEBOB_BLOWBUBBLE
static const CPlayer::AnimFrameSfx s_blowBubbleSfx[]=
{
	{	4,	CSoundMediator::SFX_BUBBLE_WAND,				},
};
static const int	s_blowBubbleCount=sizeof(s_blowBubbleSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_BUTTBOUNCEEND
static const CPlayer::AnimFrameSfx s_buttBounceEndSfx[]=
{
	{	1,	CSoundMediator::SFX_SPONGEBOB_BUTTBOUNCE,		},
};
static const int	s_buttBounceEndCount=sizeof(s_buttBounceEndSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_SPONGEBOB_HITGROUND
static const CPlayer::AnimFrameSfx s_hitGroundSfx[]=
{
	{	8,	CSoundMediator::SFX_SPONGEBOB_LAND_AFTER_FALL,	},
	{	28,	CSoundMediator::SFX_SPONGEBOB_LAND_AFTER_FALL,	},
};
static const int	s_hitGroundCount=sizeof(s_hitGroundSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_IDLEWIND
static const CPlayer::AnimFrameSfx s_idleWindSfx[]=
{
	{	12,	CSoundMediator::SFX_HAZARD__SWAMP_GAS,			},
	{	28,	CSoundMediator::SFX_SPONGEBOB_WALK_2,			},
};
static const int	s_idleWindCount=sizeof(s_idleWindSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_JUMPEND
static const CPlayer::AnimFrameSfx s_jumpEndSfx[]=
{
	{	2,	CSoundMediator::SFX_SPONGEBOB_WALK_2			},
	{	3,	CSoundMediator::SFX_SPONGEBOB_WALK_1			},
};
static const int	s_jumpEndCount=sizeof(s_jumpEndSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_RUN
static const CPlayer::AnimFrameSfx s_runSfx[]=
{
	{	3,	CSoundMediator::SFX_SPONGEBOB_WALK_1			},
	{	11,	CSoundMediator::SFX_SPONGEBOB_WALK_2			},
};
static const int	s_runCount=sizeof(s_runSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_RUNSTOP
static const CPlayer::AnimFrameSfx s_runStopSfx[]=
{
	{	5,	CSoundMediator::SFX_SPONGEBOB_WALK_1			},
	{	6,	CSoundMediator::SFX_SPONGEBOB_WALK_2			},
};
static const int	s_runStopCount=sizeof(s_runStopSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_SPONGEBOB_WALK
static const CPlayer::AnimFrameSfx s_walkSfx[]=
{
	{	8,	CSoundMediator::SFX_SPONGEBOB_WALK_1			},
	{	15,	CSoundMediator::SFX_SPONGEBOB_WALK_2			},
};
static const int	s_walkCount=sizeof(s_walkSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_PLAYER_ANIM_KARATE
static const CPlayer::AnimFrameSfx s_chopSfx[]=
{
	{	12,	CSoundMediator::SFX_SPONGEBOB_KARATE_1,			},
};
static const int	s_chopCount=sizeof(s_chopSfx)/sizeof(CPlayer::AnimFrameSfx);

// ANIM_SPONGEBOB_SWIPE
static const CPlayer::AnimFrameSfx s_swipeSfx[]=
{
	{	8,	CSoundMediator::SFX_SPONGEBOB_NET,				},
};
static const int	s_swipeCount=sizeof(s_swipeSfx)/sizeof(CPlayer::AnimFrameSfx);



// This is the table that ties up anims to sfx
const CPlayer::AnimSfx CPlayer::s_animSfx[]=
{
	{	ANIM_SPONGEBOB_BLOWBUBBLE,		s_blowBubbleCount,		s_blowBubbleSfx			},
	{	ANIM_SPONGEBOB_BUTTBOUNCEEND,	s_buttBounceEndCount,	s_buttBounceEndSfx		},
	{	ANIM_SPONGEBOB_HITGROUND,		s_hitGroundCount,		s_hitGroundSfx			},
	{	ANIM_SPONGEBOB_IDLEWIND,		s_idleWindCount,		s_idleWindSfx			},
	{	ANIM_SPONGEBOB_JUMPEND,			s_jumpEndCount,			s_jumpEndSfx			},
	{	ANIM_SPONGEBOB_RUN,				s_runCount,				s_runSfx				},
	{	ANIM_SPONGEBOB_RUNSTOP,			s_runStopCount,			s_runStopSfx			},
	{	ANIM_SPONGEBOB_WALK,			s_walkCount,			s_walkSfx				},
	{	ANIM_SPONGEBOB_KARATE,			s_chopCount,			s_chopSfx				},
	{	ANIM_SPONGEBOB_SWIPE,			s_swipeCount,			s_swipeSfx				},
};
const int				CPlayer::s_numAnimSfx=10;		// Grrrrrrrr! (pkg)

 
/*===========================================================================
end */

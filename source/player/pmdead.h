/*=========================================================================

	pmdead.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMDEAD_H__
#define __PLAYER_PMDEAD_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayerModeDead : public CPlayerMode
{
public:
	virtual void	enter();
	virtual void	think();
//	virtual void	render(DVECTOR *_pos);

private:
	// Player can press ACTION to respawn after death
	enum
	{
		DEATH_DELAY=1*60,		// Can't force a respawn for this long
		DEATH_TIMEOUT=4*60,		// SB respawns automatically after this long

		BOUNCE_INITIALX=30,
		BOUNCE_MOVEX=2,
		BOUNCE_INITIALY=-20,
		BOUNCE_MAXY=50,
		BOUNCE_MOVEY=1,
		BOUNCE_VELOCITY_SHIFT=2,
	};

	enum
	{
		FALL_TO_DEATH,
		BOUNCE_OFF_SCREEN,
		STAY_STILL,
	};

	int				m_deadTime;
	int				m_deathAnim;
	int				m_deathMovementType;
	int				m_killed;
	DVECTOR			m_bounceVelocity;


};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMDEAD_H__ */

/*===========================================================================
 end */




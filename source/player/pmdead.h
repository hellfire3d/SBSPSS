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
		DEATH_DELAY=2*60,		// Can't force a respawn for this long
		DEATH_TIMEOUT=5*60,		// SB respawns automatically after this long
	};
	int				m_deadTime;


//	void			fall();

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




/*=========================================================================

	pmjelly.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMJELLY_H__
#define __PLAYER_PMJELLY_H__

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

class CPlayerModeJellyLauncher : public CPlayerModeBase
{
public:
	virtual void				enter();
	virtual void				think();
	virtual void				renderModeUi();
	virtual int					canConverse()						{return m_firingState==FIRING_STATE__NONE;}

	virtual void				setAnimNo(int _animNo);
	virtual void				setAnimFrame(int _animFrame);

private:
	enum
	{
		TIMEOUT_FOR_BIG_SHOT=60*4,
		AMMO_AMOUNT_FOR_BIG_SHOT=3,

		POWER_UP_SHOT_ANGLE=128,

		// Gun in the UI flashes when powered up..
		FLASHSPEED=25,
		FLASHSCALE=75,
	};

	typedef enum
	{
		FIRING_STATE__NONE,
		FIRING_STATE__POWERINGUP,
		FIRING_STATE__FIRING,
		FIRING_STATE__RECOILING,
	}FIRING_STATE;

	int							canFireFromThisState();

	void						launchProjectile();


	int							m_firingFrame;
	FIRING_STATE				m_firingState;
	int							m_firingTime;
	int							m_colourFlash;
	int							m_savedAnimNo,m_savedAnimFrame;

protected:
	virtual class CPlayerState	**getStateTable();

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMJELLY_H__ */

/*===========================================================================
 end */




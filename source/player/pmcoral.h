/*=========================================================================

	pmcoral.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMCORAL_H__
#define __PLAYER_PMCORAL_H__

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

class CPlayerModeCoralBlower : public CPlayerModeBase
{
public:
	virtual void				enter();
	virtual void				think();
	virtual void				renderModeUi();

	virtual const struct PlayerMetrics	*getPlayerMetrics();

protected:
	virtual class CPlayerState	**getStateTable();

private:
	typedef enum
	{
		BLOWER_STATE__EMPTY,
		BLOWER_STATE__SUCKING,
		BLOWER_STATE__FULL,
		BLOWER_STATE__AIMING,
	} BlowerState;

	DVECTOR						*getSuckUpPoint();

	BlowerState					m_blowerState;
	class CNpcEnemy				*m_enemy;	

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMCORAL_H__ */

/*===========================================================================
 end */




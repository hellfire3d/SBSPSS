/*=========================================================================

	pmbloon.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMBLOON_H__
#define __PLAYER_PMBLOON_H__

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

class CPlayerModeBalloon : public CPlayerModeBase
{
public:
	virtual void	initialise(class CPlayer *_player);
	virtual void	shutdown();
	virtual void	enter();
	virtual void	think();
	virtual void	render();

	virtual int							canFallForever()		{return true;}
	virtual const struct PlayerMetrics	*getPlayerMetrics();

private:
	enum
	{
		BALLOON_TIME=10*60,
		BALLOON_FLASH_TIME=2*60,
	};
	int					m_balloonTimer;

	class SpriteBank	*m_sprites;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMBLOON_H__ */

/*===========================================================================
 end */




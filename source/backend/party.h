/*=========================================================================

	party.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__BACKEND_PARTY_H__
#define __BACKEND_PARTY_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPartyScene: public CScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"Party";}


private:
	class SpriteBank		*m_sprites;
	int						m_readyToExit;
	unsigned char			*m_image;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CPartyScene	PartyScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __BACKEND_PARTY_H__ */

/*===========================================================================
 end */

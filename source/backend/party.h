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

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPartyScene
{
public:
	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	void	setAnimation(int _flag)				{m_animationFlag=_flag;}

private:
	class SpriteBank		*m_sprites;
	unsigned char			*m_image;
	int						m_animationFlag;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __BACKEND_PARTY_H__ */

/*===========================================================================
 end */

/*=========================================================================

	projectl.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PROJECTL_H__
#define	__PROJECTL_PROJECTL_H__

#include "Game/Thing.h"
#include "Gfx/Skel.h"

/*****************************************************************************/

class	CProjectile : public CThing
{
public:
	void				init();
	void				init( DVECTOR initPos, s16 initHeading );
	void				shutdown();
	void				think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );

protected:
	DVECTOR				getScreenOffset();

	class SpriteBank	*m_spriteBank;
	s16					m_heading;
	s32					m_lifetime;
};


/*****************************************************************************/
#endif

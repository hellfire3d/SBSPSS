/*=========================================================================

	prnpc.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PRNPC_H__
#define __PROJECTL_PRNPC_H__

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif

class CEnemyAsProjectile : public CPlayerProjectile
{
public:
	void			render();
	void			setGraphic( CActorGfx *graphic );
	virtual void	think(int _frames);
protected:
	CActorGfx		*m_actorGfx;
	POLY_FT4		*SprFrame;
	s16				m_rotation;
};

#endif
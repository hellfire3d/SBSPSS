/*=========================================================================

	prnpcspr.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PROJECTL_PRNPCSPR_H__
#define __PROJECTL_PRNPCSPR_H__

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif

class CEnemyAsSpriteProjectile : public CPlayerProjectile
{
public:
	static CEnemyAsSpriteProjectile		*Create();
	void			render();
	void			setGraphic( int frame );
	void			setRGB( int R, int G, int B );
	void			setHasRGB( u8 hasRGB )						{m_hasRGB = hasRGB;}
	virtual void	think(int _frames);
protected:
	s16						m_rotation;
	int						m_spriteFrame;
	int						m_R;
	int						m_G;
	int						m_B;
	u8						m_hasRGB;
};

#endif
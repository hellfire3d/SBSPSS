/*=========================================================================

	pquest.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PQUEST_H__
#define __PICKUPS_PQUEST_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__PICKUPS_PICKUP_H__
#include "pickups/pickup.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CQuestItemPickup : public CBasePickup
{
public:
	virtual void		init();

	virtual DVECTOR		getSizeForPlacement();
	virtual void		collect(class CPlayer *_player);


protected:
	virtual void		collidedWith(CThing *_thisThing);


private:
	virtual int			getVisibilityRadius();

	virtual void		thinkPickup(int _frames);
	virtual void		renderPickup(DVECTOR *_pos);

	virtual CSoundMediator::SFXID	sfxToPlayWhenCollected(){return CSoundMediator::SFX_ITEM__LEVEL_ITEM;}

	int					m_hiddenUntilBossDead;
	int					m_collected,m_collectedFrameCount;

	int					m_starSinRotate;
	int					m_starSinRadius;
	int					m_starFrame;
	int					m_starSinR,m_starSinG,m_starSinB;
	int					m_gfxFrame;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PQUEST_H__ */

/*===========================================================================
 end */

/*=========================================================================

	pkelp.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PKELP_H__
#define __PICKUPS_PKELP_H__

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

class CKelpTokenPickup : public CBasePickup
{
public:
	virtual void		init();

	virtual DVECTOR		getSizeForPlacement();
	virtual void		collect(class CPlayer *_player);

	void				setTokenNumber(int _number)					{m_tokenNumber=_number;}
	bool				alwaysThink();

protected:
	virtual void		collidedWith(CThing *_thisThing);
	virtual void		renderPickup(DVECTOR *_pos);

private:
	virtual CSoundMediator::SFXID	sfxToPlayWhenCollected(){return CSoundMediator::SFX_ITEM__ANY_OTHER_ITEM;}//SFX_ITEM__SKILL_ITEM;}

	int					m_tokenNumber;
	int					m_collectedFlag;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PKELP_H__ */

/*===========================================================================
 end */

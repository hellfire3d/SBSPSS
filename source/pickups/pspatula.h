/*=========================================================================

	pspatula.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PSPATULA_H__
#define __PICKUPS_PSPATULA_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__PICKUPS_PICKUP_H__
#include "pickups/pickup.h"
#endif

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CSpatulaPickup : public CBasePickup
{
public:
	virtual void		init();

	virtual DVECTOR		getSizeForPlacement();
	virtual void		collect(class CPlayer *_player);

	void				setSpatulaNumber(int _number)				{m_spatulaNumber=_number;}

protected:
	virtual void		thinkPickup(int _frames);
	virtual void		renderPickup(DVECTOR *_pos);

private:
//	virtual CSoundMediator::SFXID	sfxToPlayWhenCollected(){return CSoundMediator::SFX_ITEM__SKILL_ITEM;}

	int					m_glint;
	int					m_glintRot;
	int					m_spatulaNumber;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PSPATULA_H__ */

/*===========================================================================
 end */

/*=========================================================================

	tfemit.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TIFEMIT_H__
#define __TRIGGERS_TIFEMIT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __TRIGGER_TFEMIT__
#include "triggers\tfemit.h"
#endif

#include "fx\fx.h"

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CIntermittentFlameEmitterTrigger : public CFlameEmitterTrigger
{
public:
	virtual void		init();
	virtual void		think(int _frames);
protected:
	virtual void		collidedWith(CThing *_thisThing);
	s32					m_timer;
	u8					m_isActive;
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __TRIGGERS_TLEVEXIT_H__ */

/*===========================================================================
 end */

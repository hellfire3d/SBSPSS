/*=========================================================================

	tplatfrm.h

	Author:		Charles Blair
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TPLATFRM_H__
#define __TRIGGERS_TPLATFRM_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlatformTrigger : public CTrigger
{
public:
#if defined (__USER_art__) || defined (__USER_sbart__)
	virtual void		render();
#endif
	virtual void		setPlatform( CNpcPlatform *platform )			{m_platform = platform;}

protected:
	virtual void		collidedWith(CThing *_thisThing);

	CNpcPlatform		*m_platform;
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __TRIGGERS_TPLATFRM_H__ */

/*===========================================================================
 end */

/*=========================================================================

	tgarygo.h

	Author:		Charles Blair
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TGARYGO_H__
#define __TRIGGERS_TGARYGO_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __FRIEND_FGARY_H__
#include "friend\fgary.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGaryStartTrigger : public CTrigger
{
public:
	virtual void		setGary( CNpcGaryFriend *gary )			{m_gary = gary;}
	bool				alwaysThink()							{return(true);}

protected:
	virtual void		collidedWith(CThing *_thisThing);

	CNpcGaryFriend		*m_gary;
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif

/*===========================================================================
 end */

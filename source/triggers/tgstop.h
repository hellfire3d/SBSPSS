/*=========================================================================

	tgstop.h

	Author:		Charles Blair
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TGSTOP_H__
#define __TRIGGERS_TGSTOP_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGaryStopTrigger : public CTrigger
{
public:
	void				init();
	void				setPositionAndSize(int _x,int _y,int _w,int _h);
	bool				alwaysThink()							{return(true);}
protected:
	void				collidedWith(CThing *_thisThing);

	u8					m_dropped;
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

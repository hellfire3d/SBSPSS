/*=========================================================================

	pointer.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_POINTER_H__
#define	__GUI_POINTER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"		// Doh.. just for DVECTOR :/
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPointerIcon
{
public:
	void				think(int _frames);
	void				render();

	void				setTarget(DVECTOR _target);
	void				snapToTarget(DVECTOR _target);
	
	int					canPointerSelect()					{return m_pointerArrivedAtTarget;}

private:
	DVECTOR				m_pointerPos;
	DVECTOR				m_pointerTarget;
	int					m_pointerSin;
	int					m_pointerArrivedAtTarget;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_POINTER_H__ */

/*===========================================================================
 end */

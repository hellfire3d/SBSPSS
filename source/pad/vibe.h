/*=========================================================================

	vibe.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PAD_VIBE_H__
#define __PAD_VIBE_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPadVibrationManager
{
public:
	void		init();

private:
	enum
	{
		MAX_VIBES_PER_PAD=10,
	};

	typedef struct
	{
		u8		m_active;
		s16		*m_vibeData;
		u8		m_ferocity;
	} VIBE_CONTROL;


	VIBE_CONTROLS	m_vibeControls[2][MAX_VIBES_PER_PAD];
}


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PAD_VIBE_H__ */

/*===========================================================================
 end */

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
	typedef enum
	{
		VIBE_SHORT,
		VIBE_MEDIUM,
		VIBE_LONG,
		VIBE_LONG_STROBEY,
		VIBE_CORAL_BLOWER_SUCK,
		VIBE_CHOP,

		VIBE_MAX_TYPES
	} VIBE_TYPE;

	static void				init();
	static void				think(int _frames);
	static void				setVibration(int _port,VIBE_TYPE _type,u8 _ferocity=255);
	static u8				getCurrentVibrationIntensity(int _port)			{return s_currentIntensityValues[_port];}

	static void				stopAllVibration();

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


	static s16				*s_vibeData[VIBE_MAX_TYPES];
	static VIBE_CONTROL		s_vibeControls[2][MAX_VIBES_PER_PAD];
	static u8				s_currentIntensityValues[2];

};


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

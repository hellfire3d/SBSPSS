/*=========================================================================

	fdata.h

	Author:		PKG
	Created:
	Project:	PRLSR
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __GFX_FDATA_H__
#define __GFX_FDATA_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef		_GLOBAL_HEADER_
#include	"system\global.h"
#endif

#ifndef		__FILE_EQUATES_H__
#include	<biglump.h>
#endif

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class FontData
{
public:
			FontData( FileEquate _fontFileId, s16 *_fontTab, s16 _charHeight, s16 _charGapX, s16 _charGapY, s16 _spaceWidth );

	FileEquate		fontFileId;
	s16		*fontTab;
	s16		charHeight;
	s16		charGapX;
	s16		charGapY;
	s16		spaceWidth;
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

// Available fonts
extern FontData	standardFont;			// Crappy old PR font


/*	Data
	---- */

/*	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GFX_FDATA_H__ */

/*===========================================================================
 end */

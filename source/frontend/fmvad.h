/*=========================================================================

	fmvintro.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_FMVAD_H__
#define __FRONTEND_FMVAD_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CFrontEndFMVAdvert : public CFrontEndMode
{
public:
	void	select();

	int		isReadyToExit();
	CFrontEndScene::FrontEndMode	getNextMode();
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_FMVAD_H__ */

/*===========================================================================
 end */

/*=========================================================================

	fmvintro.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_FMVINTRO_H__
#define __FRONTEND_FMVINTRO_H__

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

class CFrontEndFMVIntro : public CFrontEndMode
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

#endif	/* __FRONTEND_FMVINTRO_H__ */

/*===========================================================================
 end */

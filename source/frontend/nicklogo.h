/*=========================================================================

	nicklogo.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_NICKLOGO_H__
#define __FRONTEND_NICKLOGO_H__

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

class CFrontEndNickLogo : public CFrontEndMode
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

#endif	/* __FRONTEND_NICKLOGO_H__ */

/*===========================================================================
 end */

/*=========================================================================

	fmvthq.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_FMVTHQ_H__
#define __FRONTEND_FMVTHQ_H__

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

class CFrontEndFMVTHQ : public CFrontEndMode
{
public:
	void	init();
	void	shutdown();
	void	select();
	void	unselect();
	void	render();
	void	think(int _frames);

	int		isReadyToExit();
	CFrontEndScene::FrontEndMode	getNextMode();


private:
	int					m_shuttingDown;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_FMVTHQ_H__ */

/*===========================================================================
 end */

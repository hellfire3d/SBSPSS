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
	virtual void							select();
	virtual void							unselect();
	virtual void							think(int _frames);

	virtual int								isReadyToExit();
	virtual CFrontEndScene::FrontEndMode	getNextMode();

private:
	enum
	{
#ifdef __USER_CDBUILD__
		DISPLAY_FRAMES=60*6,
#else
		DISPLAY_FRAMES=6,
#endif
	};

	int										m_readyToExit;
	unsigned char							*m_image;
	int										m_frameCount;

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

/*=========================================================================

	options.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_OPTIONS_H__
#define __FRONTEND_OPTIONS_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__FRONTEND_FRONTENTD_H__
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

class CFrontEndOptions : public CFrontEndMode
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
	enum
	{
		MODE__OPTIONS,
		MODE__CONTROL,
		MODE__SCREEN,
		MODE__SOUND,

		MODE__COUNT,
	};

	enum
	{
		X_BORDER=30,
		Y_BORDER=20,
	};

	class CScrollyBackground	*m_background;
	class CGUIControlFrame		*m_modeMenus[MODE__COUNT];
	int							m_mode,m_nextMode;
	
	int							m_exitFlag;
	int							m_closingDown;

	int							m_bgmVolume;
	int							m_sfxVolume;
	int							m_speechVolume;
	int							m_controlStyle;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_OPTIONS_H__ */

/*===========================================================================
 end */

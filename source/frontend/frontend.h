/*=========================================================================

	frontend.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_FRONTEND_H__
#define	__FRONTEND_FRONTEND_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CFrontEndScene : public CScene
{
public:
	typedef enum
	{
		MODE__MAIN_TITLES,
		MODE__GAME_OPTIONS,
		MODE__CHOOSE_SLOT,
		MODE__DEMO,
		MODE__CREDITS,

		MODE__NONE,
	} FrontEndMode;

	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"FrontEnd";}


private:
	void	setMode(FrontEndMode _newMode);

	FrontEndMode				m_mode;
	class CFrontEndMode			*m_modeCode;
	static class CFrontEndMode	*s_modeCodes[];

};


class CFrontEndMode
{
public:
	virtual void	init()=0;
	virtual void	shutdown()=0;
	virtual void	select()=0;
	virtual void	unselect()=0;
	virtual void	render()=0;
	virtual void	think(int _frames)=0;

	virtual int		isReadyToExit()=0;
	virtual CFrontEndScene::FrontEndMode	getNextMode()=0;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

extern CFrontEndScene	FrontEndScene;


/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_FRONTEND_H__ */

/*===========================================================================
 end */

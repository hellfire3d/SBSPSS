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

enum
{
	FE_OT__LOGO=2,
	FE_OT__SEAPOS=10,
};


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CFrontEndScene : public CScene
{
public:
	typedef enum
	{
		MODE__NICK_LOGO,
		MODE__FMV_ADVERT,
		MODE__FMV_THQ,
		MODE__FMV_INTRO,
		MODE__MAIN_TITLES,
		MODE__GAME_OPTIONS,
		MODE__CHOOSE_SLOT,
		MODE__DEMO,

		MODE__NONE,

		MODE__EXIT_TO_GAME,
	} FrontEndMode;

	enum
	{
		LOGO_CENTRE_Y=72,
		GAME_TITLE_TEXT_CENTRE_Y=110,
		GAME_TITLE_TEXT_R=128,
		GAME_TITLE_TEXT_G=128,
		GAME_TITLE_TEXT_B=128,
	};

	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	int		readyToShutdown();
	char	*getSceneName()			{return"FrontEnd";}
	
	static	void	setStartMode(FrontEndMode _mode)				{s_startMode=_mode;}

	static	void	renderLogo();

private:
	void	thinkLogo(int _frames);
	
	void	setMode(FrontEndMode _newMode);

	int							m_exitToGame;

	FrontEndMode				m_mode;
	class CFrontEndMode			*m_modeCode;
	static class CFrontEndMode	*s_modeCodes[];

	static FrontEndMode			s_startMode;
	static int					s_bootUp;

};


class CFrontEndMode
{
public:
	virtual void	init()				{;}
	virtual void	shutdown()			{;}
	virtual void	select()			{;}
	virtual void	unselect()			{;}
	virtual void	render()			{;}
	virtual void	think(int _frames)	{;}

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

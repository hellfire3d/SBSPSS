/*=========================================================================

	pause.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__GAME_PAUSE_H__
#define	__GAME_PAUSE_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPauseMenu
{
public:
	void			init();
	void			shutdown();

	void			select();
	void			unselect();
	void			think(int _frames);
	void			render();

	int				isActive();


private:
	typedef enum
	{
		STATE__MAIN_MENU,
		STATE__CONFIRM_QUIT,
	} STATE;

	enum
	{
		RESPONSE__WAITING,
		RESPONSE__CONTINUE,
		RESPONSE__QUIT,
		RESPONSE__CONFIRM_QUIT_YES,
		RESPONSE__CONFIRM_QUIT_NO,
	};


	void					renderLives();


	int						m_active;
	int						m_SpeechPlaying;
	int						m_padDebounce;
	int						m_responseFlag;
	int						m_vibrationChangeFlag;
	int						m_vibrationState;
	STATE					m_currentState;

	class CGUIControlFrame	*m_pauseGuiFrame;
	class CGUIControlFrame	*m_confirmQuitGuiFrame;
	class CGUIObject		*m_vibGUIOption;

	class FontBank			*m_fontBank;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GAME_PAUSE_H__ */

/*===========================================================================
 end */

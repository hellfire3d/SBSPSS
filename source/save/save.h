/*=========================================================================

	save.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__SAVE_SAVE_H__
#define __SAVE_SAVE_H__

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

class CSaveScene: public CScene
{
public:
	typedef enum
	{
		NEXTSCENE_MAP,
		NEXTSCENE_PARTY,
	}NEXTSCENE;


	void			init();
	void			shutdown();
	void			render();
	void			think(int _frames);
	int				readyToShutdown();
	char			*getSceneName()					{return"Save";}

	static void		setNextScene(NEXTSCENE _next)	{s_nextScene=_next;}


private:
	typedef enum
	{
		MODE__CONFIRMSAVE,
		MODE__CHECKING,
		MODE__UNFORMATTED,
		MODE__FORMATTING,
		MODE__CHECKINGFORMAT,
		MODE__FORMATOK,
		MODE__FORMATERROR,
		MODE__NOCARD,
		MODE__NOSPACE,
		MODE__CONFIRMOVERWRITE,
		MODE__SAVING,
		MODE__SAVEOK,
		MODE__SAVEERROR,
		MODE__READYTOEXIT,
	} MODE;
	enum
	{
		USERRESPONSE__NONE,
		USERRESPONSE__OK,
		USERRESPONSE__YES,
		USERRESPONSE__NO,
	};

	void	setMode(MODE _newMode);
	void	renderButtonPrompts();

	class CSaveLoadDatabase	*m_saveLoadDatabase;

	class CGUIControlFrame	*m_wantToSaveFrame;
	class CGUIGroupFrame	*m_blankFrame;
	class CGUITextBox		*m_blankTextBox;
	class CGUIControlFrame	*m_okResponseFrame;
	class CGUITextBox		*m_okResponseTextBox;
	class CGUIControlFrame	*m_yesNoResponseFrame;
	class CGUITextBox		*m_yesNoResponseTextBox;

	MODE					m_mode;
	int						m_timeInMode;
	int						m_userResponse;

	static NEXTSCENE		s_nextScene;

	class FontBank			*m_fontBank;
	class SpriteBank		*m_spriteBank;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CSaveScene	SaveScene;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __SAVE_SAVE_H__ */

/*===========================================================================
 end */

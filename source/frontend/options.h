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

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef __GUI_GREADOUT_H__
#include "gui\greadout.h"
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

		MODE__COUNT
	};

	enum
	{
		CONTROL_UP,
		CONTROL_DOWN,
		CONTROL_LEFT,
		CONTROL_RIGHT,
		CONTROL_JUMP,
		CONTROL_FIRE,
		CONTROL_CATCH,
		CONTROL_WEAPONCHANGE,

		CONTROL_COUNT
	};

	enum
	{
		ICON_UP,
		ICON_DOWN,
		ICON_LEFT,
		ICON_RIGHT,
		ICON_CROSS,
		ICON_CIRCLE,
		ICON_SQUARE,
		ICON_TRIANGLE,

		ICON_COUNT
	};

	enum
	{
		X_BORDER=30,
		Y_BORDER=10,
	};

	typedef struct
	{
		int	m_padButton;
		int	m_icon;
	} ButtonToIconMap;

	class CScrollyBackground	*m_background;
	class CGUIControlFrame		*m_modeMenus[MODE__COUNT];
	int							m_mode,m_nextMode;
	static int					s_modeBackground[MODE__COUNT];
	
	int							m_exitFlag;
	int							m_closingDown;

	int							m_bgmVolume;
	int							m_sfxVolume;
	int							m_speechVolume;
	int							m_controlStyle;
	int							m_screenXOff;
	int							m_screenYOff;

	static int									s_controlStyleValues[];
	static CGUITextReadout::TextReadoutData		s_controlStyleReadoutText[];

	static int									s_buttonOrder[];
	int											m_controlIcons[CONTROL_COUNT];
	static CGUISpriteReadout::SpriteReadoutData	s_controlReadoutSprites[ICON_COUNT];
	static ButtonToIconMap						s_controlMap[ICON_COUNT];

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

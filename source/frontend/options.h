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
	void	getValues();
	void	setLoadMode(int _newMode);
	void	renderButtonPrompts();


	enum
	{
		MODE__OPTIONS,
		MODE__CONTROL,
		MODE__SCREEN,
		MODE__SOUND,
		MODE__LOAD,

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
		LOADMODE__INIT,
		LOADMODE__CHECKING,
		LOADMODE__UNFORMATTED,
		LOADMODE__NODATA,
		LOADMODE__NOCARD,
		LOADMODE__CONFIRMLOAD,
		LOADMODE__LOADING,
		LOADMODE__LOADOK,
		LOADMODE__LOADERROR,

		LOADMODE__COUNT,
	};

	enum
	{
		USERRESPONSE__NONE,
		USERRESPONSE__OK,
		USERRESPONSE__YES,
		USERRESPONSE__NO,
	};

	enum
	{
		X_BORDER=30,
		Y_BORDER=10,
		OPTIONS_INSTRUCTIONS_Y_POS=185,
		OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS=5,
		OPTIONS_INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT=10,
		OPTIONS_INSTRUCTIONS_BUTTON_Y_OFFSET=4,
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

	int							m_loadMode;
	int							m_loadTimeInMode;
	class CSaveLoadDatabase		*m_saveLoadDatabase;
	static int					s_loadModeTextIds[LOADMODE__COUNT];
	class CGUIControlFrame		*m_loadModeConfirmFrame;
	class CGUIControlFrame		*m_loadModeOKFrame;
	int							m_loadUserResponse;
	
	int							m_exitFlag;
	int							m_closingDown;

	int							m_bgmVolume;
	int							m_sfxVolume;
	int							m_speechVolume;
	int							m_controlStyle;
	int							m_vibrationStatus,m_lastVibrationStatus;
	class CGUIObject			*m_vibGUIOption;
	int							m_screenXOff;
	int							m_screenYOff;

	int							m_musicStarted;

	static int									s_controlStyleValues[];
	static CGUITextReadout::TextReadoutData		s_controlStyleReadoutText[];

	static int									s_vibrationValues[];
	static CGUITextReadout::TextReadoutData		s_vibrationReadoutText[];

	static int									s_buttonOrder[];
	int											m_controlIcons[CONTROL_COUNT];
	static CGUISpriteReadout::SpriteReadoutData	s_controlReadoutSprites[ICON_COUNT];
	static ButtonToIconMap						s_controlMap[ICON_COUNT];

	class FontBank								*m_fontBank;
	class SpriteBank							*m_spriteBank;

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

/*=========================================================================

	bosstext.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__GAME_BOSSTEXT_H__
#define	__GAME_BOSSTEXT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CBossText
{
public:
	void			init();
	void			shutdown();

	void			select(int _dontChangeMusic=false);
	void			think(int _frames);
	void			render();
	int				isReadyToExit();


private:
	void			exit();

	typedef struct
	{
		u16							m_titleTextId;
		u16							m_subTitleTextId;
		u16							m_instructionsTextId;
		CSoundMediator::SONGID		m_songId;
	} BOSS_DATA;
	static const BOSS_DATA	s_bossData[];

	enum
	{
		INSTRUCTIONS_Y_POS=213,
		INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT=-15,		// Eh!? (pkg)
		INSTRUCTIONS_BUTTON_Y_OFFSET=4,
	};

	class FontBank			*m_fontBank;
	class ScalableFontBank	*m_scalableFontBank;

	int						m_readyToExit;
	int						m_currentPage;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GAME_BOSSTEXT_H__ */

/*===========================================================================
 end */

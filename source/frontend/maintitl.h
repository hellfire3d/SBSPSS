/*=========================================================================

	maintitl.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_MAINTITL_H__
#define __FRONTEND_MAINTITL_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

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

class CFrontEndMainTitles : public CFrontEndMode
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
		PRESS_START_TEXT_Y=180,
		PRESS_START_TEXT_R=150,
		PRESS_START_TEXT_G=100,
		PRESS_START_TEXT_B=100,

/*
		HORIZON_LEVEL=160,
		SEA_OVERLAP=50,						// Sea is drawn to this many pixels below screen bottom

		ISLAND_LEFT_X=40,					// Island position
		ISLAND_BOTTOM_Y=HORIZON_LEVEL+10,
*/
	};

	enum
	{
		MODE__PRESS_START,
		MODE__SELECT_OPTION,

		MODE__GOTO_CHOOSE_SLOT,
		MODE__GOTO_OPTIONS,
		MODE__GOTO_ADVERT,
		MODE__GOTO_DEMO,
	};

	enum
	{
		DEMO_TIMEOUT_IN_SECONDS=45,
	};

/*	
	POLY_FT4	*prepareSeaPortionFT4(struct sFrameHdr *_fh,int _x,int _y,int _w,int _h,int _rgb);
	void		renderSeaSection(struct sFrameHdr *fh,int _x,int _y,int _w,int _h);
*/

	void		renderPressStart();

	class SpriteBank		*m_sprites;
	class FontBank			*m_smallFont;

	class CGUIControlFrame	*m_mainMenu;

	int						m_mode;

	int						m_startGameFlag;
	int						m_gotoOptionsFlag;
#if defined(__TERRITORY_EUR__)
	int						m_gotoAdvertFlag;
#endif

	int						m_demoTimeout;
	int						m_musicStarted;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_MAINTITL_H__ */

/*===========================================================================
 end */

/*=========================================================================

	start.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_START_H__
#define __FRONTEND_START_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef	__GUI_GREADOUT_H__
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

class CFrontEndStart : public CFrontEndMode
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
	typedef enum
	{
		STATE_SELECT,
		STATE_CONFIRM_ERASE,
		STATE_EXITING_TO_FRONT_END,
		STATE_EXITING_TO_GAME,
		STATE_SLOT_CREATED,
	}STATE;

	void	drawGameSlot(int _xOff,int _slotNumber);
	void	drawInstructions();


	STATE					m_state;
	int						m_selectedSlot,m_lastSelectedSlot;
	int						m_slotDrawOffset;

	class CGUIGroupFrame	*m_emptyGuiFrame;
	class CGUIControlFrame	*m_confirmEraseGuiFrame;
	class CGUIControlFrame	*m_createdSlotGuiFrame;

	enum
	{
		CONFIRM_NONE=0,
		CONFIRM_OK=1,
		CONFIRM_YES=1,
		CONFIRM_NO,
	};
	int						m_confirmFlag;
	
	int						m_startGame;
							
	class SpriteBank		*m_spriteBank;
	class FontBank			*m_font;


};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_START_H__ */

/*===========================================================================
 end */

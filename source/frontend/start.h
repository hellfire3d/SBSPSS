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
	enum
	{
		SLOT_FRAME_X=(512-416)/2,
		SLOT_FRAME_Y=30,
		SLOT_FRAME_W=416,
		SLOT_FRAME_H=64,
		SLOT_LEVEL_TEXT_X=145,
		SLOT_LEVEL_TEXT_Y=5,
		SLOT_ITEM_X=16,
		SLOT_ITEM_Y=25,
		SLOT_ITEM_YGAP=50,
		SLOT_TOKENCOUNT_X=275,
		SLOT_TOKENCOUNT_Y=5,
		SLOT_SLOTNUMBER_X=15,
		SLOT_SLOTNUMBER_Y=5,
		SLOT_EMPTYTEXT_X=416/2,
		SLOT_EMPTYTEXT_Y=15,

		INSTRUCTIONS_YSTART=147,
		INSTRUCTIONS_BUTTON_Y_OFFSET=3,
		INSTRUCTIONS_GAP_BETWEEN_BUTTONS=5,
		INSTRUCTIONS_GAP_BETWEEN_BUTTONS_AND_TEXT=10,
		INSTRUCTIONS_Y_SPACE_BETWEEN_LINES=15,
	};


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

	static const int		s_itemFrames[];

	int						m_musicStarted;


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

/*=========================================================================

	pause.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "game\pause.h"

#ifndef	__GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef	__GUI_GFACTORY_H__
#include "gui\gfactory.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __STRING_ENUMS__
#include <trans.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static const int	FRAME_WIDTH		=352;
static const int	FRAME_HEIGHT	=160;
static const int	TEXT_BOX_WIDTH	=300;
static const int	TEXT_BOX_HEIGHT	=20;
static const int	OT_POS			=5;
static const int	TEXT_SPACING	=13;
extern int newmode;
void CPauseMenu::init()
{
	m_guiFrame=new ("Conversation GUI") CGUIControlFrame();
	m_guiFrame->init(0);
	m_guiFrame->setObjectXYWH((512-FRAME_WIDTH)/2,(256-FRAME_HEIGHT)/2,FRAME_WIDTH,FRAME_HEIGHT);
	m_guiFrame->setOt(OT_POS);
	m_guiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);

	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,TEXT_SPACING,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__CONTINUE,
										&m_exitPauseMenuFlag,true);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,TEXT_SPACING*2,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__QUIT,
										&m_quitGameFlag,true);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,TEXT_SPACING*4,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__FLY_MODE,
										&newmode,PLAYER_MODE_FLY);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,TEXT_SPACING*5,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BASICUNARMED_MODE,
										&newmode,PLAYER_MODE_BASICUNARMED);
/*
#ifdef __USER_paul__
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,TEXT_SPACING*6,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__FULLUNARMED_MODE,
										&newmode,PLAYER_MODE_FULLUNARMED);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,TEXT_SPACING*7,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__NET_MODE,
										&newmode,PLAYER_MODE_NET);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,TEXT_SPACING*8,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BALLOON_MODE,
										&newmode,PLAYER_MODE_BALLOON);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,TEXT_SPACING*9,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__CORALBLOWER_MODE,
										&newmode,PLAYER_MODE_CORALBLOWER);
#endif
*/

	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::shutdown()
{
	m_guiFrame->shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::select()
{
	m_active=true;
	m_exitPauseMenuFlag=false;
	m_quitGameFlag=false;
	m_guiFrame->select();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::unselect()
{
	m_active=false;
	m_guiFrame->unselect();
}
/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include "game\game.h"
#include "player\player.h"
void CPauseMenu::think(int _frames)
{
	if(m_active)
	{
		m_guiFrame->think(_frames);
		if(m_exitPauseMenuFlag||
		   m_quitGameFlag||
		   newmode!=-1)
		{
			if(m_quitGameFlag)
			{
				CGameScene::setReadyToExit();
			}
			unselect();
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPauseMenu::render()
{
	if(m_active)
	{
		m_guiFrame->render();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPauseMenu::isActive()
{
	return m_active;
}


/*===========================================================================
 end */

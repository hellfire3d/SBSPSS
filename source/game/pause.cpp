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


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif

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
#define FRAME_WIDTH		400
#define FRAME_HEIGHT	180
#define TEXT_BOX_WIDTH	300
#define TEXT_BOX_HEIGHT	20
#define OT_POS			5
int m_exitFlag;
extern int newmode;
void CPauseMenu::init()
{
	m_guiFrame=new ("Conversation GUI") CGUIControlFrame();
	m_guiFrame->init(0);
	m_guiFrame->setObjectXYWH((512-FRAME_WIDTH)/2,(256-FRAME_HEIGHT)/2,FRAME_WIDTH,FRAME_HEIGHT);
	m_guiFrame->setOt(OT_POS);
	m_guiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);

	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,10,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__PAUSE_MENU__CONTINUE,
										&m_exitFlag,true);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,40,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__BASICUNARMED_MODE,
										&newmode,0);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,60,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__FULLUNARMED_MODE,
										&newmode,1);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,80,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__SQUEAKYBOOTS_MODE,
										&newmode,2);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,100,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__NET_MODE,
										&newmode,3);
	CGUIFactory::createValueButtonFrame(m_guiFrame,
										(FRAME_WIDTH-TEXT_BOX_WIDTH)/2,120,TEXT_BOX_WIDTH,TEXT_BOX_HEIGHT,
										STR__DEBUG__CORALBLOWER_MODE,
										&newmode,4);

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
	m_exitFlag=false;
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
	m_exitFlag=false;
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
		if(m_exitFlag||newmode!=-1)
		{
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

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

#ifndef	__GUI_GSPRITE_H__
#include "gui\gsprite.h"
#endif

#ifndef	__GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
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
void CConversation::init()
{
/*
	s_guiFrame=new ("Conversation GUI") CGUIGroupFrame();
	s_guiFrame->init(0);
	s_guiFrame->setObjectXYWH((512-FRAME_WIDTH)/2,256-FRAME_BOTTOM_OFFSET-FRAME_HEIGHT,FRAME_WIDTH,FRAME_HEIGHT);
	s_guiFrame->setOt(OT_POS);
	s_guiFrame->setFlags(CGUIObject::FLAG_DRAWBORDER);

		s_guiIcon=new("Conversation Icon") CGUISprite();
		s_guiIcon->init(s_guiFrame);
		s_guiIcon->setObjectXYWH(0,0,FRAME_HEIGHT,FRAME_HEIGHT);
		s_guiIcon->setOt(OT_POS);
		s_guiIcon->setSpriteBank(UI_UIGFX_SPR);
		s_guiIcon->setFrame(0);

		s_guiText=new("Conversation Text") CGUITextBox();
		s_guiText->init(s_guiFrame);
		s_guiText->setObjectXYWH(FRAME_HEIGHT,TEXT_BORDER,FRAME_WIDTH-FRAME_HEIGHT-TEXT_BORDER,FRAME_HEIGHT-(TEXT_BORDER*2));
		s_guiText->setOt(OT_POS);
*/

	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::shutdown()
{
//	s_guiFrame->shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::select()
{
	m_active=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::think(int _frames)
{
	ASSERT(m_active);
//	if(isActive())
//	{
//		s_guiFrame->think(_frames);
//	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CConversation::render()
{
	ASSERT(m_active);
//	if(s_currentState==STATE_ACTIVE)
//	{
//		s_guiFrame->render();
//	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CConversation::isActive()
{
	return m_active;
}


/*===========================================================================
 end */

/*=========================================================================

	gtextbox.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\gtextbox.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif



/*	Std Lib
	------- */

/*	Data
	---- */

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
void CGUITextBox::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObjectWithFont::init(_parent,_id);
	m_textId=TranslationDatabase::NO_STRING;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextBox::setText(unsigned int _textId)
{
	m_textId=_textId;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextBox::render()
{
	if(!isHidden())
	{
		getFontBank()->print((getW()-(BORDERWIDTH*2))/2,m_textY,m_textId);
	}
	CGUIObjectWithFont::render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CGUITextBox::getTextWidth()
{
	return getFontBank()->getStringWidth((char*)TranslationDatabase::getString(getTextId()));
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CGUITextBox::getTextHeight()
{
	return getFontBank()->getStringHeight((char*)TranslationDatabase::getString(getTextId()));
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextBox::recalc()
{
	FontBank	*fb;
	char		*string;

	CGUIObjectWithFont::recalc();
	fb=getFontBank();
	string=(char*)TranslationDatabase::getString(m_textId);
		
#ifdef __VERSION_DEBUG__
	if(fb->getStringWidth(string)>getW()-(BORDERWIDTH*2)||
	   fb->getStringHeight(string)>getH()-(BORDERHEIGHT*2))
	{
		GUI_DBGMSG("Text overflow in CGUITextBox!");
	}
#endif
		
	m_textY=(getH()-(BORDERHEIGHT*2)-fb->getStringHeight(string))/2;
}


/*===========================================================================
 end */
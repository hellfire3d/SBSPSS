/*=========================================================================

	gui.cpp

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
void CGUITextBox::init(GUIId _id)
{
	CGUIObject::init(_id);
	m_textId=TranslationDatabase::NO_STRING;
	m_fontBank=new ("TextBox-Font") FontBank();
	m_fontBank->initialise(&standardFont);
	m_fontBank->setJustification(FontBank::JUST_CENTRE);
	m_fontBank->setOt(CGUIObject::DEFAULT_OT);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextBox::shutdown()
{
	CGUIObject::shutdown();
	m_fontBank->dump();
	m_fontBank=NULL;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextBox::setText(unsigned int _textId)
{
	FontBank	*fb;

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
	FontBank	*fb;

	if(!isHidden())
	{
		getFontBank()->print((getW()-(BORDERWIDTH*2))/2,m_textY,m_textId);
	}
	CGUIObject::render();
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

	CGUIObject::recalc();
	fb=getFontBank();
	fb->setPrintArea(getX()+BORDERWIDTH,getY()+BORDERHEIGHT,getW()-(BORDERWIDTH*2),getH()-(BORDERHEIGHT*2));
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
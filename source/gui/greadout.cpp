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

#include "gui\greadout.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
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
void CGUITextReadout::init(GUIId _id)
{
	CGUIObject::init(_id);
	m_target=0;
	m_data=0;
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
void CGUITextReadout::setReadoutTarget(int *_target)
{
	m_target=_target;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::setReadoutData(TextReadoutData *_data)
{
	m_data=_data;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::render()
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
void CGUITextReadout::think(int _frames)
{
	CGUIObject::think(_frames);
	if(*m_target!=m_lastValue)
	{
		recalc();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::recalc()
{
	CGUIObject::recalc();

	if(m_data)
	{
		FontBank		*fb;
		char			*string;
		TextReadoutData	*data;
		int				tmp;

		m_lastValue=*m_target;
		m_textId=4;

		fb=getFontBank();
		fb->setPrintArea(getX()+BORDERWIDTH,getY()+BORDERHEIGHT,getW()-(BORDERWIDTH*2),getH()-(BORDERHEIGHT*2));

		m_textId=STR__DUMMY;
		data=m_data;
		do
		{
			if(m_lastValue==data->m_value)
			{
				m_textId=data->m_textId;
				break;
			}
			tmp=data->m_value;
			data++;
		}
		while(tmp<data->m_value);
		string=(char*)TranslationDatabase::getString(m_textId);
		
#ifdef __VERSION_DEBUG__
		if(fb->getStringWidth(string)>getW()-(BORDERWIDTH*2)||
		   fb->getStringHeight(string)>getH()-(BORDERHEIGHT*2))
		{
			GUI_DBGMSG("Text overflow in CGUITextReadout!");
		}
#endif
		
		m_textY=(getH()-(BORDERHEIGHT*2)-fb->getStringHeight(string))/2;
	}
}


/*===========================================================================
 end */
/*=========================================================================

	greadout.cpp

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

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
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
void CGUITextReadout::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObjectWithFont::init(_parent,_id);
	m_target=0;
	m_data=0;
	m_lastValue=-1;
	m_textId=TranslationDatabase::NO_STRING;
	m_textY=0;
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
	CGUIObjectWithFont::render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextReadout::think(int _frames)
{
	CGUIObjectWithFont::think(_frames);
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
	CGUIObjectWithFont::recalc();

	if(m_data)
	{
		FontBank		*fb;
		char			*string;
		TextReadoutData	*data;
		int				tmp;

		m_lastValue=*m_target;
		fb=getFontBank();
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






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObject::init(_parent,_id);
	m_target=0;
	m_data=0;
	m_sprites=new ("SpriteReadout:sprites") SpriteBank();
	m_sprites->load(UI_UIGFX_SPR);
	m_lastValue=-1;
	m_frame=0;
	m_x=m_y=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::shutdown()
{
	CGUIObject::shutdown();
	m_sprites->dump();	delete m_sprites;	m_sprites=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::setReadoutTarget(int *_target)
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
void CGUISpriteReadout::setReadoutData(SpriteReadoutData *_data)
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
void CGUISpriteReadout::render()
{
	FontBank	*fb;
	
	if(!isHidden())
	{
		m_sprites->printFT4(m_frame,getParentX()+m_x,getParentY()+m_y,0,0,getOt());
	}
	CGUIObject::render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISpriteReadout::think(int _frames)
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
void CGUISpriteReadout::recalc()
{
	CGUIObject::recalc();

	if(m_data)
	{
		char				*string;
		SpriteReadoutData	*data;
		int					tmp;
		sFrameHdr			*fh;
		int					x,y;

		m_lastValue=*m_target;
		m_frame=0;
		data=m_data;
		do
		{
			if(m_lastValue==data->m_value)
			{
				m_frame=data->m_frame;
				break;
			}
			tmp=data->m_value;
			data++;
		}
		while(tmp<data->m_value);
		fh=m_sprites->getFrameHeader(m_frame);

#ifdef __VERSION_DEBUG__
		if(fh->W>getW()-(BORDERWIDTH*2)||
		   fh->H>getH()-(BORDERHEIGHT*2))
		{
			GUI_DBGMSG("Sprite overflow in CGUISpriteReadout!");
		}
#endif

		m_x=getX()+(getW()-fh->W)/2;
		m_y=getY()+(getH()-fh->H)/2;
	}
}


/*===========================================================================
 end */
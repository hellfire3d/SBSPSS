/*=========================================================================

	gsprite.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\gsprite.h"

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
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
void CGUISprite::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObjectWithSpriteBank::init(_parent,_id);
	m_frame=-1;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISprite::setFrame(int _frame)
{
	m_frame=_frame;
	recalc();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISprite::render()
{
	ASSERT(m_frame!=-1);

	if(!isHidden())
	{
		SpriteBank	*sb;
		sb=getSpriteBank();
		sb->printFT4(sb->getFrameHeader(m_frame),getParentX()+m_x,getParentY()+m_y,0,0,getOt());
	}
	CGUIObjectWithSpriteBank::render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISprite::recalc()
{
	if(m_frame!=-1)
	{
		sFrameHdr	*fh;

		fh=getSpriteBank()->getFrameHeader(m_frame);

#ifdef __VERSION_DEBUG__
		if(fh->W>getW()-(BORDERWIDTH*2)||
		   fh->H>getH()-(BORDERHEIGHT*2))
		{	
			GUI_DBGMSG("Sprite overflow in CGUISprite!");
		}
#endif
		m_x=getX()+((getW()-fh->W)/2);
		m_y=getY()+((getH()-fh->H)/2);
	}
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISelectSprite::render()
{
	if(isSelected())
	{
		CGUISprite::render();
	}
	else
	{
		CGUIObjectWithSpriteBank::render();
	}
}


/*===========================================================================
 end */
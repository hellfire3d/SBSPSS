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

#include "gui\gui.h"

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef	__GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
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

CGUIObject	*CGUIObject::s_llBase=NULL;



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::init(GUIId _id)
{
	ASSERT(this!=m_this);

	m_id=_id;
	m_x=m_y=m_w=m_h=0;
	m_flags=getInitialFlags();
	m_this=this;

	// Add to the end of the linked list of GUI objects..
	// Also check for duplicate IDs
	if(s_llBase)
	{
		CGUIObject *pGUI;
		pGUI=s_llBase;
		while(pGUI->m_llNext)
		{
			ASSERT(pGUI->m_id==_id);
			pGUI=pGUI->m_llNext;
		}
		pGUI->m_llNext=this;
	}
	else
	{
		s_llBase=this;
	}
	m_llNext=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::shutdown()
{
	CGUIObject *pGUI;

	ASSERT(this==m_this);

	m_this=0;

	// Remove from linked list of GUI objects..
	pGUI=s_llBase;
	while(pGUI->m_llNext!=this);
	{
		ASSERT(pGUI);	// Huh? Couldn't find this item in the list!?!?!
		pGUI=pGUI->m_llNext;
	}
	pGUI->m_llNext=this->m_llNext;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	NB: The chain of render functions needs to be reversed so
				that the draworder is correct! Look at any subclasses
				render() function to see what I mean by this.. (PKG)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::render()
{
	ASSERT(this==m_this);

	if(isHidden())
	{
		return;
	}

	if(getFlags(FLAG_DRAWBORDER))
	{
		POLY_G4	*g4;
		int	x,y,w,h;
		int	r,g,b;

		x=getX();
		y=getY();
		w=getW();
		h=getH();
		r=g=b=240;

		// Border
		DrawLine(x  ,y  ,x+w,y  ,r,g,b,DEFAULT_OT);
		DrawLine(x  ,y  ,x  ,y+h,r,g,b,DEFAULT_OT);
		DrawLine(x+w,y  ,x+w,y+h,r,g,b,DEFAULT_OT);
		DrawLine(x  ,y+h,x+w,y+h,r,g,b,DEFAULT_OT);

		x+=3;y+=2;w-=6;h-=4;
		DrawLine(x  ,y  ,x+w,y  ,r,g,b,DEFAULT_OT);
		DrawLine(x  ,y  ,x  ,y+h,r,g,b,DEFAULT_OT);
		DrawLine(x+w,y  ,x+w,y+h,r,g,b,DEFAULT_OT);
		DrawLine(x  ,y+h,x+w,y+h,r,g,b,DEFAULT_OT);
		x-=3;y-=2;w+=6;h+=4;

		// Background
		g4=GetPrimG4();
		setXYWH(g4,x,y,w,h);
		setSemiTrans(g4,true);
		setRGB0(g4,250, 15,125);
		setRGB1(g4,250,125, 15);
		setRGB2(g4, 15,250,125);
		setRGB3(g4, 15,125,250);
		AddPrimToList(g4,DEFAULT_OT);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::think(int _frames)
{
	ASSERT(this==m_this);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::recalc()
{
	ASSERT(this==m_this);
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern void guiOpen()
{
	ASSERT(!CGUIObject::s_llBase);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern void guiClose()
{
	// Dump everything automatically
	// Don't walk the linked list here but keep removing the head since frame
	// objects remove their own children (PKG or do they?)
	while(CGUIObject::s_llBase)
	{
		CGUIObject::s_llBase->shutdown();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern void	guiThink(int _frames)
{
	CGUIObject *pGUI;
	pGUI=CGUIObject::s_llBase;
	while(pGUI)
	{
		pGUI->think(_frames);
		pGUI=pGUI->m_llNext;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern void	guiRender()
{
	CGUIObject *pGUI;
	pGUI=CGUIObject::s_llBase;
	while(pGUI)
	{
		pGUI->render();
		pGUI=pGUI->m_llNext;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern CGUIObject *guiGetItem(CGUIObject::GUIId _searchId)
{
	CGUIObject *pGUI;
	pGUI=CGUIObject::s_llBase;
	while(pGUI)
	{
		if(pGUI->m_id==_searchId)
		{
			return pGUI;
		}
		pGUI=pGUI->m_llNext;
	}

	// Bwah!
	ASSERT(0);
	return NULL;
}


/*===========================================================================
 end */
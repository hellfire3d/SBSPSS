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

#include "gui\gframe.h"

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
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
void CGUIGroupFrame::setFlags(GUI_FLAGS _flags)
{
//	int	childFlags;

//	childFlags=_flags&(-1^FLAG_SELECTED);	// Don't propogate the SELECTED flag

	CGUIObject::setFlags(_flags);
	if(getChild())getChild()->setFlags(_flags);
//	if(getNext())getNext()->setFlags(_flags);

//	if(_flags&FLAG_SELECTED)
//	{
//		selectFrame();
//	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIGroupFrame::clearFlags(GUI_FLAGS _flags)
{
//	int	childFlags;
	
//	childFlags=_flags&(-1^FLAG_SELECTED);	// Don't propogate the SELECTED flag
	
	CGUIObject::clearFlags(_flags);
	if(getChild())getChild()->clearFlags(_flags);
//	if(getNext())getNext()->clearFlags(_flags);
	
//	if(_flags&FLAG_SELECTED)
//	{
//		unselectFrame();
//	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
/*
void CGUIGroupFrame::selectFrame()
{
	CGUIObject	*pGUI;
	
	pGUI=getChild();
	if(pGUI)
	{
		pGUI->select();
		pGUI=pGUI->getNext();
	}
}
*/


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
/*
void CGUIGroupFrame::unselectFrame()
{
	CGUIObject	*pGUI;
	
	pGUI=getChild();
	if(pGUI)
	{
		pGUI->unselect();
		pGUI=pGUI->getNext();
	}
}
*/







/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIControlFrame::think(int _frames)
{
	int	pad;

	CGUIObject::think(_frames);

	pad=PadGetRepeat(0);
	if(pad&PAD_DOWN)
	{
		CGUIObject	*pGUI;
		
		pGUI=getChild();
		if(pGUI)
		{
			// Get current selection and deselect it
			while(!pGUI->isSelected())
			{
				ASSERT(pGUI);
				pGUI=pGUI->getNext();
			}
			pGUI->unselect();
GUI_DBGMSG("unselected %d",pGUI->getId());

			// Find next object and select it
			pGUI=pGUI->getNext();
			if(!pGUI)pGUI=getChild();
			pGUI->select();
GUI_DBGMSG("selected %d",pGUI->getId());
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIControlFrame::setFlags(GUI_FLAGS _flags)
{
	GUI_FLAGS	childFlags;

	CGUIObject::setFlags(_flags);
	childFlags=(GUI_FLAGS)(_flags&(-1^FLAG_SELECTED));	// Don't propogate the SELECTED flag
	if(getChild())getChild()->setFlags(childFlags);
//	if(getNext())getNext()->setFlags(_flags);

	if(_flags&FLAG_SELECTED)
	{
		selectFrame();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIControlFrame::clearFlags(GUI_FLAGS _flags)
{
	GUI_FLAGS	childFlags;

	CGUIObject::clearFlags(_flags);
	childFlags=(GUI_FLAGS)(_flags&(-1^FLAG_SELECTED));	// Don't propogate the SELECTED flag
	if(getChild())getChild()->clearFlags(childFlags);
//	if(getNext())getNext()->clearFlags(_flags);
	
	if(_flags&FLAG_SELECTED)
	{
		unselectFrame();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIControlFrame::selectFrame()
{
	CGUIObject	*pGUI;

	pGUI=getChild();
	if(pGUI)
	{
		pGUI->select();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIControlFrame::unselectFrame()
{
	CGUIObject	*pGUI;
	
	pGUI=getChild();
	if(pGUI)
	{
		while(!pGUI->isSelected())
		{
			ASSERT(pGUI);
			pGUI=pGUI->getNext();
		}
		pGUI->unselect();
	}
}


/*===========================================================================
 end */
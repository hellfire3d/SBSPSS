/*=========================================================================

	gbutton.cpp

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

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
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
	CGUIObject	*pGUI;
	
	CGUIObject::setFlags(_flags);
	pGUI=getChild();
	while(pGUI)
	{
		pGUI->setFlags(_flags);
		pGUI=pGUI->getNext();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIGroupFrame::clearFlags(GUI_FLAGS _flags)
{
	CGUIObject	*pGUI;

	CGUIObject::clearFlags(_flags);
	pGUI=getChild();
	while(pGUI)
	{
		pGUI->clearFlags(_flags);
		pGUI=pGUI->getNext();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CGUIGroupFrame::isSelectable()
{
	CGUIObject	*pGUI;
	int			selectable=false;
	
	if(!isHidden())
	{
		pGUI=getChild();
		while(pGUI)
		{
			if(pGUI->isSelectable())
			{
				selectable=true;
				break;
			}
			pGUI=pGUI->getNext();
		}
	}

	return selectable;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CGUIGroupFrame::isUnselectable()
{
	CGUIObject	*pGUI;
	int			unselectable=false;
	
	pGUI=getChild();
	while(pGUI)
	{
		if(pGUI->isUnselectable())
		{
			unselectable=true;
			break;
		}
		pGUI=pGUI->getNext();
	}

	return unselectable;
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIControlFrame::think(int _frames)
{
	int	pad;
	int	makeNoise=false;

	CGUIObject::think(_frames);

	pad=PadGetRepeat(0);
	if(pad&PAD_DOWN)
	{
		selectNextItem();
		makeNoise=true;
	}
	else if(pad&PAD_UP)
	{
		selectPreviousItem();
		makeNoise=true;
	}

	if(makeNoise)
	{
		CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__MOVE_CURSOR);
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
void CGUIControlFrame::selectPreviousItem()
{
	CGUIObject	*pGUI,*prevGUI;
	
	pGUI=getChild();
	prevGUI=0;
	if(pGUI)
	{
		// Get current selection and deselect it
		while(!pGUI->isSelected())
		{
			ASSERT(pGUI);
			if(pGUI->isSelectable())
			{
				prevGUI=pGUI;
			}
			pGUI=pGUI->getNext();
		}
		if(pGUI->isUnselectable())
		{
			pGUI->unselect();

			// Find previous selectable object and select it
			if(!prevGUI)
			{
				do
				{
					if(pGUI->isSelectable())
					{
						prevGUI=pGUI;
					}
					pGUI=pGUI->getNext();
				}
				while(pGUI);
			}
			prevGUI->select();
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIControlFrame::selectNextItem()
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
		if(pGUI->isUnselectable())
		{
			pGUI->unselect();

			// Find next selectable object and select it
			do
			{
				pGUI=pGUI->getNext();
				if(!pGUI)pGUI=getChild();
			}
			while(!pGUI->isSelectable());
			pGUI->select();
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CGUIObject::GUIId CGUIControlFrame::getIdOfSelectedItem()
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
		return pGUI->getId();
	}

	return CGUIObject::noId;
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
	while(!pGUI->isSelectable())
	{
		ASSERT(pGUI);
		pGUI=pGUI->getNext();
	}
	pGUI->select();
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
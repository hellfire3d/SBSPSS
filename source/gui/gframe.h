/*=========================================================================

	gframe.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GFRAME_H__
#define	__GUI_GFRAME_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GUI_GUI_H__
#include "gui\gui.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGUIGroupFrame : public CGUIObject
{
public:
	virtual void		setFlags(GUI_FLAGS _flags);
	virtual void		clearFlags(GUI_FLAGS _flags);

	virtual int			isSelectable();
	virtual int			isUnselectable();

	
protected:
	GUI_FLAGS			getInitialFlags()							{return FLAG_NONE;}

};


class CGUIControlFrame : public CGUIObject
{
public:
	virtual void		think(int _frames);

	virtual void		setFlags(GUI_FLAGS _flags);
	virtual void		clearFlags(GUI_FLAGS _flags);
	
	void				selectPreviousItem();
	void				selectNextItem();

	GUIId				getIdOfSelectedItem();
	
protected:
	GUI_FLAGS			getInitialFlags()							{return FLAG_DRAWBORDER;}
	
	void				selectFrame();
	void				unselectFrame();
	
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GFRAME_H__ */

/*===========================================================================
 end */

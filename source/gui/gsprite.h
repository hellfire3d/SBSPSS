/*=========================================================================

	gsprite.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GSPRITE_H__
#define	__GUI_GSPRITE_H__

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

// Sprite
class CGUISprite : public CGUIObjectWithSpriteBank
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	virtual void		setFrame(int _frame);

	virtual void		render();


protected:
	void				recalc();


private:
	int					m_frame;
	int					m_x,m_y;

};


// Sprite that only appears when the object is selected
class CGUISelectSprite : public CGUISprite
{
public:
	virtual void		render();
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GSPRITE_H__ */

/*===========================================================================
 end */

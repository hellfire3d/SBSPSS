/*=========================================================================

	gbutton.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GBUTTON_H__
#define	__GUI_GBUTTON_H__

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

class CGUIToggleButton : public CGUIObject
{
public:
	virtual void		setButtonTarget(int *_target);
	
	virtual void		think(int _frames);
	
	
protected:
	int					*getTarget()								{return m_target;}
	virtual int			isSelectable()								{return true;}
	
	
private:
	int					*m_target;
	
};


class CGUICycleButton : public CGUIToggleButton
{
public:
	virtual void		setButtonData(int *_data);

	virtual void		think(int _frames);


protected:
	int					*getData()									{return m_data;}


private:
	int					*m_data;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GBUTTON_H__ */

/*===========================================================================
 end */

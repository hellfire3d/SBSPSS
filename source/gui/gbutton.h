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

// Toggles a target int between true/false
class CGUIToggleButton : public CGUIObject
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	void				setButtonTarget(int *_target);
	
	virtual void		think(int _frames);
	
	
protected:
	int					*getTarget()								{return m_target;}
	virtual int			isSelectable()								{return true;}
	
	
private:
	int					*m_target;
	
};

// Sets target to a specified value
class CGUIValueButton : public CGUIToggleButton
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	void				setButtonValue(int _value);

	virtual void		think(int _frames);


private:
	int					m_value;

};

// Cycles target between a specified range of values
class CGUICycleButton : public CGUIToggleButton
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	void				setButtonData(int *_data);

	virtual void		think(int _frames);


protected:
	int					*getData()									{return m_data;}


private:
	int					*m_data;

};

// Scrolls target between set limits
class CGUISliderButton : public CGUIToggleButton
{
public:
	enum
	{
		DEFAULT_SCROLL_SPEED=10,
	};

	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	void				setButtonRange(int _min,int _max);
	void				setScrollSpeed(int _scrollSpeed);

	virtual void		think(int _frames);


private:
	int					m_min,m_max;
	int					m_scrollSpeed;

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

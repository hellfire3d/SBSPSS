/*=========================================================================

	gui.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GUI_H__
#define	__GUI_GUI_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGUIObject
{
public:
	typedef int			GUIId;

	typedef enum
	{
		FLAG_SELECTED=		1<<0,
		FLAG_HIDDEN=		1<<1,
		FLAG_DRAWBORDER=	1<<2,

		INITIAL_FLAGS=		FLAG_DRAWBORDER,
	} GUI_FLAGS;

	enum
	{
		DEFAULT_OT=20,
		BORDERWIDTH=8,
		BORDERHEIGHT=5,
	};

	
	virtual void		init(GUIId _id);
	virtual void		shutdown();

	void				setObjectX(int _x)							{m_x=_x;recalc();}
	void				setObjectY(int _y)							{m_y=_y;recalc();}
	void				setObjectW(int _w)							{m_w=_w;recalc();}
	void				setObjectH(int _h)							{m_h=_h;recalc();}
	void				setObjectXY(int _x,int _y)					{setObjectX(_x);setObjectY(_y);}
	void				setObjectWH(int _w,int _h)					{setObjectW(_w);setObjectH(_h);}
	void				setObjectXYWH(int _x,int _y,int _w,int _h)	{setObjectXY(_x,_y);setObjectWH(_w,_h);}

	virtual void		render();
	virtual void		think(int _frames);
	virtual void		recalc();

	void				setFlags(GUI_FLAGS _flags)					{m_flags|=_flags;}
	void				clearFlags(GUI_FLAGS _flags)				{m_flags&=_flags^-1;}
	int					getFlags(GUI_FLAGS _flags)					{return (m_flags&_flags)!=0;}

	// Quick access functions to the most frequently used flags
	void				select()									{setFlags(FLAG_SELECTED);}
	void				unselect()									{clearFlags(FLAG_SELECTED);}
	int					isSelected()								{return getFlags(FLAG_SELECTED);}
	void				hide()										{setFlags(FLAG_HIDDEN);}
	void				unhide()									{clearFlags(FLAG_HIDDEN);}
	int					isHidden()									{return getFlags(FLAG_HIDDEN);}

	
protected:
	int					getX()										{return m_x;}
	int					getY()										{return m_y;}
	int					getW()										{return m_w;}
	int					getH()										{return m_h;}
	

private:
	GUIId				m_id;
	int					m_x,m_y,m_w,m_h;
	int					m_flags;

	CGUIObject			*m_this;
	CGUIObject			*m_llNext;
	static CGUIObject	*s_llBase;


	// ..don't like using friends for this :(
	friend void			guiOpen();
	friend void			guiClose();
	friend void			guiThink(int _frames);
	friend void			guiRender();
	friend CGUIObject	*guiGetItem(CGUIObject::GUIId _searchId);

};





/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

extern void			guiOpen();
extern void			guiClose();
extern void			guiThink(int _frames);
extern void			guiRender();
extern CGUIObject	*guiGetItem(CGUIObject::GUIId _searchId);

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GUI_H__ */

/*===========================================================================
 end */

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
		FLAG_NONE=			0,
		FLAG_SELECTED=		1<<0,
		FLAG_HIDDEN=		1<<2,
		FLAG_DRAWBORDER=	1<<3,
	} GUI_FLAGS;

	enum
	{
		INITIAL_OT=200,
		BORDERWIDTH=8,
		BORDERHEIGHT=5,
	};


	virtual void		init(CGUIObject *_parent,GUIId _id);
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

	virtual void		setFlags(GUI_FLAGS _flags)					{m_flags|=_flags;}
	virtual void		clearFlags(GUI_FLAGS _flags)				{m_flags&=_flags^-1;}
	int					getFlags(GUI_FLAGS _flags)					{return (m_flags&_flags)!=0;}

	// Quick access functions to the most frequently used flags
	void				select()									{setFlags(FLAG_SELECTED);}
	void				unselect()									{clearFlags(FLAG_SELECTED);}
	int					isSelected()								{return getFlags(FLAG_SELECTED);}
	void				hide()										{setFlags(FLAG_HIDDEN);}
	void				unhide()									{clearFlags(FLAG_HIDDEN);}
	int					isHidden()									{return getFlags(FLAG_HIDDEN);}

	CGUIObject			*getChild()									{return m_child;}
	CGUIObject			*getNext()									{return m_next;}
	virtual int			isSelectable()								{return false;}

int getId() {return m_id;}

protected:
	virtual void		recalc();

	virtual GUI_FLAGS	getInitialFlags()							{return FLAG_NONE;}

	int					getX()										{return m_x;}
	int					getY()										{return m_y;}
	int					getW()										{return m_w;}
	int					getH()										{return m_h;}

	int					getParentX()								{if(m_parent)return m_parent->getX()+m_parent->getParentX();else return 0;}
	int					getParentY()								{if(m_parent)return m_parent->getY()+m_parent->getParentY();else return 0;}

	void				setOt(int _ot)								{m_ot=_ot;}
	int					getOt()										{return m_ot;}


private:
	GUIId				m_id;
	int					m_x,m_y,m_w,m_h;
	int					m_flags;
	int					m_ot;

	CGUIObject			*m_this;									// Used to check that that the object has been initialised
	CGUIObject			*m_parent;									// Parent object
	CGUIObject			*m_child;									// First child
	CGUIObject			*m_next;									// Next item at this level
};


class CGUIObjectWithFont : public CGUIObject
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id);
	virtual void		shutdown();

	virtual void		setFlags(GUI_FLAGS _flags);
	virtual void		clearFlags(GUI_FLAGS _flags);
	
	
protected:
	enum
	{
		DEFAULT_FONT_R=150,
		DEFAULT_FONT_G=100,
		DEFAULT_FONT_B=100,
		SELECTED_FONT_R=175,
		SELECTED_FONT_G=225,
		SELECTED_FONT_B=175,
	};

	virtual void		recalc();

	class FontBank		*getFontBank()							{return m_fontBank;}


private:
	class FontBank		*m_fontBank;


};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GUI_H__ */

/*===========================================================================
 end */

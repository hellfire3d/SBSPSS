/*=========================================================================

	greadout.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GREADOUT_H__
#define	__GUI_GREADOUT_H__

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

class CGUITextReadout : public CGUIObjectWithFont
{
public:
	typedef struct
	{
		int		m_value;
		int		m_textId;
	} TextReadoutData;

	
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	virtual void		setReadoutTarget(int *_target);
	virtual void		setReadoutData(TextReadoutData *_data);

	virtual void		render();
	virtual void		think(int _frames);


protected:
	void				recalc();

	
private:
	int					*m_target;
	TextReadoutData		*m_data;
	int					m_lastValue;
	unsigned int		m_textId;
	int					m_textY;

};


class CGUISpriteReadout : public CGUIObjectWithSpriteBank
{
public:
	typedef struct
	{
		int		m_value;
		int		m_frame;
	} SpriteReadoutData;

	
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	virtual void		setReadoutTarget(int *_target);
	virtual void		setReadoutData(SpriteReadoutData *_data);

	virtual void		render();
	virtual void		think(int _frames);


protected:
	void				recalc();

	
private:
	int					*m_target;
	SpriteReadoutData	*m_data;
	int					m_lastValue;
	int					m_frame;
	int					m_x,m_y;

};


class CGUIBarReadout : public CGUIObject
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	virtual void		setReadoutTarget(int *_target);
	virtual void		setReadoutRange(int _min,int _max);

	virtual void		render();
	virtual void		think(int _frames);


protected:
	void				recalc();


private:
	int					*m_target;
	int					m_min,m_max;
	int					m_markerOffset;
	int					m_lastValue;

};





/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GREADOUT_H__ */

/*===========================================================================
 end */

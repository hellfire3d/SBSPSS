/*=========================================================================

	gtextbox.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GTEXTBOX_H__
#define	__GUI_GTEXTBOX_H__

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

class CGUITextBox : public CGUIObjectWithFont
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id);

	virtual void		setText(unsigned int _textId);

	virtual void		render();


protected:
	void				recalc();


private:
	unsigned int		m_textId;
	int					m_textY;

};





/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GTEXTBOX_H__ */

/*===========================================================================
 end */

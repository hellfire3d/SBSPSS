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

class CGUITextBox : public CGUIObject
{
public:
	virtual void		init(GUIId _id);
	virtual void		shutdown();

	virtual void		setText(unsigned int _textId);

	virtual void		render();


protected:
	void				recalc();
	class FontBank		*getFontBank()							{return m_fontBank;}


private:
	class FontBank		*m_fontBank;
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

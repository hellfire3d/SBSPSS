NB - This is not used.. but left IN VSS incase it needs to be resurrected (PKG)


/*=========================================================================

	gtextent.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GTEXTENT_H__
#define	__GUI_GTEXTENT_H__

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

class CGUITextEntry : public CGUIObjectWithFont
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);
	virtual void		shutdown();

	virtual void		setTextDetails(char *_store,int _maxLength);

	virtual void		render();
	virtual void		think(int _frames);

	virtual void		setFlags(GUI_FLAGS _flags);
	
	virtual int			isSelectable()								{return true;}
	virtual int			isUnselectable();


private:
	class CGUIKeyboard	*m_keyboard;

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

/*=========================================================================

	gbubbles.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__GUI_GBUBBLES_H__
#define	__GUI_GBUBBLES_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GUI_GUI_H__
#include "gui\gui.h"
#endif

#ifndef	__GFX_BUBICLES_H__
#include "gfx\bubicles.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGUIBubbles : public CGUIObject
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);

	virtual void		setFlags(GUI_FLAGS _flags);
	virtual void		clearFlags(GUI_FLAGS _flags);

	virtual int			isSelectable()								{return true;}

protected:

private:
	void				startEmitter();
	void				stopEmitter();
	
	class CBubicleEmitter	*m_emitter;

	static BubicleEmitterData	s_emitterData;

};





/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GUI_GBUBBLES_H__ */

/*===========================================================================
 end */

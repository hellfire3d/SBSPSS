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

class CGUITextReadout : public CGUIObject
{
public:
	typedef struct
	{
		int		m_value;
		int		m_textId;
	} TextReadoutData;

	
	virtual void		init(GUIId _id);

	virtual void		setReadoutTarget(int *_target);
	virtual void		setReadoutData(TextReadoutData *_data);


	virtual void		render();
	virtual void		think(int _frames);


protected:
	void				recalc();
	class FontBank		*getFontBank()							{return m_fontBank;}

	
private:
	int					*m_target;
	TextReadoutData		*m_data;
	int					m_lastValue;
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

#endif	/* __GUI_GREADOUT_H__ */

/*===========================================================================
 end */

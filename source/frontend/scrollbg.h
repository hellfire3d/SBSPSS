/*=========================================================================

	scrollbg.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:	Scrolling tiled background class

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__FRONTEND_SCROLLBG_H__
#define	__FRONTEND_SCROLLBG_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CScrollyBackground
{
public:
	typedef enum
	{
		DRAWMODE_NORMAL=0,
		DRAWMODE_ADDITIVE=1,
		DRAWMODE_SUBTRACTIVE=2,
	}DRAWMODE;

	enum
	{
		NORMAL_SCALE=256,
	};

	void				init();
	void				shutdown();
	void				render();
	void				think(int _frames);

	void				setSpeed(int _xSpeed,int _ySpeed)			{m_xSpeed=_xSpeed;m_ySpeed=_ySpeed;}
	void				setSpeedScale(int _speedScale)				{m_speedScale=_speedScale;}
	void				setOt(int _ot)								{m_ot=_ot;}
	void				setFrame(int _frame)						{m_frame=_frame;}
	void				setTheDrawMode(DRAWMODE _drawMode)			{m_drawMode=_drawMode;}
	void				setColour(u8 _r,u8 _g,u8 _b)				{m_r=_r;m_g=_g;m_b=_b;}
	void				setScale(int _scale)						{m_scale=_scale;}

private:
	enum
	{
		DEFAULT_X_SPEED=-2,
		DEFAULT_Y_SPEED=-3,
		DEFAULT_SPEED_SCALE=2,
		DEFAULT_OT=MAX_OT-1,
	};

	class SpriteBank	*m_sprites;
	int					m_xOff,m_yOff;
	int					m_xSpeed,m_ySpeed;
	int					m_speedScale;
	int					m_ot;
	int					m_frame;
	DRAWMODE			m_drawMode;
	u8					m_r,m_g,m_b;
	int					m_scale;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __FRONTEND_SCROLLBG_H__ */

/*===========================================================================
 end */

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
	void				init();
	void				shutdown();
	void				render();
	void				think(int _frames);

	void				setSpeed(int _xSpeed,int _ySpeed)			{m_xSpeed=_xSpeed;m_ySpeed=_ySpeed;}
	void				setSpeedScale(int _speedScale)				{m_speedScale=_speedScale;}
	void				setOt(int _ot)								{m_ot=_ot;}


private:
	enum
	{
		DEFAULT_X_SPEED=-2,
		DEFAULT_Y_SPEED=-3,
		DEFAULT_SPEED_SCALE=2,
		DEFAULT_OT=1000,
	};

	class SpriteBank	*m_sprites;
	int					m_xOff,m_yOff;
	int					m_xSpeed,m_ySpeed;
	int					m_speedScale;
	int					m_ot;

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

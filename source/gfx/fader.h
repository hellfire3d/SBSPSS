/*=========================================================================

	fader.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__GFX_FADER_H__
#define	__GFX_FADER_H__

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

class CFader
{
public:
	typedef enum
	{
		BLACK_FADE,
		WHITE_FADE,
	} FADE_STYLE;

	static void			render();
	static void			think(int _frames);

	static void			setFadingOut(FADE_STYLE _style=BLACK_FADE);
	static void			setFadingIn(FADE_STYLE _style=BLACK_FADE);

	static int			isFading();


private:
	enum
	{
		FRAMES_TO_WAIT=5,		// Waits before fading in/out and after fading out
		FADE_SPEED=10,
	};

	typedef enum
	{
		PAUSE_BEFORE_FADING_IN,
		FADING_IN,
		FADED_IN,

		FADING_OUT,
		PAUSE_AFTER_FADING_OUT,
		FADED_OUT,
	} FADE_MODE;
	
	static FADE_MODE	s_fadeMode;
	static FADE_STYLE	s_fadeStyle;
	static int			s_fadeValue;
	static int			s_waitFrames;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GFX_FADER_H__ */

/*===========================================================================
 end */

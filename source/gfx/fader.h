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
		BUBBLE_FADE,
		BLACK_FADE,
	} FADE_STYLE;

	static void			render();
	static void			think(int _frames);

	static void			setFadingOut(FADE_STYLE _style=BUBBLE_FADE);
	static void			setFadingIn(FADE_STYLE _style=BUBBLE_FADE);

	static int			isFading();


private:
	enum
	{
		// Bubble style
		FADE_BORDER_SIZE=80,
		FADE_SMOOTH_BORDER_SIZE=60,
		BUBBLES_PER_FRAME=30,

		// Both styles
		FADE_SPEED=3,
	};

	typedef enum
	{
		FADED_IN,
		FADING_IN,
		FADING_OUT,
		FADED_OUT,
	} FADE_MODE;
	
	static FADE_MODE	s_fadeMode;
	static FADE_STYLE	s_fadeStyle;
	static int			s_fadeValue;

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

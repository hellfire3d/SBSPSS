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
	static void			render();
	static void			think(int _frames);

	static void			setFadingOut();
	static void			setFadingIn();
	static int			isFading();


private:
	enum
	{
		FADE_BORDER_SIZE=80,
		FADE_SMOOTH_BORDER_SIZE=60,
		BUBBLES_PER_FRAME=30,
		FADE_SPEED=3,
	};

	typedef enum _fade_mode
	{
		FADED_IN,
		FADING_IN,
		FADING_OUT,
		FADED_OUT,
	} FADE_MODE;
	
	static FADE_MODE	s_fadeMode;
	static int			s_fadeLine;	

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

/*=========================================================================

	gamebubs.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__GAME_GAMEBUBS_H__
#define	__GAME_GAMEBUBS_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

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

class CGameBubicleFactory
{
public:
	typedef enum
	{
		TYPE_SMALL,
		TYPE_MEDIUM,
		TYPE_LARGE,

		NUM_TYPES
	}GAMEBUBICLETYPE;

	static void		spawnBubicles(int _x,int _y,int _w,int _h,GAMEBUBICLETYPE _type);

private:
	static struct BubicleEmitterData	s_emitters[NUM_TYPES];

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __GAME_GAMEBUBS_H__ */

/*===========================================================================
 end */

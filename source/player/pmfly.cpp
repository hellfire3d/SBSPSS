/*=========================================================================

	pmfly.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmfly.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeFly::enter()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#if	defined(__USER_paul__) 
int playerflyspeed=30;
#elif	defined(__USER_daveo__)
int playerflyspeed=10;
#else
static const int playerflyspeed=5;
#endif
void	CPlayerModeFly::think()
{
	DVECTOR	pos;
	int		controlHeld;

	pos=getPlayerPos();
	controlHeld=getPadInputHeld();
	if(controlHeld&PI_LEFT)
	{
		pos.vx-=playerflyspeed;
	}
	else if(controlHeld&PI_RIGHT)
	{
		pos.vx+=playerflyspeed;
	}
	if(controlHeld&PI_UP)
	{
		pos.vy-=playerflyspeed;
	}
	else if(controlHeld&PI_DOWN)
	{
		pos.vy+=playerflyspeed;
	}
	setPlayerPos(&pos);
}

/*===========================================================================
end */

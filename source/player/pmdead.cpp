/*=========================================================================

	pmdead.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmdead.h"

//#ifndef __PLAYER_PLAYER_H__
//#include "player\player.h"
//#endif


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
void	CPlayerModeDead::enter()
{
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeDead::think()
{
	DVECTOR	pos;
	int		controlHeld;

	pos=getPlayerPos();
	controlHeld=getPadInputHeld();
	if(controlHeld&PI_LEFT)
	{
		pos.vx-=5;
	}
	else if(controlHeld&PI_RIGHT)
	{
		pos.vx+=5;
	}
	if(controlHeld&PI_UP)
	{
		pos.vy-=5;
	}
	else if(controlHeld&PI_DOWN)
	{
		pos.vy+=5;
	}
	setPlayerPos(&pos);
}

/*===========================================================================
end */

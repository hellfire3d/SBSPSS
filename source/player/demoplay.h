/*=========================================================================

	demoplay.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_DEMOPLAY_H__
#define __PLAYER_DEMOPLAY_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __PLAYER_PLAYER_H__
#include "player/player.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CDemoPlayer : public CPlayer
{
public:
	typedef struct
	{
		u8	m_inputValue;
		u8	m_length;
	}demoPlayerControl;

protected:
	virtual PLAYERINPUT	readPadInput();
private:

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_DEMOPLAY_H__ */

/*===========================================================================
 end */

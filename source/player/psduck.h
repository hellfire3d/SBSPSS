/*=========================================================================

	psduck.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSDUCK_H__
#define __PLAYER_PSDUCK_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pstates.h"


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayerStateDuck : public CPlayerState
{
public:
	void			enter(class CPlayerModeBasic *_playerMode);
	void			think(class CPlayerModeBasic *_playerMode);

};


class CPlayerStateSoakUp : public CPlayerState
{
public:
	void			enter(class CPlayerModeBasic *_playerMode);
	void			think(class CPlayerModeBasic *_playerMode);

};


class CPlayerStateGetUp : public CPlayerState
{
public:
	void			enter(class CPlayerModeBasic *_playerMode);
	void			think(class CPlayerModeBasic *_playerMode);

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSDUCK_H__ */

/*===========================================================================
 end */




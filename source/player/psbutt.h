/*=========================================================================

	psbutt.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSBUTT_H__
#define __PLAYER_PSBUTT_H__

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

class CPlayerStateButtBounce : public CPlayerState
{
public:
	virtual void	enter(class CPlayerModeBasic *_playerMode);
	virtual void	think(class CPlayerModeBasic *_playerMode);
	
};

class CPlayerStateButtBounceFall : public CPlayerState
{
public:
	virtual void	enter(class CPlayerModeBasic *_playerMode);
	virtual void	think(class CPlayerModeBasic *_playerMode);

};

class CPlayerStateButtBounceLand : public CPlayerState
{
public:
	virtual void	enter(class CPlayerModeBasic *_playerMode);
	virtual void	think(class CPlayerModeBasic *_playerMode);

private:
	
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSBUTT_H__ */

/*===========================================================================
 end */




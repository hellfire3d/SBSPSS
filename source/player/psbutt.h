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
	virtual void	enter(class CPlayer *_player);
	virtual void	think(class CPlayer *_player);
	
};

class CPlayerStateButtBounceFall : public CPlayerState
{
public:
	virtual void	enter(class CPlayer *_player);
	virtual void	think(class CPlayer *_player);

};

class CPlayerStateButtBounceLand : public CPlayerState
{
public:
	virtual void	enter(class CPlayer *_player);
	virtual void	think(class CPlayer *_player);

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




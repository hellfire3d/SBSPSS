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
	virtual void	enter(class CPlayerModeBase *_playerMode);
	virtual void	think(class CPlayerModeBase *_playerMode);
	
};

class CPlayerStateButtBounceFall : public CPlayerState
{
public:
	virtual void	enter(class CPlayerModeBase *_playerMode);
	virtual void	think(class CPlayerModeBase *_playerMode);

};

class CPlayerStateButtBounceLand : public CPlayerState
{
public:
	virtual void	enter(class CPlayerModeBase *_playerMode);
	virtual void	think(class CPlayerModeBase *_playerMode);
	
private:
	int				m_bounceOffFloor;

};

class CPlayerStateButtBounceUp : public CPlayerState
{
public:
	virtual void	enter(class CPlayerModeBase *_playerMode);
	virtual void	think(class CPlayerModeBase *_playerMode);

private:
	int				m_bounceFrames;

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateButtBounce		s_stateButtBounce;
extern	CPlayerStateButtBounceFall	s_stateButtBounceFall;
extern	CPlayerStateButtBounceLand	s_stateButtBounceLand;
extern	CPlayerStateButtBounceUp	s_stateButtBounceUp;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSBUTT_H__ */

/*===========================================================================
 end */




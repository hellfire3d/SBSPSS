/*=========================================================================

	pslook.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSLOOK_H__
#define __PLAYER_PSLOOK_H__

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

class CPlayerStateLookDown : public CPlayerState
{
public:
	virtual void			enter(class CPlayerModeBase *_playerMode);
	virtual void			think(class CPlayerModeBase *_playerMode);

protected:
	virtual int				getNextState();
	virtual int				controlThatMustBeHeld();

};


class CPlayerStateLookDownRelax : public CPlayerState
{
public:
	virtual void			enter(class CPlayerModeBase *_playerMode);
	virtual void			think(class CPlayerModeBase *_playerMode);

};


class CPlayerStateLookUp : public CPlayerStateLookDown
{
public:
	virtual void			enter(class CPlayerModeBase *_playerMode);

protected:
	virtual int				getNextState();
	virtual int				controlThatMustBeHeld();

};


class CPlayerStateLookUpRelax : public CPlayerStateLookDownRelax
{
public:
	virtual void			enter(class CPlayerModeBase *_playerMode);

};


/*----------------------------------------------------------------------
	Globals
	------- */

extern	CPlayerStateLookDown		s_stateLookDown;
extern	CPlayerStateLookDownRelax	s_stateLookDownRelax;
extern	CPlayerStateLookUp			s_stateLookUp;
extern	CPlayerStateLookUpRelax		s_stateLookUpRelax;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSLOOK_H__ */

/*===========================================================================
 end */




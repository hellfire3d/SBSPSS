/*=========================================================================

	pstates.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pstates.h"

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
const PlayerMetrics *CPlayerState::getPlayerMetrics(CPlayer *_player)
{
	return _player->getPlayerMetrics();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::setState(CPlayer *_player,int _state)
{
	return _player->setState((PLAYER_STATE)_state);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::setMode(class CPlayer *_player,int _mode)
{
	_player->setMode((PLAYER_MODE)_mode);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::getFacing(CPlayer *_player)
{
	return _player->getFacing();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::setFacing(CPlayer *_player,int _facing)
{
	_player->setFacing(_facing);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::getAnimNo(CPlayer *_player)
{
	return _player->getAnimNo();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::setAnimNo(CPlayer *_player,int _animNo)
{
	_player->setAnimNo(_animNo);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::setAnimFrame(CPlayer *_player,int _animFrame)
{
	_player->setAnimFrame(_animFrame);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::advanceAnimFrameAndCheckForEndOfAnim(class CPlayer *_player)
{
	int	animFrame,frameCount;
	int	looped;
	animFrame=_player->getAnimFrame()+1;
	frameCount=_player->getAnimFrameCount();
	looped=false;
	if(animFrame>=frameCount)
	{
		looped=true;
		animFrame=0;
	}
	_player->setAnimFrame(animFrame);
	return looped;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::retreatAnimFrameAndCheckForEndOfAnim(class CPlayer *_player)
{
	int	animFrame;
	int	looped;
	animFrame=_player->getAnimFrame()-1;
	looped=false;
	if(animFrame<0)
	{
		looped=true;
		animFrame=_player->getAnimFrameCount()-1;
	}
	_player->setAnimFrame(animFrame);
	return looped;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR CPlayerState::getMoveVelocity(CPlayer *_player)
{
	return _player->getMoveVelocity();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::setMoveVelocity(CPlayer *_player,DVECTOR *_moveVel)
{
	_player->setMoveVelocity(_moveVel);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR CPlayerState::getPlayerPos(CPlayer *_player)
{
	return _player->getPlayerPos();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::setPlayerPos(class CPlayer *_player,DVECTOR *_pos)
{
	return _player->setPlayerPos(_pos);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::getPadInputHeld(CPlayer *_player)
{
	return _player->getPadInputHeld();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::getPadInputDown(CPlayer *_player)
{
	return _player->getPadInputDown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::isOnEdge(class CPlayer *_player)
{
	return _player->isOnEdge();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerState::canMoveLeft(class CPlayer *_player)
{
	return _player->canMoveLeft();
}
int CPlayerState::canMoveRight(class CPlayer *_player)
{
	return _player->canMoveRight();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::moveLeft(CPlayer *_player)
{
	_player->moveLeft();
}
void CPlayerState::moveRight(CPlayer *_player)
{
	_player->moveRight();
}
void CPlayerState::slowdown(CPlayer *_player)
{
	_player->slowdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::jump(CPlayer *_player)
{
	_player->jump();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::fall(CPlayer *_player)
{
	_player->fall();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerState::respawn(CPlayer *_player)
{
	_player->respawn();
}


/*===========================================================================
 end */

#include "player\pstates.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif




#ifndef	__ANIM_PLAYER_ANIM_HEADER__
#include <player_anim.h>
#endif





/*----------------------------------------------------------------------*/
const PlayerMetrics *CPlayerState::getPlayerMetrics(CPlayer *_player)
{
	return _player->getPlayerMetrics();
}
void CPlayerState::setState(CPlayer *_player,int _state)
{
	_player->setState((PLAYER_STATE)_state);
}
int CPlayerState::getFacing(CPlayer *_player)
{
	return _player->getFacing();
}
void CPlayerState::setFacing(CPlayer *_player,int _facing)
{
	_player->setFacing(_facing);
}
int CPlayerState::getAnimNo(CPlayer *_player)
{
	return _player->getAnimNo();
}
void CPlayerState::setAnimNo(CPlayer *_player,int _animNo)
{
	_player->setAnimNo(_animNo);
}
void CPlayerState::setAnimFrame(CPlayer *_player,int _animFrame)
{
	_player->setAnimFrame(_animFrame);
}
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
DVECTOR CPlayerState::getMoveVelocity(CPlayer *_player)
{
	return _player->getMoveVelocity();
}
void CPlayerState::setMoveVelocity(CPlayer *_player,DVECTOR *_moveVel)
{
	_player->setMoveVelocity(_moveVel);
}
int CPlayerState::getPadInput(CPlayer *_player)
{
	return _player->getPadInput();
}
int CPlayerState::isOnSolidGround(CPlayer *_player)
{
	return _player->isOnSolidGround();
}
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
void CPlayerState::jump(CPlayer *_player)
{
	_player->jump();
}
void CPlayerState::fall(CPlayer *_player)
{
	_player->fall();
}








/*----------------------------------------------------------------------*/
void CPlayerStateIdle::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_IDLEGENERIC04);
}
void CPlayerStateIdle::think(CPlayer *_player)
{
	int	control;
	control=getPadInput(_player);
	
	if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_JUMP))
	{
		setState(_player,STATE_JUMP);
	}
	else if(control&(CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT)|CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT)))
	{
		setState(_player,STATE_RUN);
	}
	else if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_ACTION))
	{
		setState(_player,STATE_CHOP);
	}
	else if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_DOWN))
	{
		setState(_player,STATE_DUCK);
	}
	else if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		if(getRndRange(100)<95)
			setAnimNo(_player,ANIM_PLAYER_ANIM_IDLEGENERIC04);
		else
			setAnimNo(_player,ANIM_PLAYER_ANIM_IDLEGENERIC03);
	}
}

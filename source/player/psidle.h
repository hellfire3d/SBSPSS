/*=========================================================================

	psidle.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PSIDLE_H__
#define __PLAYER_PSIDLE_H__

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

////
class CPlayerStateBaseIdle : public CPlayerState
{
public:
	void			thinkControl(class CPlayer *_player);
};


////
class CPlayerStateTeeterIdle : public CPlayerStateBaseIdle
{
public:
	virtual void	enter(class CPlayer *_player);
	virtual void	think(class CPlayer *_player);
};


////
typedef struct
{
	int		m_startFrame;					// Or -1 for none
	int		m_loopFrame;					// Must exist
	int		m_endFrame;						// Or -1 for none
	int		m_loopCount;					// Number of times to run m_loopFrame
} IdleAnims;


class CPlayerStateIdle : public CPlayerStateBaseIdle
{
public:
	virtual void	enter(class CPlayer *_player);
	virtual void	think(class CPlayer *_player);

private:
	typedef enum
	{
		ANIMSTATE_START,
		ANIMSTATE_LOOP,
		ANIMSTATE_END,
	} ANIMSTATE;

	void			setNextIdleAnim(class CPlayer *_player);

	virtual IdleAnims	*getIdleAnimsDb(int _animNo)=0;
	virtual int			getNumIdleAnims()=0;

	int					m_idleTime;				// Number of idle anims that have been started
	int					m_currentIdleAnim;
	ANIMSTATE			m_animState;
	int					m_loopCount;			// Number of times to loop middle section of anim
};


class CPlayerStateUnarmedIdle : public CPlayerStateIdle
{
private:
	virtual IdleAnims	*getIdleAnimsDb(int _animNo);
	virtual int			getNumIdleAnims();

};


class CPlayerStateCoralBlowerIdle : public CPlayerStateIdle
{
private:
	virtual IdleAnims	*getIdleAnimsDb(int _animNo);
	virtual int			getNumIdleAnims();

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSIDLE_H__ */

/*===========================================================================
 end */




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
	void			thinkControl(class CPlayerModeBase *CPlayerMode);
};


////
class CPlayerStateTeeterIdle : public CPlayerStateBaseIdle
{
public:
	virtual void	enter(class CPlayerModeBase *_playerMode);
	virtual void	think(class CPlayerModeBase *_playerMode);
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
	virtual void	enter(class CPlayerModeBase *_playerMode);
	virtual void	think(class CPlayerModeBase *_playerMode);

private:
	typedef enum
	{
		ANIMSTATE_START,
		ANIMSTATE_LOOP,
		ANIMSTATE_END,
	} ANIMSTATE;

	void			setNextIdleAnim(class CPlayerModeBase *_playerMode);

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

extern	CPlayerStateUnarmedIdle		s_stateUnarmedIdle;
extern	CPlayerStateTeeterIdle		s_stateTeeterIdle;
extern	CPlayerStateCoralBlowerIdle	s_stateCoralBlowerIdle;


/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PSIDLE_H__ */

/*===========================================================================
 end */




/*=========================================================================

	player.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PLAYER_H__
#define __PLAYER_PLAYER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GAME_THING_H__
#include "Game/Thing.h"
#endif

#ifndef __SKEL_HEADER__
#include "Gfx/Skel.h"
#endif

#ifndef __DATA_STRUCTS_HEADER__
#include <dstructs.h>
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class	CPlayer : public CThing
{
public:
	void			init();
	void			shutdown();
	void			think(int _frames);
	void			render();


private:		
	typedef enum
	{
		STATE_IDLE,
		STATE_RUNSTART,
		STATE_RUN,
		STATE_RUNSTOP,
			
		NUM_STATES,
	}PLAYER_STATE;

	enum
	{
		MAX_RUN_VELOCITY=8,
		RUN_SPEEDUP=4,
		RUN_REVERSESLOWDOWN=2,
		RUN_SLOWDOWN=1,
	};

	enum
	{
		FACING_LEFT=+1,
		FACING_RIGHT=-1,
	};
	
	void			setState(PLAYER_STATE _state);
	void			setFacing(int _facing);
	void			finishedAnim();
	virtual int		getPadInput();

	int				m_frame;
	int				m_animNo;
	PLAYER_STATE	m_state;
	CSkel			m_skel;

	int				m_runVel;
	int				m_facing;

	static int		s_stateAnims[NUM_STATES];

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PLAYER_H__ */

/*===========================================================================
 end */

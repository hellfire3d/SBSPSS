/*=========================================================================

	pmchop.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMCHOP_H__
#define __PLAYER_PMCHOP_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CPlayerModeChop : public CPlayerModeBase
{
public:
	virtual void				enter();
	virtual void				think();
	virtual void				renderModeUi();

	virtual void				setAnimNo(int _animNo);
	virtual void				setAnimFrame(int _animFrame);

	virtual ATTACK_STATE		getAttackState();

	virtual void				setPlayerCollisionSize(int _x,int _y,int _w,int _h);

private:
	enum
	{
		CHOP_ATTACK_START_FRAME=15,
		CHOP_ATTACK_END_FRAME=21,
		CHOP_TIMEOUT=20*60,
		CHOP_FLASH_TIME=4*60,
	};


	int							canAttackFromThisState();

	int							m_savedAnimNo,m_savedAnimFrame;
	int							m_chopFrame;
	int							m_chopping;
	int							m_timer;

	int							m_savedCSX,m_savedCSY,m_savedCSW,m_savedCSH;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMCHOP_H__ */

/*===========================================================================
 end */




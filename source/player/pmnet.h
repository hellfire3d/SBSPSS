/*=========================================================================

	pmnet.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PLAYER_PMNET_H__
#define __PLAYER_PMNET_H__

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

class CPlayerModeNet : public CPlayerModeBase
{
public:
	virtual void				enter();
	virtual void				think();
	virtual void				renderModeUi();

	virtual void				setAnimNo(int _animNo);
	virtual void				setAnimFrame(int _animFrame);

private:
	typedef enum
	{
		NET_STATE__EMPTY,
		NET_STATE__JUST_CAUGHT_SOMETHING,
		NET_STATE__FULL,
		NET_STATE__JUST_LAUNCHED_SOMETHING,
	} NetState;

	int							canSwingNetFromThisState();

	int							m_savedAnimNo,m_savedAnimFrame;
	int							m_netFrame;
	int							m_netting;
	NetState					m_netState;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PLAYER_PMNET_H__ */

/*===========================================================================
 end */




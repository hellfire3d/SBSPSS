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

protected:
	virtual class CPlayerState	**getStateTable();

private:
	typedef enum
	{
		NET_STATE__INERT,
		NET_STATE__CATCHING,
		NET_STATE__JUST_CAUGHT_SOMETHING,
		NET_STATE__LAUNCHING,
		NET_STATE__JUST_LAUNCHED_SOMETHING,
	} NetState;

	int							canSwingNetFromThisState();

	int							m_savedAnimNo,m_savedAnimFrame;
	int							m_netFrame;
	NetState					m_netState;
	int							m_netSin;

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




/*=========================================================================

	tsswitch.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TSSWITCH_H__
#include "triggers\tsswitch.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamSwitchEmitterTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CTrigger::setPositionAndSize( _x, _y, _w, _h );

	m_effect = CFX::Create( CFX::FX_TYPE_STEAM, Pos );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamSwitchEmitterTrigger::toggleVisible()
{
	m_effect->toggleVisible();
}

/*=========================================================================

	tsdownemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TSDOWNEMIT_H__
#include "triggers\tsdownemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#include "fx\fxsteam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamDownEmitterTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CSteamEmitterTrigger::setPositionAndSize( _x, _y, _w, _h );

	CFXSteam *steam = (CFXSteam *) m_effect;
	steam->SetSize( ( _w >> 4 ) * ONE );
	DVECTOR velocity;
	velocity.vx = 0;
	velocity.vy = _h >> 4;
	steam->SetVel( velocity );
}

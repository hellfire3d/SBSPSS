/*=========================================================================

	tsleftemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TSLEFTEMIT_H__
#include "triggers\tsleftemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#include "fx\fxsteam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamLeftEmitterTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CSteamEmitterTrigger::setPositionAndSize( _x, _y, _w, _h );

	CFXSteam *steam = (CFXSteam *) m_effect;
	steam->SetSize( ( _h >> 4 ) * ONE );
	DVECTOR velocity;
	velocity.vx = -_w >> 4;
	velocity.vy = 0;
	steam->SetVel( velocity );
	steam->SetHorizontal( true );
}

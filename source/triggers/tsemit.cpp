/*=========================================================================

	tsemit.cpp

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TSEMIT_H__
#include "triggers\tsemit.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CSteamEmitterTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CTrigger::setPositionAndSize( _x, _y, _w, _h );

	m_effect = CFX::Create( CFX::FX_TYPE_STEAM, Pos );
	//CFXBaseAnim *m_animEffect = (CFXBaseAnim *) m_effect;
	//m_animEffect->SetScaleX( ( _w * ONE ) >> 5 );
	//m_animEffect->SetScaleY( ( _h * ONE ) >> 5 );
}

/*=========================================================================

	tfemit.h

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TFEMIT_H__
#include "triggers\tfemit.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#include	"fx\fxfire.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CFlameEmitterTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	CTrigger::setPositionAndSize( _x, _y, _w, _h );

	m_effect = CFX::Create( CFX::FX_TYPE_FLAMES, Pos );
	CFXFire *FX = (CFXFire *) m_effect;
	FX->SetSize(_w,_h);
//	m_animEffect->SetScaleX( ( _w * ONE ) >> 5 );
//	m_animEffect->SetScaleY( ( _h * ONE ) >> 5 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CFlameEmitterTrigger::setHeading(int newHeading)
{
	CFXBaseAnim *m_animEffect = (CFXBaseAnim *) m_effect;
	m_animEffect->SetHeading( newHeading );	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CFlameEmitterTrigger::setSize( int _w, int _h )
{
	CFXFire *FX= (CFXFire *) m_effect;
	FX->SetSize(_w,_h);
//	m_animEffect->SetScaleX( ( _w * ONE ) >> 5 );
//	m_animEffect->SetScaleY( ( _h * ONE ) >> 5 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CFlameEmitterTrigger::collidedWith(CThing *_thisThing)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player = (CPlayer *) _thisThing;

			if ( !player->isRecoveringFromHit() )
			{
				player->takeDamage( DAMAGE__KILL_OUTRIGHT );
			}

			break;
		}

		default:
			break;
	}
}

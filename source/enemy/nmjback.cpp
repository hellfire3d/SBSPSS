/*=========================================================================

	nmjback.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NMJBACK_H__
#include "enemy\nmjback.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#include "fx\fx.h"
#include "fx\fxjfish.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishBackground::postInit()
{
	CNpcEnemy::postInit();

//	CFXJellyFishLegs	*T=(CFXJellyFishLegs*)CFX::Create(CFX::FX_TYPE_JELLYFISH_LEGS,this);
//	T->SetUp(128,4,8,16);
//	T->SetOtPos( 15 );

	m_actorGfx->SetOtPos( 15 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMotherJellyfishBackground::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),false);
			m_actorGfx->RotateScale( SprFrame, renderPos, 0, 8192, 8192 );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}
	}
}

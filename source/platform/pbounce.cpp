/*=========================================================================

	pbounce.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBOUNCE_H__
#include "platform\pbounce.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

void CNpcBouncePlatform::think( int _frames )
{
	if ( m_contact )
	{
		CPlayer *player = GameScene.getPlayer();

		player->springPlayerUp();

		m_contact = false;
	}

	CPlatformThing::think(_frames);
}
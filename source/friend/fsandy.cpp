/*=========================================================================

	fsandy.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FSANDY_H__
#include "friend\fsandy.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef	__ANIM_SANDY_HEADER__
#include <ACTOR_SANDY_Anim.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSandyFriend::postInit()
{
	switch( CLevel::getCurrentChapter() )
	{
		case 3:
		{
			m_conversation = SCRIPTS_CH3L2_01_DAT;

			break;
		}

		default:
		{
			m_conversation = SCRIPTS_CH1L2_03_DAT;

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSandyFriend::startConderversation()
{
	if( !CConversation::isActive() )
	{
		CConversation::trigger( m_conversation );
	}
}